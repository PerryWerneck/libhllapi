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

		hllapi_lasterror = "Unexpected error";
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

 	try {

		switch(hllapi_get_message_id()) {
		case LIB3270_MESSAGE_NONE:					// 0 - No message
			return HLLAPI_STATUS_SUCCESS;			// keyboard was unlocked and ready for input.

		case LIB3270_MESSAGE_DISCONNECTED:			// 4 - Disconnected from host
			return HLLAPI_STATUS_DISCONNECTED;		// Your application program was not connected to a valid session.

		case LIB3270_MESSAGE_MINUS:
		case LIB3270_MESSAGE_PROTECTED:
		case LIB3270_MESSAGE_NUMERIC:
		case LIB3270_MESSAGE_OVERFLOW:
		case LIB3270_MESSAGE_INHIBIT:
		case LIB3270_MESSAGE_KYBDLOCK:
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

	hllapi_lasterror = "Unexpected message id";
	return HLLAPI_STATUS_SYSTEM_ERROR;
 }

 HLLAPI_API_CALL hllapi_get_message_id(void) {

 	try {

		TN3270::Host & session = getSession();

		if(!session.isConnected()) {
			hllapi_lasterror = strerror(ENOTCONN);
			return HLLAPI_STATUS_DISCONNECTED;
		}

		return (DWORD) session.getProgramMessage();

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;

	}

	return HLLAPI_STATUS_SUCCESS;

 }

 HLLAPI_API_CALL hllapi_disconnect(void) {

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

 HLLAPI_API_CALL hllapi_set_charset(LPSTR text) {

   	try {

		getSession().setCharSet((const char *) text);
		return HLLAPI_STATUS_SUCCESS;

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;
 }

 HLLAPI_API_CALL hllapi_get_datadir(LPSTR datadir) {

 #ifdef _WIN32
	HKEY 			hKey	= 0;
 	unsigned long	datalen = strlen(datadir);

	*datadir = 0;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\pw3270",0,KEY_QUERY_VALUE,&hKey) == ERROR_SUCCESS)
	{
		unsigned long datatype;					// #defined in winnt.h (predefined types 0-11)
		if(RegQueryValueExA(hKey,"datadir",NULL,&datatype,(LPBYTE) datadir,&datalen) != ERROR_SUCCESS)
			*datadir = 0;
		RegCloseKey(hKey);
	}
#endif // _WIN32

	return *datadir;
 }

 HLLAPI_API_CALL hllapi_wait(WORD seconds) {

 	try {

		TN3270::Host &host = getSession();

		if(!host.isConnected())
			return HLLAPI_STATUS_DISCONNECTED;

		host.wait(seconds);

		return hllapi_get_state();

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_cmp_text_at(WORD row, WORD col, LPSTR text) {

 	try {

		TN3270::Host &host = getSession();

		if(!host.isConnected())
			return HLLAPI_STATUS_DISCONNECTED;

		string contents = host.toString(row, col, strlen(text), 0);

		return strncmp(contents.c_str(),text,strlen(text));

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_find_text(LPSTR text) {

 	try {

		TN3270::Host &host = getSession();

		if(!host.isConnected()) {
			hllapi_lasterror = "Not connected";
			return -1;
		}

		string contents = host.toString(0,-1,0);

		size_t pos = contents.find(text);

		if(pos == string::npos) {
			hllapi_lasterror = "Not found";
			return -1;
		}

		return pos;

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return -1;

 }

 DWORD hllapi_translate_keyboard_state(LIB3270_KEYBOARD_LOCK_STATE state, HLLAPI_STATUS def) {

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

