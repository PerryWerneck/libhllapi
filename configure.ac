dnl
dnl Software pw3270, desenvolvido com base nos códigos fontes do  C3270  e  X3270
dnl (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
dnl aplicativos mainframe. Registro no INPI sob o nome G3270.
dnl
dnl Copyright (C) <2008> <Banco do Brasil S.A.>
dnl
dnl Este programa é software livre. Você pode redistribuí-lo e/ou modificá-lo sob
dnl os termos da GPL v.2 - Licença Pública Geral  GNU,  conforme  publicado  pela
dnl Free Software Foundation.
dnl
dnl Este programa é distribuído na expectativa de  ser  útil,  mas  SEM  QUALQUER
dnl GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou  de  ADEQUAÇÃO
dnl A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença Pública Geral GNU para
dnl obter mais detalhes.
dnl
dnl Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
dnl programa;  se  não, escreva para a Free Software Foundation, Inc., 59 Temple
dnl Place, Suite 330, Boston, MA, 02111-1307, USA
dnl
dnl Contatos:
dnl
dnl perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
dnl erico.mendonca@gmail.com	(Erico Mascarenhas de Mendonça)
dnl

dnl Process this file with autoconf to produce a configure script.

dnl The minimum version of autoconf required.
AC_PREREQ(2.61)

dnl Initialise automake with the package name, version and
dnl bug-reporting address.
AC_INIT([hllapi], [5.2], [perry.werneck@gmail.com])

dnl Place auxilliary scripts here.
AC_CONFIG_AUX_DIR([scripts])

dnl Compute the canonical host-system type
AC_CANONICAL_HOST

dnl Put macro definitions here (though they aren't used).
AC_CONFIG_HEADER([src/include/config.h])

dnl Initialise automake stuff.
AM_INIT_AUTOMAKE

dnl Check for iconv
AM_ICONV

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_SED
AC_PROG_LN_S

AC_LANG([C++])

dnl ---------------------------------------------------------------------------
dnl Initialize defaults
dnl ---------------------------------------------------------------------------

DBG_CFLAGS="-g -fstack-check -DDEBUG=1"
RLS_CFLAGS="-DNDEBUG=1"
PLUGINS=""
APP_RESOURCES=""
APP_LDFLAGS=""
DLL_LDFLAGS="-shared -Wl,-soname,\$(@F)"
DEPENDS=""

dnl ---------------------------------------------------------------------------
dnl Check for OS specifics
dnl ---------------------------------------------------------------------------

case "$host" in
	*-mingw32|*-pc-msys)
		app_cv_osname="windows"

		CFLAGS="$CFLAGS -pthread -D_WIN32_WINNT=0x0600"
		LIBS="$LIBS -lws2_32 -lwtsapi32 -lcomdlg32"
		DLLEXT=".dll"

		app_win32_revision=$(date +%-y.%-m.%-d.%-H)
		AC_SUBST(WIN32_VERSION,$app_win32_revision)

		app_win32_file_version=$(date +%-y,%-m,%-d,%-H)
		AC_SUBST(WIN32_FILE_VERSION,$app_win32_file_version)

		AC_CONFIG_FILES(src/core/windows/resources.rc)
		AC_CONFIG_FILES(win/hllapi.nsi)

		if test "$host_cpu" = "x86_64"; then
			app_cv_programfiles="PROGRAMFILES64"
			app_cv_winarch="64"
		else
			app_cv_programfiles="PROGRAMFILES"
			app_cv_winarch="32"
		fi

		AC_SUBST(PROGRAMFILES,$app_cv_programfiles)
		AC_SUBST(WINARCH,$app_cv_winarch)
		;;

 	*)
		CFLAGS="$CFLAGS -pthread"
		LDFLAGS="$LDFLAGS -pthread"
		app_cv_datadir="/usr/share"
		app_cv_confdir="/etc"
		app_cv_osname="linux"
		LOGDIR="/var/log"
		DLLEXT=".so"

esac

