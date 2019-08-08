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

 static DWORD action(const TN3270::Action id) {

 	try {

		TN3270::Host &host = getSession();

		if(!host.isConnected())
			return HLLAPI_STATUS_DISCONNECTED;

		host.push(id);

		return 0;

	} catch(std::exception &e) {

		hllapi_lasterror = e.what();

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 HLLAPI_API_CALL hllapi_enter(void) {
	return action(TN3270::ENTER);
 }

 HLLAPI_API_CALL hllapi_erase(void) {
 	return action(TN3270::ERASE);
 }

 HLLAPI_API_CALL hllapi_erase_eof(void) {
 	return action(TN3270::ERASE_EOF);
 }

 HLLAPI_API_CALL hllapi_erase_eol(void) {
 	return action(TN3270::ERASE_EOL);
 }

 HLLAPI_API_CALL hllapi_erase_input(void) {
 	return action(TN3270::ERASE_INPUT);
 }

/*

 HLLAPI_API_CALL hllapi_reset(void)
 {
	return HLLAPI_STATUS_SUCCESS;
 }

 HLLAPI_API_CALL hllapi_action(LPSTR buffer) {
 	try
 	{
		session::get_default()->action((const char *) buffer);
 	}
	catch(std::exception &e)
	{
		return HLLAPI_STATUS_SYSTEM_ERROR;
	}
	return HLLAPI_STATUS_SUCCESS;
 }

 HLLAPI_API_CALL hllapi_print(void)
 {
	return session::get_default()->print();
 }

 */
