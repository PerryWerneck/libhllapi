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

#ifndef PRIVATE_H_INCLUDED

	#define PRIVATE_H_INCLUDED 1

	#include <config.h>
	#include <exception>
	#include <stdexcept>
	#include <string>
	#include <cerrno>
	#include <cstring>
	#include <iostream>
	#include <system_error>
	#include <lib3270/ipc.h>
	#include <lib3270/log.h>
	#include <lib3270/hllapi.h>

	#ifdef HAVE_LIBINTL
		#include <libintl.h>
		#define _( x ) dgettext(PACKAGE_NAME, x)
		#define N_( x ) x
	#else
		#define _( x ) x
		#define N_( x ) x
	#endif // HAVE_LIBINTL

	//
	// Compiler-specific #defines.
	//
	// Reference: GLIBC gmacros.h
	//
	#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)

			#define GNUC_UNUSED \
					__attribute__((__unused__))

	#else

			#define GNUC_UNUSED

	#endif


	using std::runtime_error;
	using std::string;
	using TN3270::Host;
	using std::exception;
	using std::clog;
	using std::endl;

	extern TN3270_PRIVATE std::string hllapi_lasterror;

	TN3270_PRIVATE TN3270::Host & getSession();

	TN3270_PRIVATE DWORD hllapi_translate_error(LIB3270_KEYBOARD_LOCK_STATE state);
	TN3270_PRIVATE DWORD hllapi_translate_error(LIB3270_MESSAGE state);
	TN3270_PRIVATE DWORD hllapi_translate_error(const std::system_error &error);

#endif // PRIVATE_H_INCLUDED
