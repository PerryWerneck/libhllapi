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
 * programa; se não, escreva para a Free Software Foundation, Inc., 51 Franklin
 * St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Este programa está nomeado como - e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

/**
 * @file
 *
 * @brief Win32 delayed loading support methods.
 *
 * @author perry.werneck@gmail.com
 *
 */

 #include <winsock2.h>
 #include <windows.h>

 #ifdef _MSC_VER
	#pragma comment(lib,"Advapi32.lib")
	#pragma comment(lib,"Delayimp.lib")
	#pragma comment(lib,"ipc3270.lib")
	#include <Delayimp.h>
 #else
	#include <config.h>
 #endif // !_MSC_VER

 #include "../private.h"
 #include <lmcons.h>
 #include <delayimp.h>
 #include <fcntl.h>
 #include <string>
 #include <stdexcept>
 #include <lib3270.h>
 #include <lib3270/hllapi.h>

 #ifdef HAVE_LIBINTL
	#include <libintl.h>
 #endif // HAVE_LIBINTL

 using namespace std;

 extern "C" {

 #ifdef _MSC_VER

	#pragma comment(linker, "/EXPORT:DllRegisterServer=DllRegisterServer,PRIVATE")
	#pragma comment(linker, "/EXPORT:DllUnregisterServer=DllUnregisterServer,PRIVATE")
	#pragma comment(linker, "/EXPORT:DllInstall=DllInstall,PRIVATE")

	extern HRESULT DllRegisterServer();
	extern HRESULT DllInstall(BOOL, PCWSTR);
	extern HRESULT DllUnregisterServer();

 #else

	extern __declspec (dllexport) HRESULT DllRegisterServer();
	extern __declspec (dllexport) HRESULT DllInstall(BOOL, PCWSTR);
	extern __declspec (dllexport) HRESULT DllUnregisterServer();

 #endif // MSC_VER

	FARPROC WINAPI hllapi_delay_load_hook(unsigned reason, DelayLoadInfo * info);

 }

#ifdef USING_STATIC_IPC3270

	// https://docs.microsoft.com/en-us/cpp/build/reference/loading-all-imports-for-a-delay-loaded-dll?view=vs-2019
	#ifdef _MSC_VER
		const PfnDliHook __pfnDliNotifyHook2 = hllapi_delay_load_hook;
		const PfnDliHook __pfnDliFailureHook2 = hllapi_delay_load_hook;
	#else
		PfnDliHook __pfnDliNotifyHook2 = hllapi_delay_load_hook;
		PfnDliHook __pfnDliFailureHook2 = hllapi_delay_load_hook;
	#endif // _MSC_VER

#endif // USING_STATIC_IPC3270

/*---[ Implement ]----------------------------------------------------------------------------------*/

 static HANDLE hModule = 0;
 static HANDLE hEventLog = 0;

 HRESULT DllRegisterServer() {
 	return S_OK;
 }

 HRESULT DllUnregisterServer() {
 	return S_OK;
 }

 HRESULT DllInstall(BOOL bInstall, PCWSTR pszCmdLine) {
 	return S_OK;
 }

 BOOL WINAPI DllMain(HANDLE hInstance, DWORD dwcallpurpose, LPVOID GNUC_UNUSED(lpvResvd)) {

    switch(dwcallpurpose) {
    case DLL_PROCESS_ATTACH:
        hModule = hInstance;
        hEventLog = RegisterEventSource(NULL, LIB3270_STRINGIZE_VALUE_OF(PRODUCT_NAME));
        break;

	case DLL_PROCESS_DETACH:
		hModule = 0;
		DeregisterEventSource(hEventLog);
		hEventLog = 0;
		break;

    }

    return TRUE;
 }

 HLLAPI_API_CALL hllapi_report_event(LPSTR message) {

	char	username[UNLEN + 1];
	DWORD	szName = sizeof(username);

	memset(username,0,UNLEN + 1);

	if(!GetUserName(username, &szName)) {
		username[0] = 0;
	}

	const char *outMsg[] = {
			username,
			PACKAGE_NAME,
			message
	};

	debug("Event: \"%s\"",message);

	ReportEvent(
			hEventLog,
			EVENTLOG_ERROR_TYPE,
			1,
			0,
			NULL,
			3,
			0,
			outMsg,
			NULL
	);

	return HLLAPI_STATUS_SUCCESS;

 }


#ifdef USING_STATIC_IPC3270

 static void dummyProc() {
	throw std::runtime_error(_("Operation not supported"));
 }

 FARPROC WINAPI hllapi_delay_load_hook(unsigned reason, DelayLoadInfo * info) {

	static string savedpath;

	printf("%s\n",__FUNCTION__);

	// https://docs.microsoft.com/en-us/cpp/build/reference/structure-and-constant-definitions?view=vs-2019
	switch (reason) {
	case dliNoteStartProcessing:
		{

			char datadir[4096];
			memset(datadir,' ',sizeof(datadir));
			datadir[sizeof(datadir)-1] = 0;

			hllapi_get_datadir(datadir);

			if(*datadir) {

				char curdir[4096];
				memset(curdir,0,sizeof(curdir));

				if(GetCurrentDirectory(sizeof(curdir)-1,curdir)) {
					savedpath = curdir;
				} else {
					savedpath.clear();
				}

				SetCurrentDirectory(datadir);

			}

		}
		break;

	case dliNoteEndProcessing:
		if(!savedpath.empty()) {

			SetCurrentDirectory(savedpath.c_str());
			savedpath.clear();

		}
		break;

	case dliNotePreLoadLibrary:
		break;

	case dliNotePreGetProcAddress:
		break;

	case dliFailLoadLib:
		{
			string msg = "Can't load ";
			msg += (const char *) info->szDll;
			hllapi_report_event((LPSTR) msg.c_str());
		}
		break;

	case dliFailGetProc:
		{
			// TODO: Check usage of RaiseException ?
			string msg = "Can't find method on ";
			msg += (const char *) info->szDll;
			hllapi_report_event((LPSTR) msg.c_str());
		}
		return (FARPROC) dummyProc;

	}

	// Returning NULL causes the delay load machinery to perform default
	// processing for this notification.
	return NULL;

 }

#endif // USING_STATIC_IPC3270
