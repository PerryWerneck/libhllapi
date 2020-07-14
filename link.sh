#!/bin/bash
make Debug 

sudo ln -sf $(readlink -f .bin/Debug/libhllapi.so.5.3) /usr/lib64/libhllapi.so.5.3
sudo ln -sf $(readlink -f .bin/Debug/libhllapi.so.5.3) /usr/lib64/libhllapi.so
sudo ln -sf $(readlink -f src/include/lib3270/hllapi.h) /usr/include/lib3270/hllapi.h

sudo ln -sf $(readlink -f sdk/*.pc) /usr/lib64/pkgconfig



