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

 static DWORD set(std::function<void(TN3270::Host &)> worker) noexcept {

 	try {

		TN3270::Host &host = getSession();

		if(!host.isConnected())
			return HLLAPI_STATUS_DISCONNECTED;

		worker(host);

		return HLLAPI_STATUS_SUCCESS;

	} catch(const std::exception &e) {

		hllapi_lasterror = e.what();

	} catch(...) {

		hllapi_lasterror = "Unexpected error";

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_set_cursor_address(WORD pos) {

	return set([pos](TN3270::Host &host) {
		host.setCursor((unsigned short) pos -1);
	});


 }

 HLLAPI_API_CALL hllapi_setcursor(WORD pos) {

	return set([pos](TN3270::Host &host) {
		host.setCursor((unsigned short) pos -1);
	});

 }

 HLLAPI_API_CALL hllapi_set_cursor_position(WORD row, WORD col) {

	return set([row,col](TN3270::Host &host) {
		host.setCursor((unsigned short) row, (unsigned short) col);
	});

 }

 HLLAPI_API_CALL hllapi_get_cursor_address() {

 	try {

		TN3270::Host &host = getSession();

		if(!host.isConnected())
			return 0;

		return (DWORD) (host.getCursorAddress()+1);

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();
		return -1;

	}

	return -1;
 }

 HLLAPI_API_CALL hllapi_getcursor() {
 	return hllapi_get_cursor_address();
 }

