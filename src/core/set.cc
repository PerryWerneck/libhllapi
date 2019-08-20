/*
 * "Software pw3270, desenvolvido com base nos códigos fontes do WC3270  e X3270
 * (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
 * aplicativos mainframe. Registro no INPI sob o nome G3270.
 *
 * Copyright (C) <2008> <Banco do Brasil S.A.>
 *
 * Este programa é software livre. Você pode redistribuí-lo e/ou modificá-lo sob
 * os termos da GPL v.2 - Licença Pública Geral  GNU,  conforme  publicado  pela
 * Free Software Foundation.
 *
 * Este programa é distribuído na expectativa de  ser  útil,  mas  SEM  QUALQUER
 * GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou  de  ADEQUAÇÃO
 * A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença Pública Geral GNU para
 * obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
 * programa;  se  não, escreva para a Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA, 02111-1307, USA
 *
 * Este programa está nomeado como - e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

 #include "private.h"
 #include <lib3270/keyboard.h>

 #include <string>
 #include <functional>


/*--[ Implement ]------------------------------------------------------------------------------------*/

 DWORD hllapi_translate_keyboard_state(LIB3270_KEYBOARD_LOCK_STATE state, HLLAPI_STATUS def = HLLAPI_STATUS_SYSTEM_ERROR) {

	// Is unlocked.
	if(state == LIB3270_KL_UNLOCKED)
		return def;

	// Is connected?
	if((state & LIB3270_KL_NOT_CONNECTED) != 0)
		return HLLAPI_STATUS_DISCONNECTED;

	if( (state & (LIB3270_KL_AWAITING_FIRST|LIB3270_KL_OIA_TWAIT)) != 0)
		return HLLAPI_STATUS_WAITING;

	return HLLAPI_STATUS_KEYBOARD_LOCKED;

 }

 static DWORD set(std::function<void(TN3270::Host &)> worker) noexcept {

	LIB3270_KEYBOARD_LOCK_STATE kLock = LIB3270_KL_UNLOCKED;

	try {

		TN3270::Host &host = getSession();

		kLock = host.waitForKeyboardUnlock();
		if(kLock == LIB3270_KL_UNLOCKED) {

			try {

				worker(host);
				return HLLAPI_STATUS_SUCCESS;

			} catch(const std::exception &e) {

				// Worker has failed!
				hllapi_lasterror = e.what();
			}

			// Failed! Get lock state.
			kLock = host.getKeyboardLockState();

		}


	} catch(const std::exception &e) {

		// Error getting session or lock state
		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;

	} catch(...) {

		// Unexpected error getting session or lock state
		hllapi_lasterror = "Unexpected error";
		return HLLAPI_STATUS_SYSTEM_ERROR;

	}

	return hllapi_translate_keyboard_state(kLock);

 }

 HLLAPI_API_CALL hllapi_set_text_at(WORD row, WORD col, LPSTR text) {

	if(!(text && *text))
		return HLLAPI_STATUS_BAD_PARAMETER;

	return set([row,col,text](TN3270::Host &host) {

		host.push(row,col,(const char *) text);

	});


 }

 HLLAPI_API_CALL hllapi_input_string(LPSTR text, WORD length)
 {

	if(!(text && *text))
		return HLLAPI_STATUS_BAD_PARAMETER;

	return set([text,length](TN3270::Host &host) {

		host.input((const char *) text, (int) length, '@');

	});

 }

 /*
 HLLAPI_API_CALL hllapi_emulate_input(const LPSTR buffer, WORD len, WORD pasting)
 {
	if(!hllapi_is_connected())
		return HLLAPI_STATUS_DISCONNECTED;

	try
	{
		session::get_default()->input_string(buffer);
	}
	catch(std::exception &e)
	{
		return HLLAPI_STATUS_SYSTEM_ERROR;
	}

	return HLLAPI_STATUS_SUCCESS;
 }


 */
