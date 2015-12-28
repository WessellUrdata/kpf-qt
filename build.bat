@echo off
title KPF Build Tool
color 0A

rem build env. applications
rem please change according to your setup
set CWD=%~dp0
set QT=C:\Qt\5.5\mingw492_32\bin
set MINGW=C:\MinGW\bin
set QMAKE=%QT%\qmake.exe
set MAKE=%MINGW%\mingw32-make.exe
set PATH=%MINGW%;%QT%;%PATH%

if exist bin\win32\KPF.exe goto clean

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

:build
set /p j="How many threads do you want running for compilation? "
echo.
echo building makefile
%QMAKE% kpf-qt.pro -o src\Makefile
echo.
echo compiling binary
%MAKE% -j%j% -C src
copy /Y src\release\KPF.exe "%CWD%\bin\win32\KPF.exe"
echo.
echo copyting required libraries
copy /Y %QT%\Qt5Core.dll bin\win32\Qt5Core.dll
copy /Y %QT%\Qt5Gui.dll bin\win32\Qt5Gui.dll
copy /Y %QT%\Qt5Widgets.dll bin\win32\Qt5Widgets.dll
copy /Y %QT%\icudt54.dll bin\win32\icudt53.dll
copy /Y %QT%\icuin54.dll bin\win32\icuin53.dll
copy /Y %QT%\icuuc54.dll bin\win32\icuuc53.dll
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
if exist icudt53.dll del icudt53.dll
if exist icuin53.dll del icuin53.dll
if exist libgcc_s_dw2-1.dll del libgcc_s_dw2-1.dll
if exist "libstdc++-6.dll" del "libstdc++-6.dll"
if exist "libwinpthread-1.dll" del "libwinpthread-1.dll"
if exist icuuc53.dll del icuuc53.dll
if exist platforms\qwindows.dll del platforms\qwindows.dll
if exist kse.ini del kse.
if exist logs rmdir /S /Q logs
cd "%CWD%\src"
%MAKE% clean
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
