@echo off
del testprogram.exe
nmake /f win\Makefile.msc testprogram.exe
testprogram.exe


