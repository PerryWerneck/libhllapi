#!/bin/bash
PROJECT_DIR=$(readlink -f $(dirname $(dirname $(readlink -f $0))))

PUBLISH=0
CLEAR_OLD=0

pack() {

	echo -e "\e]2;hllapi-${1}\a"

	cd ${PROJECT_DIR}

	BUILDDIR=$(mktemp -d)

	./configure \
		--cache=.${1}.cache \
		--host=${1}-w64-mingw32 \
		--prefix=/usr/${1}-w64-mingw32/sys-root/mingw \
                --bindir=${BUILDDIR} \
                --libdir=${BUILDDIR} \
                --localedir=${BUILDDIR} \
                --includedir=${BUILDDIR} \
                --sysconfdir=${BUILDDIR} \
                --datadir=${BUILDDIR} \
                --datarootdir=${BUILDDIR} \
		--with-application-datadir=${BUILDDIR}


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

	make install
	if [ "$?" != "0" ]; then
		exit -1
	fi

	NSI=$(readlink -f win/hllapi.nsi)

	cp LICENSE ${BUILDDIR}

	cd ${BUILDDIR}

	makensis -NOCD ${NSI}
	if [ "$?" != "0" ]; then
		exit -1
	fi

	PRODUCT_NAME=$(${1}-w64-mingw32-pkg-config --variable=product_name lib3270)

	if [ ! -z ${PRODUCT_NAME} ]; then

		if [ -d ~/public_html/win/${PRODUCT_NAME}/${2} ]; then

			if [ "${CLEAR_OLD}" == "1" ]; then
				rm -fr ~/public_html/win/${PRODUCT_NAME}/${2}/hllapi-*.exe
			fi

			cp -v *.exe ~/public_html/win/${PRODUCT_NAME}/${2}

		fi

		if [ "${PUBLISH}" == "1" ] && [ ! -z ${WIN_PACKAGE_SERVER} ]; then
			echo scp *.exe ${WIN_PACKAGE_SERVER}/${PRODUCT_NAME}/${2}
			if [ "$?" != "0" ]; then
				exit -1
			fi
		fi

	fi

	mv -f *.exe ${PROJECT_DIR}
	if [ "$?" != "0" ]; then
		exit -1
	fi


	cd ${PROJECT_DIR}
	rm -fr ${BUILDDIR}
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
			CLEAR_OLD=1
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

pack i686 x86_32
pack x86_64 x86_64


