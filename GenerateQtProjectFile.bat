@ECHO OFF 
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat"  x86
SET QMAKE=%QTDIR%/bin/qmake.exe
%QMAKE% -project
ECHO Done.
PAUSE

