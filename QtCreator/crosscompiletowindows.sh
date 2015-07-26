#!/bin/sh

i686-w64-mingw32.static-qmake-qt5 HetRoversnestConsole.pro
make

i686-w64-mingw32.static-qmake-qt5 HetRoversnestDesktop.pro
make

echo "Done"
