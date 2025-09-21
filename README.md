## HLLAPI library for lib3270/pw3270

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![CodeQL Advanced](https://github.com/PerryWerneck/libhllapi/actions/workflows/codeql.yml/badge.svg)](https://github.com/PerryWerneck/libhllapi/actions/workflows/codeql.yml)
[![Publish](https://github.com/PerryWerneck/libhllapi/actions/workflows/publish.yml/badge.svg)](https://github.com/PerryWerneck/libhllapi/actions/workflows/publish.yml)
[![build result](https://build.opensuse.org/projects/home:PerryWerneck:pw3270/packages/libhllapi/badge.svg?type=percent)](https://build.opensuse.org/package/show/home:PerryWerneck:pw3270/libhllapi)

## Installation

### Linux

You can download installation package for supported distributions in Open Build Service.

[<img src="https://raw.githubusercontent.com/PerryWerneck/pw3270/master/branding/obs-badge-en.svg" alt="Download from open build service" height="80px">](https://software.opensuse.org/download.html?project=home%3APerryWerneck%3Apw3270&package=libhllapi)

### Windows

TODO

## Building for Linux

1. Build and install [libipc3270](../../../libipc3270)


2. Get hllapi sources from git

	```
	$ git clone https://github.com/PerryWerneck/libhllapi.git
	```

3. Setup, build and install

	```shell
	meson setup .build
	meson compile -C .build
	meson install -C .build
	```

## Building for Windows

### Cross-compiling on SuSE Linux (Native or WSL)

1. Add the MinGW Repositories

	```shell
	$ sudo zypper ar obs://windows:mingw:win32 mingw32
	$ sudo zypper ar obs://windows:mingw:win64 mingw64
	$ sudo zypper ar obs://home:PerryWerneck:pw3270 pw3270
	$ sudo zypper ref
	```

2. Install cross compilers

	```shell
	zypper in \
			pkgconfig \
			gettext-devel \
			mingw64-libcurl-devel \
			mingw64-cross-meson \
			mingw64-libopenssl-devel \
			mingw64-cross-gcc-c++
	```


3. Get hllapi sources from git

	```
	$ git clone https://github.com/PerryWerneck/libhllapi.git
	```

4. Configure and build

	```shell
	meson setup --cross-file /usr/lib/rpm/macros.d/meson-mingw64-cross-file.txt .build
	meson compile -C .build
	```

### Windows native with MSYS2

1. Install and update MSYS2 

	* Download and install [msys2](https://www.msys2.org/)
	* Update msys:
	
	```shell
	pacman -Syu
	```
	Afther this close and reopen mingw shell.

2. Update system path

	* Add c:\msys64\usr\bin and c:\msys64\mingw64\bin to system path

3. Install devel packages using pacman on mingw shell

	```shell
	pacman -S \
		dos2unix \
		mingw-w64-x86_64-gcc \
		mingw-w64-x86_64-meson \
		mingw-w64-x86_64-iconv \
		pkgconf \
		mingw-w64-x86_64-gettext \
		gettext-devel \
		mingw-w64-x86_64-openssl
	```

	Afther this close and reopen mingw shell.

4. Get hllapi sources from git

	```
	$ git clone https://github.com/PerryWerneck/libhllapi.git
	```

5. Build with packman

	```shell
	makepkg BUILDDIR=/tmp/pkg -p PKGBUILD.mingw
	```

### Windows native with MSVC

1. Download and install Visual Studio Build Tools (https://visualstudio.microsoft.com/pt-br/downloads/)

2. Download and install git for windows

3. Download and install meson for windows

4. Get libhllapi sources from git

	```shell
	git clone https://github.com/PerryWerneck/libhllapi.git ./libhllapi
	```

[TODO]
