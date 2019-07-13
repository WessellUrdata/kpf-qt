@echo off
title KPF Build Tool

set CWD=%~dp0

:: Qt version to build against
set QT=5.13.0

:: Visual Studio Version (2017 or 2019)
set VS=2019

:: Change to 64 bit a 64 bit build
:: If statement allows defining ARCH
:: outside of this build script for
:: external use
if not defined ARCH (
    set ARCH=32
)

:: Sets architecture specific variables
if %ARCH% EQU 32 (
    set MSVC=msvc2017
    set VCA=x86
    set VCR=vcredist_msvc2017_x86.exe
) else if %ARCH% EQU 64 (
    set MSVC=msvc2017_64
    set VCA=amd64
    set VCR=vcredist_msvc2017_x64.exe
)

:: Setting system paths with needed toolchain locations
set PF=%PROGRAMFILES(X86)%
set CPATH=%PATH%
set PATH=C:\Qt\%QT%\%MSVC%\bin;C:\Qt\Tools\QtCreator\bin;^
%PF%\Microsoft Visual Studio\%VS%\Community\VC\Auxiliary\Build;%CPATH%;

:: Command flags
:: -b goes right to build
:: -c goes right to clean
:: any other goes to help
for %%A in (%*) do (
    if "%%A" EQU "-b" (
        goto build
    ) else if "%%A" EQU "-c" (
        goto clean
    ) else (
        goto help
    )
)

:build
echo.
:: Setup C Build Environment
call vcvarsall %VCA%
echo.
:: Gets max processor threads to compile on
set /p J="Would you like to use all CPU cores? [Y/n] "
if "%J%" EQU "Y" (
    set J=/J %NUMBER_OF_PROCESSORS%
    echo %NPROC%
) else if "%J%" EQU "y" (
    set J=/J %NUMBER_OF_PROCESSORS%
) else (
    set J=
)
echo.
echo Building makefile
:: mkdir build
qmake kpf-qt.pro -o staging\Makefile
cd staging
jom /F Makefile.release %J%
copy /Y "%CWD%\staging\release\KPF.exe" "%CWD%\bin\windows\KPF.exe"
copy /Y C:\Qt\vcredist\%VCR% "%CWD%\bin\windows\redist\%VCR%"
echo.
echo Copying required libraries
copy /Y C:\Qt\%QT%\%MSVC%\bin\Qt5Core.dll "%CWD%\bin\windows\Qt5Core.dll"
copy /Y C:\Qt\%QT%\%MSVC%\bin\Qt5Gui.dll "%CWD%\bin\windows\Qt5Gui.dll"
copy /Y C:\Qt\%QT%\%MSVC%\bin\Qt5Widgets.dll "%CWD%\bin\windows\Qt5Widgets.dll"
copy /Y C:\Qt\%QT%\%MSVC%\plugins\platforms\qwindows.dll "%CWD%\bin\windows\platforms\qwindows.dll"
echo.
cd %CWD%
echo KPF Build Complete!
goto end

:clean
echo.
echo Cleaning up binaries
cd %CWD%\bin\windows
if exist KPF.exe del KPF.exe
if exist Qt5Core.dll del Qt5Core.dll
if exist Qt5Gui.dll del Qt5Gui.dll
if exist Qt5Widgets.dll del Qt5Widgets.dll
if exist platforms\qwindows.dll del platforms\qwindows.dll
if exist redist\%VCR% del redist\%VCR%
if exist logs rmdir /S /Q logs
cd %CWD%
if exist staging rmdir /S /Q staging
echo KPF Cleaning complete
goto end

:help
echo.
echo Just in case you wanna run a certain
echo event manually, use one of the cmd args
echo     -b force build
echo     -c force clean

:end
:: Clean up created variables
set PATH=%CPATH%
set MSVC=
set QT=
set VS=
set J=
set NPROC=
set VCR=
set VCA=
set PF=
set ARCH=
if not defined SKIP_PAUSE (
    pause
)