AC_SUBST(DATADIR,$app_cv_datadir)
AC_SUBST(CONFDIR,$app_cv_confdir)
AC_SUBST(OSNAME,$app_cv_osname)
AC_SUBST(LIBS)
AC_SUBST(LOGDIR)
AC_SUBST(DLLEXT)
AC_SUBST(DLLPREFIX)
AC_SUBST(LDFLAGS)
AC_SUBST(DLL_LDFLAGS)

dnl ---------------------------------------------------------------------------
dnl Check for other programs
dnl ---------------------------------------------------------------------------

AC_PATH_TOOL([AR], [ar], [ar])
AC_PATH_TOOL([WINDRES], [windres], [no])
AC_PATH_TOOL([VALGRIND], [valgrind], [no])

PKG_CHECK_EXISTS

dnl ---------------------------------------------------------------------------
dnl Version info
dnl ---------------------------------------------------------------------------

AC_SUBST(PACKAGE_DESCRIPTION,"PW3270 HLLAPI Module")

app_vrs_major=$(echo $VERSION | cut -d. -f1)
app_vrs_minor=$(echo $VERSION | cut -d. -f2)

AC_DEFINE_UNQUOTED(PACKAGE_MAJOR_VERSION, $app_vrs_major)
AC_DEFINE_UNQUOTED(PACKAGE_MINOR_VERSION, $app_vrs_minor)

AC_SUBST(PACKAGE_MAJOR_VERSION,$app_vrs_major)
AC_SUBST(PACKAGE_MINOR_VERSION,$app_vrs_minor)

AC_ARG_WITH([release], [AS_HELP_STRING([--with-release], [Set release])], [ app_cv_release="$withval" ],[ app_cv_release="`date +%-y`.`date +%-m`.`date +%-d`" ])

app_rls_major=$(echo $app_cv_release.0.0 | cut -d. -f1)
app_rls_minor=$(echo $app_cv_release.0.0 | cut -d. -f2)

AC_DEFINE_UNQUOTED(PACKAGE_RELEASE, $app_cv_release)
AC_DEFINE_UNQUOTED(PACKAGE_MAJOR_RELEASE, $app_rls_major)
AC_DEFINE_UNQUOTED(PACKAGE_MINOR_RELEASE, $app_rls_minor)

AC_SUBST(PACKAGE_RELEASE,$app_cv_release)
AC_SUBST(PACKAGE_MAJOR_RELEASE,$app_rls_major)
AC_SUBST(PACKAGE_MINOR_RELEASE,$app_rls_minor)

case "$host" in
        *-mingw32|*-pc-msys)
                AC_SUBST(SONAME,hllapi.dll)
                ;;

        s390x-*)
                AC_SUBST(SONAME,libhllapi.so)
                ;;

        *)
                AC_SUBST(SONAME,libhllapi.so.$app_vrs_major.$app_vrs_minor)

esac

dnl ---------------------------------------------------------------------------
dnl Check for doxygen
dnl ---------------------------------------------------------------------------

AC_PATH_TOOL([DOXYGEN], [doxygen])

dnl Help line for graphviz (optionally used by doxygen)
AC_ARG_ENABLE(graphviz,
	AS_HELP_STRING([--disable-graphviz], [prevent doxygen from using dot]),
	[wantgraphviz=$enableval], [wantgraphviz=yes])

AC_CHECK_PROGS([GRAPHVIZDOT], [dot])
AC_MSG_CHECKING([whether doxygen should use graphviz])
if test "x$wantgraphviz" = "xyes" ; then
	if test $GRAPHVIZDOT ; then
		AC_MSG_RESULT([yes])
		usegraphviz=yes
	else
		AC_MSG_RESULT([no, graphviz missing])
		usegraphviz=no
	fi
else
	AC_MSG_RESULT([no, disabled by configure])
	usegraphviz=no
fi

AC_SUBST(HAVE_DOT,`echo $usegraphviz | tr yesno YESNO`)

if test "x$DOXYGEN" != "x"; then
	AC_CONFIG_FILES(doxygen/doxyfile)
fi

