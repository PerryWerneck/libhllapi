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
 #include <getopt.h>
 #include <lib3270/hllapi.h>

 using namespace std;

/*---[ Implement ]--------------------------------------------------------------------------------*/

 int main(int argc, char **argv) {

 	const char *host	= "";
 	const char *session = ""; // "pw3270:A";

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

 	int rc = hllapi_init((char *) session);
 	if(rc) {
		cout << "hllapi_init returns with rc=" << rc << " (" << hllapi_get_last_error() << ")" << endl;
		return rc;
	}

	cout << "TN3270 library revision is " << ((int) hllapi_get_revision()) << endl;

	rc = hllapi_connect(host,1);
 	if(rc) {
		cout << "hllapi_connect returns with rc=" << rc << " (" << hllapi_get_last_error() << ")" << endl;
		return rc;
	}

	cout << "HLLAPI Last error was \"" << hllapi_get_last_error() << "\"" << endl;

	/*
	printf("Library revision is %d\n",(int) hllapi_get_revision());

	if(host) {

		printf("connect=%d\n",(int) hllapi_connect((char *) host,1));
		printf("wait_for_ready=%d\n",(int) hllapi_wait_for_ready(10));
		printf("connected=%s\n",(int) hllapi_is_connected() ? "Yes" : "No");

		printf("disconnect=%d\n",(int) hllapi_disconnect());
	}

	printf("connected=%s\n",(int) hllapi_is_connected() ? "Yes" : "No");
	*/

	rc = hllapi_deinit();
	cout << "hllapi_deinit exits with rc=" << rc << endl;

	return 0;
 }

