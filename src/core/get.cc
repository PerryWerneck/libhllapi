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

 static DWORD get(std::function<void(TN3270::Host &)> worker) noexcept {

	try {

		TN3270::Host &host = getSession();

		host.waitForReady();
		worker(host);

	} catch(const std::system_error &e) {

		return hllapi_translate_error(e);

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;

	} catch(...) {

		hllapi_lasterror = _( "Unexpected error" );
		return HLLAPI_STATUS_SYSTEM_ERROR;

	}

 	return HLLAPI_STATUS_SUCCESS;

 }

 HLLAPI_API_CALL hllapi_get_screen_at(WORD row, WORD col, LPSTR buffer) {

	if(!(buffer && *buffer)) {
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;
	}

	return get([row,col,buffer](TN3270::Host &host) {

		size_t length = strlen(buffer);
		string contents = host.toString( (unsigned int) row, (unsigned int) col, (int) length);

		strncpy((char *) buffer, contents.c_str(), std::min(length,contents.size()));

	});

 }

 HLLAPI_API_CALL hllapi_get_screen(WORD offset, LPSTR buffer, WORD len) {

	if(!(buffer && *buffer)) {
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;
	}

	if(len == 0) {
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;
	}

	return get([offset,buffer,len](TN3270::Host &host) {

		string contents = host.toString((int) offset, (int) len);

		memset(buffer,' ',len);
		strncpy((char *) buffer, contents.c_str(), std::min((size_t) len,contents.size()));

	});


 }

 HLLAPI_API_CALL hllapi_get_lu_name(LPSTR buffer, WORD len) {

	if(!(buffer && *buffer)) {
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;
	}

	if(len == 0){
		hllapi_lasterror = _( "Invalid parameter" );
		return HLLAPI_STATUS_BAD_PARAMETER;
	}

	return get([buffer,len](TN3270::Host &host) {

		string luname = host.getAssociatedLUName();
		memset(buffer,' ',len);
		strncpy((char *) buffer, luname.c_str(), std::min((size_t) len,luname.size()));

	});

 }
