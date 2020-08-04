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

 static DWORD set(std::function<void(TN3270::Host &)> worker) noexcept {

	try {

		worker(getSession());
		return HLLAPI_STATUS_SUCCESS;

	} catch(const std::system_error &e) {

		return hllapi_translate_error(e);

	} catch(const std::exception &e) {

		// Error getting session or lock state
		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;

	} catch(...) {

		// Unexpected error getting session or lock state
		hllapi_lasterror = _( "Unexpected error" );

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_set_text_at(WORD row, WORD col, LPSTR text) {

	if(!(text && *text)) {
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;
	}

	return set([row,col,text](TN3270::Host &host) {

		host.push(row,col,(const char *) text);

	});


 }

 HLLAPI_API_CALL hllapi_input_string(LPSTR text, WORD length)
 {

	if(!(text && *text)) {
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;
	}

	if(length < 1)
		length = strlen(text);

	return set([text,length](TN3270::Host &host) {

		host.input((const char *) text, (int) length, '@');

	});

 }

 HLLAPI_API_CALL hllapi_emulate_input(const LPSTR text, WORD length, WORD GNUC_UNUSED(pasting))
 {
	if(!(text && *text)) {
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;

	}

	if(!length)
		length = strlen(text);

	return set([text,length](TN3270::Host &host) {

		host.input((const char *) text, (int) length, '@');

	});

 }
