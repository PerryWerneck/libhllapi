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
 * Este programa está nomeado como testprogram.cc e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

 #include <iostream>
 #include <cstring>
 #include <cstdio>
 #include <string>
 #include <ctime>
 #include <lib3270/hllapi.h>

 #if defined(_MSC_VER)
	#pragma comment(lib,"hllapi.lib")
	#define strncasecmp _strnicmp
	#define strcasecmp _stricmp
 #else
 	#include <getopt.h>
 #endif

 #define SCREEN_LENGTH 2000

 using namespace std;

/*---[ Implement ]--------------------------------------------------------------------------------*/

 int main(int argc, char **argv) {

	int rc;

	char buffer[SCREEN_LENGTH+1];

 	const char *host	= "";
 	const char *session = ":a";

 #if ! defined(_MSC_VER)

	#pragma GCC diagnostic push
	static struct option options[] =
	{
		{ "host",			required_argument,	0,	'h' },
		{ "session-name",	required_argument,	0,	'S' },
		{ 0, 0, 0, 0}
	};
	#pragma GCC diagnostic pop

	int long_index =0;
	int opt;
	while((opt = getopt_long(argc, argv, "h:S:", options, &long_index )) != -1)
	{

		switch(opt)
		{
		case 'h':
			host = optarg;
			break;

		case 'S':
			session = optarg;
			break;

		}

	}

 #endif // ! defined(_MSC_VER)

	try {

		rc = hllapi_init((char *) session);
		if(rc) {
			cout << "hllapi_init returns with rc=" << rc << " (" << hllapi_get_last_error_as_cstring() << ")" << endl;
			return rc;
		}

		cout << "TN3270 library revision is " << ((int) hllapi_get_revision()) << endl << ">";

		hllapi_set_timeout(10);

	} catch(const std::exception &e) {

		cerr << e.what() << endl;
		return -1;

	} catch(...) {

		cerr << "Unexpected error" << endl;
		return -1;
	}

	cout.flush();

 	string cmdline;
	while(getline(cin, cmdline)) {

		if(cmdline.empty())
			break;

		memset(buffer,' ',SCREEN_LENGTH);
		buffer[SCREEN_LENGTH] = 0;

		rc = HLLAPI_STATUS_SUCCESS;

		if(strcasecmp(cmdline.c_str(),"connect") == 0) {

			cout << "Connecting..." << endl;
			rc = hllapi_connect((LPSTR) host,1);

		} else if(strcasecmp(cmdline.c_str(),"wait") == 0) {

			rc = hllapi_wait_for_ready(10);
			cout << "hllapi_wait_for_ready returns with rc=" << rc << " (" << hllapi_get_last_error_as_cstring() << ")" << endl;

		} else if(strcasecmp(cmdline.c_str(),"disconnect") == 0) {

			cout << "Disconnecting..." << endl;
			rc = hllapi_disconnect();

		} else if(strcasecmp(cmdline.c_str(),"luname") == 0) {

			rc = hllapi_get_lu_name(buffer,SCREEN_LENGTH);
			if(rc == HLLAPI_STATUS_SUCCESS) {
				cout << "LU Name is " << buffer << endl;
			}

		} else if(strcasecmp(cmdline.c_str(),"contents") == 0) {

			rc = hllapi_get_screen(0,buffer,SCREEN_LENGTH);
			cout << "hllapi_get_screen returns with rc=" << rc << " (" << hllapi_get_last_error_as_cstring() << ")" << endl
					<< buffer << endl << endl;

		} else if(strncasecmp(cmdline.c_str(),"cursor",6) == 0) {

			unsigned int row, col;
			const char * arg = cmdline.c_str()+6;

			while(*arg && isspace(*arg))
				arg++;

			if(*arg) {

				switch(std::sscanf(arg,"%u,%u",&row,&col)) {
				case 1:
					cout << "Moving cursor to " << row << endl;
					rc = hllapi_set_cursor_address((WORD) row);
					break;

				case 2:
					cout << "Moving cursor to " << row << "," << col << endl;
					rc = hllapi_set_cursor_position((WORD) row, (WORD) col);
					break;
				}

			} else {

				int addr = hllapi_get_cursor_address();

				cout << "Cursor address=" << addr << endl;

				WORD row, col;
				rc = hllapi_get_cursor_position(&row,&col);

				cout << "Cursor position=" << row << "," << col << endl;
			}


		} else if(strncasecmp(cmdline.c_str(),"at",2) == 0) {

			unsigned int row, col, len;

			switch(std::sscanf(cmdline.c_str()+3,"%u,%u,%u",&row,&col,&len)) {
			case 3:
				{
					cout << "Getting " << len << " bytes from " << row << "," << col << endl;

					char * buffer = new char[len+1];
					memset(buffer,' ',len+1);
					buffer[len] = 0;

					rc = hllapi_get_screen_at(row,col,buffer);
					if(rc == HLLAPI_STATUS_SUCCESS) {
						cout << "Contents:" << endl << buffer << endl;
					}

					delete[] buffer;
				}
				break;

			case 2:
				{
					cout << "Getting " << col << " bytes from " << row << endl;

					char * buffer = new char[col+1];
					memset(buffer,' ',col+1);
					buffer[col] = 0;

					rc = hllapi_get_screen(row,buffer,col);
					if(rc == HLLAPI_STATUS_SUCCESS) {
						cout << "Contents:" << endl << buffer << endl;
					}

					delete[] buffer;

				}
				break;
			}

		} else if(strncasecmp(cmdline.c_str(),"enter",5) == 0) {

			rc = hllapi_enter();

		} else if(strncasecmp(cmdline.c_str(),"pf ",3) == 0) {

			rc = hllapi_pfkey(atoi(cmdline.c_str()+3));

		} else if(strncasecmp(cmdline.c_str(),"input ",6) == 0) {

			rc = hllapi_input_string((LPSTR) cmdline.c_str()+6,0);

		} else if(strncasecmp(cmdline.c_str(),"wait ",5) == 0) {

			rc = hllapi_wait_for_ready(5);

		} else if(strncasecmp(cmdline.c_str(),"find ",5) == 0) {

			cout << "Searching for \"" << (cmdline.c_str()+5) << "\"" << endl;
			rc = hllapi_find_text((LPSTR) (cmdline.c_str()+5));

		} else if(strcasecmp(cmdline.c_str(),"perftest") == 0) {

			if(!hllapi_is_connected()) {

				cout << "Connect to host first" << endl;

			} else {

				// Test "hllapi_wait_for_ready
				cout << "Testing hllapi_wait_for_ready" << endl;
				{
					time_t start_time = time(0);
					for(size_t ix = 0; ix < 100; ix++) {

						hllapi_wait_for_ready(10);

					}
					cout << "Time for 100 iterations of hllapi_wait_for_ready was " << (time(0) - start_time) << " seconds" << endl;

				}

				// Teste hllapi_get_string_at
				{
					cout << "Testing hllapi_get_screen_at" << endl;
					char buffer[81];
					time_t start_time = time(0);
					for(size_t ix = 0; ix < 100; ix++) {

						memset(buffer,' ',sizeof(buffer));
						buffer[80] = 0;
						hllapi_get_screen_at(14,1,buffer);

					}
					cout << "Time for 100 iterations of hllapi_get_screen_at was " << (time(0) - start_time) << " seconds" << endl;

				}

			}

		} else {

			cout << "Unknown command \"" << cmdline << "\""<< endl;
		}

		if(rc != HLLAPI_STATUS_SUCCESS) {
			cout << "rc=" << rc << " (" << hllapi_get_last_error_as_cstring() << ")" << endl;
			rc = HLLAPI_STATUS_SUCCESS;
		}

		cout << endl << ">";
		cout.flush();
	}

 	rc = hllapi_deinit();
	cout << "hllapi_deinit exits with rc=" << rc << endl;

	return 0;
 }

	/*


	rc = hllapi_wait_for_ready(10);
 	if(rc) {
		cout << "hllapi_wait_for_ready returns with rc=" << rc << " (" << hllapi_get_last_error_as_cstring() << ")" << endl;
		return rc;
	}

	if(hllapi_is_connected()) {

		cout << "Host is connected" << endl;

		memset(buffer,' ',SCREEN_LENGTH);
		buffer[SCREEN_LENGTH] = 0;

		rc = hllapi_get_screen(0,buffer,SCREEN_LENGTH);
		cout << "hllapi_get_screen returns with rc=" << rc << " (" << hllapi_get_last_error_as_cstring() << ")" << endl;

		if(rc == HLLAPI_STATUS_SUCCESS) {
			cout << endl << buffer << endl;
		}

	} else {

		cout << "Host is not connected" << endl;


	}

	rc = hllapi_get_lu_name(buffer,SCREEN_LENGTH);
	if(rc == HLLAPI_STATUS_SUCCESS) {
		cout << "LU Name is " << buffer << endl;
	}

	rc = hllapi_disconnect();
 	if(rc) {
		cout << "hllapi_disconnect returns with rc=" << rc << " (" << hllapi_get_last_error_as_cstring() << ")" << endl;
		return rc;
	}

	cout << "HLLAPI Last error was \"" << hllapi_get_last_error_as_cstring() << "\"" << endl;

	*/


