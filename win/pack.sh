#!/bin/bash
cd $(dirname $(dirname $(readlink -f $0)))

PUBLISH=0

pack() {

	echo -e "\e]2;hllapi-${1}\a"

	./configure --cache=.${1}.cache \
		--host=${1}-w64-mingw32 \
		--prefix=/usr/${1}-w64-mingw32/sys-root/mingw \
		--libdir=/usr/${1}-w64-mingw32/sys-root/mingw/lib

	if [ "$?" != "0" ]; then
		exit -1
	fi

	make clean
	if [ "$?" != "0" ]; then
		exit -1
	fi

	make Release
	if [ "$?" != "0" ]; then
		exit -1
	fi

	makensis win/hllapi.nsi

	if [ -d ~/public_html ]; then
		mkdir -p ~/public_html/win
		cp -v ./win/*.exe ~/public_html/win
		if [ "$?" != "0" ]; then
			exit -1
		fi
	fi

	if [ "${PUBLISH}" == "1" ] && [ ! -z ${WIN_PACKAGE_SERVER} ]; then
		scp ./win/*.exe ${WIN_PACKAGE_SERVER}
		if [ "$?" != "0" ]; then
			exit -1
		fi
	fi

	rm -fr $TEMPDIR
	rm -fr .${1}.cache

}

until [ -z "$1" ]
do
        if [ ${1:0:2} = '--' ]; then
                tmp=${1:2}
                parameter=${tmp%%=*}
                parameter=$(echo $parameter | tr "[:lower:]" "[:upper:]")
                value=${tmp##*=}

                case "$parameter" in
		NOPUBLISH)
			PUBLISH=0
			;;

		PUBLISH)
			PUBLISH=1
			;;

		CLEAR)
			if [ -d ~/public_html/win ]; then
				rm -fr ~/public_html/win/hllapi-*-{i686,x86_64}.exe
			fi

			;;
		HELP)
			echo "${0} nome_da_matriz"
			echo ""
			echo "Opções:"
			echo ""

			if [ ! -z ${WIN_PACKAGE_SERVER} ]; then
				echo "  --nopublish	Don't publish in ${WIN_PACKAGE_SERVER}"
				echo "  --publish	Publish in ${WIN_PACKAGE_SERVER}"
			fi


			if [ -d ~/public_html/win ]; then
				echo "  --clear	Remove ~/public_html/win/hllapi-*-{i686,x86_64}.exe"
			fi

			echo ""
			exit 0
			
			;;
		
		esac
	fi

	shift

done

NOCONFIGURE=1 ./autogen.sh
if [ "$?" != "0" ]; then
	exit -1
fi

pack i686
pack x86_64


