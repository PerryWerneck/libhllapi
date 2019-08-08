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

 #ifdef __cplusplus
 extern "C" {
 #endif

 #define HLLAPI_MAXLENGTH					4096

 /**
  * @brief Function codes
  *
  * Reference http://www.ibm.com/support/knowledgecenter/SSEQ5Y_6.0.0/com.ibm.pcomm.doc/books/html/emulator_programming08.htm
  *
  */
 typedef enum hllapi_cmd {
	HLLAPI_CMD_CONNECTPS			= 1,		/// @brief connect presentation space
	HLLAPI_CMD_DISCONNECTPS			= 2,		/// @brief disconnect presentation space
	HLLAPI_CMD_INPUTSTRING			= 3,		/// @brief send string
	HLLAPI_CMD_WAIT					= 4,		/// @brief Wait if the session is waiting for a host response
	HLLAPI_CMD_COPYPS				= 5,		/// @brief Copies the contents of the presentation space into a string buffer.
	HLLAPI_CMD_SEARCHPS				= 6,		/// @brief Search the presentation space for a specified string.
	HLLAPI_CMD_QUERYCURSOR			= 7,		/// @brief Determines the location of the cursor in the presentation space.
	HLLAPI_CMD_COPYPSTOSTR			= 8,		/// @brief Copy presentation space to string
	HLLAPI_SET_SESSION_PARAMETERS	= 9,		/// @brief Lets you change certain default session options in EHLLAPI for all sessions.
	HLLAPI_CMD_COPYSTRTOPS			= 15,		/// @brief Copies an ASCII string directly to a specified position in the presentation space.
	HLLAPI_CMD_PAUSE				= 18,		/// @brief Waits for a specified amount of time.
	HLLAPI_RESET_SYSTEM				= 21,		/// @brief Reinitializes EHLLAPI to its starting state.
	HLLAPI_CMD_SETCURSOR			= 40,		/// @brief Places the cursor at a specified position in presentation space.
	HLLAPI_CMD_SENDFILE				= 90,		/// @brief Send file to the host
	HLLAPI_CMD_RECEIVEFILE			= 91,		/// @brief Receive a file from the host

	HLLAPI_CMD_GETREVISION			= 2000		/// @brief Get lib3270 revision
 } HLLAPI_CMD;


 /**
  * @brief Standard Return Codes
  *
  * http://ps-2.kev009.com/tl/techlib/manuals/adoclib/3270hcon/hconugd/retrncod.htm#C20819C058kmar
  */
 typedef enum hllapi_status {

	/// @brief Good return code.
	///
	/// Either the function was successfully executed, or there were no host updates since the last call was issued.
	HLLAPI_STATUS_SUCCESS = 0,

	/// @brief The presentation space was not valid or not connected.
	///
	///	Either the presentation space short session ID was invalid, or the application is not connected.
	HLLAPI_STATUS_DISCONNECTED = 1,

	/// @brief An incorrect option was specified.
	HLLAPI_STATUS_BAD_PARAMETER	= 2,

	/// @brief The execution of the function was inhibited because the target presentation space was busy.
	///
	///	For example, X or XSYSTEM is displayed in the OIA for the 3270 terminal emulation.
	HLLAPI_STATUS_TIMEOUT = 4,

	/// @brief	The execution of the function was inhibited for some reason other than the reasons stated in return code 4.
	HLLAPI_STATUS_KEYBOARD_LOCKED = 5,

	// 06	A data error occurred due to specification of an invalid parameter (for example, a length error causing truncation).
	// 07	The specified presentation space position was invalid.
	// 08	No prerequisite function was issued.

	/// @brief A system error occurred.
	HLLAPI_STATUS_SYSTEM_ERROR = 9,

	/// @brief The function number is not supported by the emulation program.
	HLLAPI_STATUS_UNSUPPORTED = 10,

	/// @brief Resource unavailable at this time.
	///
	/// The resource that you requested is unavailable (for example, too many attempts have been made to connect to the same presentation space (PS) or another application is connected to the PS).
	HLLAPI_STATUS_UNAVAILABLE = 11,

	// 20	Invalid keystroke caused by ESC= option.
	// 21	OIA was updated.
	// 22	PS was updated.
	// 23	Both OIA and PS were updated.

	/// @brief Either the string was not found, or the presentation space is unformatted.
	HLLAPI_STATUS_NOT_FOUND = 24,

	// 25	Keystrokes were not available on input queue.
	// 26	A host event occurred. See QUERY HOST UPDATE (24) for details.
	// 28	Field length was 0.
	// 31	Keystroke queue overflow. Keystrokes were lost.

 } HLLAPI_STATUS;

 #define HLLAPI_STATUS_WAITING	HLLAPI_STATUS_TIMEOUT

 #if defined(_WIN32)

	#include <windows.h>

	// http://www.mingw.org/wiki/Visual_Basic_DLL
	#define HLLAPI_API_CALL __declspec (dllexport) DWORD __stdcall
	#define HLLAPI_API_CSTR __declspec (dllexport) const char * __stdcall

 #else

	#include <cstdint>

	// From wtypesbase.h
	typedef uint8_t BYTE;
	typedef uint16_t WORD;
	typedef unsigned int UINT;
	typedef int INT;
	typedef uint32_t LONG;
	typedef LONG WINBOOL;
	typedef uint32_t DWORD;
	typedef void *HANDLE;
	typedef WORD *LPWORD;
	typedef DWORD *LPDWORD;
	typedef char CHAR;
	typedef CHAR *LPSTR;
	typedef const CHAR *LPCSTR;
	typedef char WCHAR;
	typedef WCHAR TCHAR;
	typedef WCHAR *LPWSTR;
	typedef TCHAR *LPTSTR;
	typedef const WCHAR *LPCWSTR;
	typedef const TCHAR *LPCTSTR;
	typedef HANDLE *LPHANDLE;

	#define LPWORD				uint16_t *

	#define LPSTR				char *
	#define HANDLE				int

	#define HLLAPI_API_CALL		__attribute__((visibility("default"))) extern DWORD
	#define HLLAPI_API_CSTR		__attribute__((visibility("default"))) extern const char *

 #endif // _WIN32

	HLLAPI_API_CALL hllapi(const LPWORD func, LPSTR str, LPWORD length, LPWORD rc);

	HLLAPI_API_CALL hllapi_init(LPSTR mode);
	HLLAPI_API_CALL hllapi_deinit(void);
	HLLAPI_API_CALL hllapi_get_revision(void);

	HLLAPI_API_CALL hllapi_reset(void);

	HLLAPI_API_CALL hllapi_connect(const LPSTR uri, WORD wait);
	HLLAPI_API_CALL hllapi_disconnect(void);

	HLLAPI_API_CALL hllapi_get_message_id(void);
	HLLAPI_API_CALL hllapi_is_connected(void);
	HLLAPI_API_CALL hllapi_get_state(void);
	HLLAPI_API_CALL hllapi_get_screen_at(WORD row, WORD col, LPSTR buffer);
	HLLAPI_API_CALL hllapi_get_screen(WORD pos, LPSTR buffer, WORD len);
	HLLAPI_API_CALL hllapi_enter(void);
	HLLAPI_API_CALL hllapi_set_text_at(WORD row, WORD col, LPSTR text);
	HLLAPI_API_CALL hllapi_cmp_text_at(WORD row, WORD col, LPSTR text);
	HLLAPI_API_CALL hllapi_find_text(LPSTR text);
    HLLAPI_API_CALL hllapi_emulate_input(const LPSTR buffer, WORD len, WORD pasting);
    HLLAPI_API_CALL hllapi_input_string(LPSTR buffer, WORD len);
	HLLAPI_API_CALL hllapi_wait_for_ready(WORD seconds);
	HLLAPI_API_CALL hllapi_wait_for_change(WORD seconds);
	HLLAPI_API_CALL hllapi_wait(WORD seconds);
	HLLAPI_API_CALL hllapi_pfkey(WORD key);
	HLLAPI_API_CALL hllapi_pakey(WORD key);

    HLLAPI_API_CALL hllapi_set_session_parameter(LPSTR param, WORD len, WORD value);

	HLLAPI_API_CALL hllapi_erase(void);
	HLLAPI_API_CALL hllapi_erase_eof(void);
	HLLAPI_API_CALL hllapi_erase_eol(void);
	HLLAPI_API_CALL hllapi_erase_input(void);

	HLLAPI_API_CALL hllapi_action(LPSTR buffer);

	HLLAPI_API_CALL hllapi_print(void);

	HLLAPI_API_CALL hllapi_get_datadir(LPSTR datadir);
	HLLAPI_API_CALL hllapi_set_charset(LPSTR datadir);

	HLLAPI_API_CSTR hllapi_get_last_error();

 #ifdef __cplusplus
 }    /* end of extern "C" */
 #endif


