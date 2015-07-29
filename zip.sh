#!/bin/sh

CONSOLE_EXE=./cityofthieves/release/cityofthieves.exe
DESKTOP_EXE=./cityofthieves/release/CityOfThievesDesktop.exe

if [ ! -e $CONSOLE_EXE ]
then
  ./crosscompiletowindows.sh
fi

if [ ! -e $CONSOLE_EXE ]
then
  echo "ERROR: Cannot find "$CONSOLE_EXE
  exit
fi

if [ ! -e $DESKTOP_EXE ]
then
  ./crosscompiletowindows.sh
fi

if [ ! -e $DESKTOP_EXE ]
then
  echo "ERROR: Cannot find "$DESKTOP_EXE
  exit
fi

mkdir CityOfThieves

cp $CONSOLE_EXE ./CityOfThieves/CityOfThievesConsole.exe
cp $DESKTOP_EXE ./CityOfThieves/CityOfThievesDesktop.exe

zip -9 -r GameCityOfThieves.zip CityOfThieves

rm -rf CityOfThieves