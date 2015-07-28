#!/bin/sh

rm CityOfThieves.zip

mkdir CityOfThieves

# Go to folder /HetRoversnest/QtCreator/CityOfThieves
cd CityOfThieves

# Create folder /HetRoversnest/QtCreator/CityOfThieves/Exe
mkdir Exes

cp -r ../../Files .

cp ../release/HetRoversnestConsole.exe ./Exes/CityOfThievesConsole.exe
cp ../release/HetRoversnestDesktop.exe ./Exes/CityOfThievesDesktop.exe

# Go to folder /HetRoversnest/QtCreator/CityOfThieves
cd ..

zip -9 -r CityOfThieves.zip CityOfThieves

rm -rf CityOfThieves

mv CityOfThieves.zip ../GameCityOfThievesExe.zip
