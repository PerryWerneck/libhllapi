#!/bin/bash
LANG=C objdump -p .bin/Debug/hllapi.dll | grep "DLL Name:"
