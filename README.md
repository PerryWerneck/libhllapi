## HLLAPI library for lib3270/pw3270

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![CodeQL](https://github.com/PerryWerneck/libhllapi/workflows/CodeQL/badge.svg?branch=master)
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

3. Build and install

	```
	$ cd libhllapi
	$ ./autogen.sh
	$ make all
	$ sudo make install
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

2. Get hllapi sources from git

	```
	$ git clone https://github.com/PerryWerneck/libhllapi.git
	```

3. Install 64 bits cross compilers

	```
	$ ./libhllapi/win/install-cross.sh --64
	```

4. Run the build script

	```shell
	$ cd libhllapi
	$ ./win/pack.sh
	```

### Windows native with MSYS2

TODO

### Windows native with MSVC

1. Download and install Visual Studio Build Tools (https://visualstudio.microsoft.com/pt-br/downloads/)

2. Download and install git for windows

3. Get libhllapi sources from git

	```shell
	git clone https://github.com/PerryWerneck/libhllapi.git ./libhllapi
	```

4. Download latest build of the 'glue' library.

	```shell
	cd libhllapi
	wget https://github.com/PerryWerneck/libipc3270/releases/download/5.5/msvc-libipc3270.zip
	7z x msvc-libipc3270.zip -y -oipc3270
	```

5. Make 

	```shell
	nmake /f win\Makefile-ci.msc LIB3270_SDK_PATH=ipc3270 DESTDIR=ipc3270.msvc install
	```

