#!/bin/sh
# Crosscompiles City Of Thieves to the Windows operating system

cd cityofthieves

i686-w64-mingw32.static-qmake-qt5 cityofthieves.pro
make

i686-w64-mingw32.static-qmake-qt5 CityOfThievesDesktop.pro
make

cd ..

CONSOLE_EXE=./cityofthieves/release/cityofthieves.exe
DESKTOP_EXE=./cityofthieves/release/CityOfThievesDesktop.exe

if [ ! -e $CONSOLE_EXE ]
then
  echo "ERROR: Cannot find "$CONSOLE_EXE
else
  echo "OK: Can find "$CONSOLE_EXE
fi


if [ ! -e $DESKTOP_EXE ]
then
  echo "ERROR: Cannot find "$DESKTOP_EXE
else
  echo "OK: Can find "$DESKTOP_EXE
fi
