@echo off
title KPF Build Tool

rem This build script is for MSVC and only suppors 64 bit systems
rem To build for 32 bit systems, please use `build.bat` instead
rem and build with mingw32.

rem build env. applications
rem please change according to your setup
set CWD=%~dp0
set QT=C:\Qt\5.10.1\msvc2017_64\bin
set PATH=%PATH%;%QT%;^
%PROGRAMFILES(X86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build

rem Just in case you wanna run a certain
rem event manually, use one of the cmd args
rem -b force build
rem -c force clean
rem -h show help
for %%A in (%*) do (
	if "%%A"=="-b" (
		goto build
	) else if "%%A" equ "-c" (
		goto clean
	) else if "%%A" equ "-h" (
		goto help
	) else (
		goto error
	)
)

if exist bin\win64\KPF.exe goto clean

:build
call vcvars64
echo.
set /p J="Would you like to use all CPU cores? [Y/N] "
if "%J%" equ "Y" (
	set CL=/MP
) else if "%J%" equ "y" (
	set CL=/MP
)
echo.
echo building makefile
qmake kpf-qt.pro -o src\Makefile
echo.
echo compiling binary
cd %CWD%\src
nmake /NOLOGO /f Makefile.Release
cd %CWD%
copy /Y src\release\KPF.exe "%CWD%\bin\win64\KPF.exe"
echo.
echo copying required libraries
copy /Y %QT%\Qt5Core.dll "%CWD%\bin\win64\Qt5Core.dll"
copy /Y %QT%\Qt5Gui.dll "%CWD%\bin\win64\Qt5Gui.dll"
copy /Y %QT%\Qt5Widgets.dll "%CWD%\bin\win64\Qt5Widgets.dll"
copy /Y %QT%\..\plugins\platforms\qwindows.dll "%CWD%\bin\win64\platforms\qwindows.dll"
echo.
echo Build complete!
goto finish

:clean
call vcvars64
echo.
echo Cleaning up binaries
cd "%CWD%\bin\win64"
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
if exist .qmake.stash del .qmake.stash
cd ..
echo.
echo Cleaning complete
goto finish

:help
echo.
echo Just in case you wanna run a certain
echo event manually, use one of the cmd args
echo     -b force build
echo     -c force clean
echo     -h show help
goto :finish

:error
echo.
echo Error: "%1" is an invalid command switch!
echo.

:finish
