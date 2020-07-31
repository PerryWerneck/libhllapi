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

/*--[ Implement ]------------------------------------------------------------------------------------*/

 HLLAPI_API_CALL hllapi_get_revision(void)
 {
 	try {

 		return atoi(getSession().getRevision().c_str());

	} catch(const std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return (DWORD) -1;
 }

 HLLAPI_API_CALL hllapi_connect(const LPSTR uri, WORD wait)
 {
 	try {

 		getSession().connect((const char *) uri);

 		if(wait)
			return hllapi_wait_for_ready(wait);

	} catch(const std::exception &e) {

		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;

	} catch(...) {

		hllapi_lasterror = _( "Unexpected error" );
		return HLLAPI_STATUS_SYSTEM_ERROR;

	}

	return hllapi_get_state();

 }

 HLLAPI_API_CALL hllapi_is_connected(void) {

 	try {

 		return getSession().isConnected();

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();
	}

	return HLLAPI_STATUS_SUCCESS;

 }

 HLLAPI_API_CALL hllapi_get_state(void) {

//	debug("",__FUNCTION__);

 	try {

		switch(hllapi_get_message_id()) {
		case LIB3270_MESSAGE_NONE:					// 0 - No message
			return HLLAPI_STATUS_SUCCESS;			// keyboard was unlocked and ready for input.

		case LIB3270_MESSAGE_DISCONNECTED:			// 4 - Disconnected from host
			hllapi_lasterror = _("Disconnected from host");
			return HLLAPI_STATUS_DISCONNECTED;		// Your application program was not connected to a valid session.

		case LIB3270_MESSAGE_MINUS:
		case LIB3270_MESSAGE_PROTECTED:
		case LIB3270_MESSAGE_NUMERIC:
		case LIB3270_MESSAGE_OVERFLOW:
		case LIB3270_MESSAGE_INHIBIT:
		case LIB3270_MESSAGE_KYBDLOCK:
			hllapi_lasterror = _("keyboard is locked");
			return HLLAPI_STATUS_KEYBOARD_LOCKED;	// keyboard is locked.

		case LIB3270_MESSAGE_SYSWAIT:
		case LIB3270_MESSAGE_TWAIT:
		case LIB3270_MESSAGE_AWAITING_FIRST:
		case LIB3270_MESSAGE_X:
		case LIB3270_MESSAGE_RESOLVING:
		case LIB3270_MESSAGE_CONNECTING:
			return HLLAPI_STATUS_WAITING;			// time-out while still busy (in XCLOCK or XSYSTEM in X) for the 3270 terminal emulation.
		}

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;
	}

	hllapi_lasterror = _( "Unexpected message id" );
	return HLLAPI_STATUS_SYSTEM_ERROR;
 }

 HLLAPI_API_CALL hllapi_get_message_id(void) {

 	try {

		return (DWORD) getSession().getProgramMessage();

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return -1;

 }

 HLLAPI_API_CALL hllapi_disconnect(void) {

	trace("%s",__FUNCTION__);
 	try {

		getSession().disconnect();

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;

	}

	return HLLAPI_STATUS_SUCCESS;

 }

 HLLAPI_API_CALL hllapi_wait_for_ready(WORD seconds) {

 	try {

		TN3270::Host &host = getSession();

		if(host.isConnected())
			host.waitForReady((unsigned int) seconds);

		return hllapi_get_state();

	} catch(const std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_wait_for_change(WORD seconds) {

 	try {

		TN3270::Host &host = getSession();

		if(host.isConnected())
			host.waitForChange((unsigned int) seconds);

		return hllapi_get_state();

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_set_charset(LPSTR charset) {

   	try {

		getSession().setCharSet((const char *) charset);
		return HLLAPI_STATUS_SUCCESS;

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;
 }


 HLLAPI_API_CALL hllapi_wait(WORD seconds) {

 	try {

		getSession().wait(seconds);

		return hllapi_get_state();

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_cmp_text_at(WORD row, WORD col, LPSTR text) {

 	try {

		return getSession().compare((unsigned int) row, (unsigned int) col, text);

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_wait_for_text_at(WORD row, WORD col, LPSTR text) {

 	try {

		getSession().wait((unsigned short) row, (unsigned short) col, (const char *) text);

		return HLLAPI_STATUS_SUCCESS;

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_cmp_text_at_address(WORD addr, LPSTR text) {

 	try {

		return getSession().compare((int) addr, text);

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }


 HLLAPI_API_CALL hllapi_find_text(const LPSTR text) {

 	try {

 		if(text && *text) {

			size_t pos = getSession().find(text);
			if(pos != string::npos)
				return pos;

 		}

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return -1;

 }

