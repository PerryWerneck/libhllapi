
## HLLAPI library for lib3270/pw3270

## Installation

### Linux

For the supported distributions get the install repositories and instructions from https://software.opensuse.org/download.html?project=home%3APerryWerneck%3Apw3270&package=libhllapi

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
	$ make install
	```

