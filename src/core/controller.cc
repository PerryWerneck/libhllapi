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

/*--[ Globals ]--------------------------------------------------------------------------------------*/

 static TN3270::Host	* hllapi_host		= nullptr;
 std::string		  	  hllapi_lasterror	= "";

/*--[ Implement ]------------------------------------------------------------------------------------*/

 HLLAPI_API_CALL hllapi_init(LPSTR id)
 {
 	trace("%s(%s)",__FUNCTION__,id);

#if defined(HAVE_LIBINTL) && defined(_WIN32)
	{
		static bool initialized = false;

		if(!initialized) {
			initialized = true;

			std::string localedir = TN3270::getInstallLocation();
			localedir += "\\locale";

			bindtextdomain(PACKAGE_NAME, localedir.c_str());
		}

	}
#endif // HAVE_LIBINTL && _WIN32

	try
	{
		if(hllapi_host)
			delete hllapi_host;

#ifdef _WIN32
		// TODO: Detect the current value (maybee something like g_charset)
		hllapi_host = new TN3270::Host(*id ? id : nullptr, "CP1252");
#else
		hllapi_host = new TN3270::Host(*id ? id : nullptr, "UTF-8");
#endif // _WIN32

		debug("Will set timeout to %d",120);

		hllapi_host->setTimeout(120);

	} catch(const std::exception &e) {
		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;
	} catch(...) {
		hllapi_lasterror = "Unexpected error";
		return HLLAPI_STATUS_SYSTEM_ERROR;
	}

	debug("hllapi_host=%p",hllapi_host);
 	return HLLAPI_STATUS_SUCCESS;

 }

 HLLAPI_API_CALL hllapi_deinit(void)
 {
 	trace("%s()",__FUNCTION__);

 	try
 	{
		if(hllapi_host)
		{
			delete hllapi_host;
			hllapi_host = NULL;
		}

	}
	catch(std::exception &e)
	{
		hllapi_lasterror = e.what();
		return HLLAPI_STATUS_SYSTEM_ERROR;
	}

 	return HLLAPI_STATUS_SUCCESS;
 }

 TN3270::Host & getSession() {

 	if(!hllapi_host) {
		throw runtime_error( _("Not initialized") );
 	}

 //	trace("%s()=%p",__FUNCTION__,hllapi_host);
 	return *hllapi_host;

 }

HLLAPI_API_CALL hllapi_get_last_error(LPSTR lpBuffer, WORD len) {

	strncpy(lpBuffer,hllapi_get_last_error_as_cstring(),len-1);
	lpBuffer[len-1] = 0; // Just in case

	return strlen(lpBuffer);
}

 HLLAPI_API_CSTR hllapi_get_last_error_as_cstring() {

 	return hllapi_lasterror.c_str();

 }

