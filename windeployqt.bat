@echo off
set appDir=cmake_vs2019_v142\Release\
set app=ProjectorCommander.exe
set windeployqt=\bin\windeployqt.exe

cd %appDir%
%QTDIR%%windeployqt% %app%

pause