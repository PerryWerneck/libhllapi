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
 #include <errno.h>

/*--[ Implement ]------------------------------------------------------------------------------------*/

 DWORD hllapi_translate_error(LIB3270_KEYBOARD_LOCK_STATE state) {

	// Is unlocked.
	if(state == LIB3270_KL_UNLOCKED)
		return HLLAPI_STATUS_SYSTEM_ERROR;

	// Is connected?
	if((state & LIB3270_KL_NOT_CONNECTED) != 0)
		return HLLAPI_STATUS_DISCONNECTED;

	if( (state & (LIB3270_KL_AWAITING_FIRST|LIB3270_KL_OIA_TWAIT)) != 0)
		return HLLAPI_STATUS_WAITING;

	return HLLAPI_STATUS_KEYBOARD_LOCKED;

 }

 DWORD hllapi_translate_error(const std::system_error &error) {

	static const struct Translate {
		int from;
		HLLAPI_STATUS to;
	} translate[] = {
		{ ENOTCONN,		HLLAPI_STATUS_DISCONNECTED		},
		{ ECONNRESET,	HLLAPI_STATUS_DISCONNECTED		},
#ifndef _WIN32
		{ EHOSTDOWN,	HLLAPI_STATUS_DISCONNECTED		},
#endif // !_WIN32
		{ EINVAL,		HLLAPI_STATUS_BAD_PARAMETER		},
		{ ETIMEDOUT,	HLLAPI_STATUS_TIMEOUT			},
		{ EBUSY,		HLLAPI_STATUS_TIMEOUT			},
		{ EPERM,		HLLAPI_STATUS_KEYBOARD_LOCKED	},
		{ EROFS,		HLLAPI_STATUS_KEYBOARD_LOCKED	},
		{ EOVERFLOW,	HLLAPI_STATUS_BAD_POSITION		},
	};

	int value = error.code().value();

	hllapi_lasterror = error.what();

	hllapi_lasterror += " (rc=";
	hllapi_lasterror += std::to_string(value);
	hllapi_lasterror += ")";

	for(size_t ix = 0; ix < (sizeof(translate)/sizeof(struct Translate)); ix++) {

		if(translate[ix].from == value)
			return translate[ix].to;

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 DWORD hllapi_translate_error(LIB3270_MESSAGE state) {

	switch(state) {
		case LIB3270_MESSAGE_NONE:
			return HLLAPI_STATUS_SUCCESS;

		case LIB3270_MESSAGE_DISCONNECTED:
			return HLLAPI_STATUS_DISCONNECTED;

		case LIB3270_MESSAGE_MINUS:
		case LIB3270_MESSAGE_PROTECTED:
		case LIB3270_MESSAGE_NUMERIC:
		case LIB3270_MESSAGE_OVERFLOW:
		case LIB3270_MESSAGE_INHIBIT:
		case LIB3270_MESSAGE_KYBDLOCK:
			return HLLAPI_STATUS_KEYBOARD_LOCKED;

		default:
			return HLLAPI_STATUS_SYSTEM_ERROR;
	}

	return HLLAPI_STATUS_SYSTEM_ERROR;
 }
