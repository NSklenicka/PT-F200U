@echo off
set appDir=cmake_vs2019_v142\Release\
set app=ProjectorCommander.exe
set windeployqt=\bin\windeployqt.exe

cd %appDir%
%QT_DIR%%windeployqt% %app%

cd ..
cd ..
set appDir=cmake_vs2019_v142\Debug\
cd %appDir%
%QT_DIR%%windeployqt% %app%

cd ..
cd ..
set appDir=bin\
cd %appDir%
%QT_DIR%%windeployqt% %app%

pause