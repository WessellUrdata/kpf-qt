@echo off
title KPF Build Tool
color 0A

rem build env. applications
rem please change according to your setup
set CWD=%~dp0
set QT=C:\Qt\5.6\msvc2013\bin
set VCPATH=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
set QMAKE=%QT%\qmake.exe
set VC="%VCPATH%\vcvarsall.bat"
rem set PATH=%QT%;%VC%;%PATH%


rem Make sure to get nmake!!!!!
call %VC% x86

rem Just in case you wanna run a certain
rem event manually, use one of the cmd args
rem -b force build
rem -c force clean
if "%1" NEQ "" (
	if "%1" EQU "-b" (
		goto build
	) else if "%1" EQU "-c" (
		goto clean
	) else (
		goto error
	)
)

if exist bin\win32\KPF.exe goto clean

:build
echo.
echo building makefile
%QMAKE% kpf-qt.pro -o src\Makefile
echo.
echo compiling binary
cd %CWD%\src
nmake -nologo -f Makefile.Release
cd %CWD%
copy /Y src\release\KPF.exe "%CWD%\bin\win32\KPF.exe"
echo.
echo copying required libraries
copy /Y %QT%\Qt5Core.dll "%CWD%\bin\win32\Qt5Core.dll"
copy /Y %QT%\Qt5Gui.dll "%CWD%\bin\win32\Qt5Gui.dll"
copy /Y %QT%\Qt5Widgets.dll "%CWD%\bin\win32\Qt5Widgets.dll"
copy /Y %QT%\..\plugins\platforms\qwindows.dll "%CWD%\bin\win32\platforms\qwindows.dll"
echo.
echo Build complete!
goto finish

:clean
echo.
echo Cleaning up binaries
cd "%CWD%\bin\win32"
if exist KPF.exe del KPF.exe
if exist Qt5Core.dll del Qt5Core.dll
if exist Qt5Gui.dll del Qt5Gui.dll
if exist Qt5Widgets.dll del Qt5Widgets.dll
if exist platforms\qwindows.dll del platforms\qwindows.dll
if exist kse.ini del kse.ini
if exist logs rmdir /S /Q logs
cd "%CWD%\src"
nmake -f Makefile.Release clean
if exist object_script.KPF.Debug del object_script.KPF.Debug
if exist object_script.KPF.Release del object_script.KPF.Release
if exist debug rmdir /S /Q debug
if exist release rmdir /S /Q release
if exist Makefile del Makefile
if exist Makefile.Debug del Makefile.Debug
if exist Makefile.Release del Makefile.Release
cd ..
echo.
echo Cleaning complete
goto finish

:error
echo.
echo Error: "%1" is an invalid command switch!
echo.

:finish
pause
