@ECHO OFF

SET CMAKEDIR=cmake_vs2019_v142

REM sometimes you just need new project files

IF EXIST %CMAKEDIR% (
	ECHO Clean project files?
	DEL %CMAKEDIR%
)

cmake.exe -S. -B %CMAKEDIR% -G "Visual Studio 16 2019" -A win32 -T v142

PAUSE