dnl ---------------------------------------------------------------------------
dnl Check for C++ 2011 support
dnl ---------------------------------------------------------------------------
AC_DEFUN([AX_CHECK_COMPILE_FLAG],
[AC_PREREQ(2.59)dnl for _AC_LANG_PREFIX
AS_VAR_PUSHDEF([CACHEVAR],[ax_cv_check_[]_AC_LANG_ABBREV[]flags_$4_$1])dnl
AC_CACHE_CHECK([whether _AC_LANG compiler accepts $1], CACHEVAR, [
  ax_check_save_flags=$[]_AC_LANG_PREFIX[]FLAGS
  _AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS $4 $1"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM()],
    [AS_VAR_SET(CACHEVAR,[yes])],
    [AS_VAR_SET(CACHEVAR,[no])])
  _AC_LANG_PREFIX[]FLAGS=$ax_check_save_flags])
AS_IF([test x"AS_VAR_GET(CACHEVAR)" = xyes],
  [m4_default([$2], :)],
  [m4_default([$3], :)])
AS_VAR_POPDEF([CACHEVAR])dnl
])dnl AX_CHECK_COMPILE_FLAGS

AX_CHECK_COMPILE_FLAG([-std=gnu++11], [CFLAGS="$CFLAGS -std=gnu++11"])

dnl ---------------------------------------------------------------------------
dnl Check for libintl
dnl ---------------------------------------------------------------------------

INTL_LIBS=""

AC_PATH_TOOL([XGETTEXT], [xgettext], [no])
AC_PATH_TOOL([MSGCAT], [msgcat], [no])
AC_PATH_TOOL([MSGINIT], [msginit], [no])
AC_PATH_TOOL([MSGMERGE], [msgmerge], [no])
AC_PATH_TOOL([MSGFMT], [msgfmt], [no])

AC_SUBST(INTL_LIBS)

dnl ---------------------------------------------------------------------------
dnl Check for IPC3270
dnl ---------------------------------------------------------------------------

AC_ARG_WITH([static-ipc3270],
        [AS_HELP_STRING([--with-static-ipc3270], [Uses static version of ipc3270])],
[
	app_cv_static_ipc3270=$withval
],[
	app_cv_static_ipc3270="auto"
])

if test "$app_cv_static_ipc3270" == "auto"; then
	case "$host" in
	*-mingw32|*-pc-msys)
		app_cv_static_ipc3270="yes"
		;;

	*)
		app_cv_static_ipc3270="no"

	esac

fi

if test "$app_cv_static_ipc3270" == "yes"; then
	LDFLAGS="-static -static-libgcc -static-libstdc++ $LDFLAGS"
	PKG_CHECK_MODULES( [IPC3270], [ipc3270-static], AC_DEFINE(USING_STATIC_IPC3270), AC_MSG_ERROR([IPC3270 not present.]))
else
	PKG_CHECK_MODULES( [IPC3270], [ipc3270], AC_DEFINE(USING_SHARED_IPC3270), AC_MSG_ERROR([IPC3270 not present.]))
fi

AC_SUBST(IPC3270_LIBS)
AC_SUBST(IPC3270_CFLAGS)

dnl ---------------------------------------------------------------------------
dnl Check for pic
dnl ---------------------------------------------------------------------------

AC_ARG_ENABLE([pic],
	[AS_HELP_STRING([--disable-pic], [disable support for PIC libraries (required to compile on 64bits)])],
[
	app_cv_pic="$enableval"
],[

	case "$host_cpu" in
	x86_64)
		app_cv_pic="yes"
		;;

	aarch64)
		app_cv_pic="yes"
		;;

	*)
		app_cv_pic="no"
	esac

])

if test "$app_cv_pic" == "yes"; then
	CFLAGS="$CFLAGS -fPIC"
	CXXFLAGS="$CXXFLAGS -fPIC"
fi

dnl ---------------------------------------------------------------------------
dnl Configure which files to generate.
dnl ---------------------------------------------------------------------------

AC_SUBST(DEPENDS)
AC_CONFIG_FILES(Makefile)

dnl ---------------------------------------------------------------------------
dnl Output the generated config.status script.
dnl ---------------------------------------------------------------------------
AC_SUBST(BASEDIR,$ac_pwd)

AC_OUTPUT


