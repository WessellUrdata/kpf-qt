@echo off
title KPF Build Tool

rem This build script is for MinGW, which is 32 bit
rem To build for 64 bit systems, please use `build.msvc.bat` instead

rem build env. applications
rem please change according to your setup
set CWD=%~dp0
set QT=C:\Qt\5.11.2\mingw53_32\bin
set PATH=%QT%;C:\Qt\Tools\mingw530_32\bin;%PATH%

rem Just in case you wanna run a certain
rem event manually, use one of the cmd args
rem -b force build
rem -c force clean
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

if exist bin\win32\KPF.exe goto clean

:build
set /p j="How many threads do you want running for compilation? "
echo.
echo building makefile
qmake kpf-qt.pro -o src\Makefile
echo.
echo compiling binary
mingw32-make -j%j% -C src
copy /Y src\release\KPF.exe "%CWD%\bin\win32\KPF.exe"
echo.
echo copyting required libraries
copy /Y %QT%\Qt5Core.dll bin\win32\Qt5Core.dll
copy /Y %QT%\Qt5Gui.dll bin\win32\Qt5Gui.dll
copy /Y %QT%\Qt5Widgets.dll bin\win32\Qt5Widgets.dll
copy /Y %QT%\libgcc_s_dw2-1.dll bin\win32\libgcc_s_dw2-1.dll
copy /Y "%QT%\libstdc++-6.dll" "bin\win32\libstdc++-6.dll"
copy /Y "%QT%\libwinpthread-1.dll" "bin\win32\libwinpthread-1.dll"
copy /Y %QT%\..\plugins\platforms\qwindows.dll bin\win32\platforms\qwindows.dll
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
if exist libgcc_s_dw2-1.dll del libgcc_s_dw2-1.dll
if exist "libstdc++-6.dll" del "libstdc++-6.dll"
if exist "libwinpthread-1.dll" del "libwinpthread-1.dll"
if exist icuuc54.dll del icuuc54.dll
if exist platforms\qwindows.dll del platforms\qwindows.dll
if exist kse.ini del kse.ini
if exist logs rmdir /S /Q logs
cd "%CWD%\src"
mingw32-make clean
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
