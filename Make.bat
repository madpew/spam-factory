:: Build Script for gbdk-n projects
:: IMPORTANT! SDCC needs to be in your path

@echo off
SETLOCAL EnableExtensions EnableDelayedExpansion

:: point this to your GBDK-n directory
set GBDK_DIR=C:\gbdk-n

:: variables
set RELs=
set INTERACTIVE=0
set TARGET=

:: get current directory name to use as output filename
for %%d in ("%CD%") do set TARGET=%%~nxd

:: make sure directories exist
if not exist obj mkdir obj
if not exist rom mkdir rom

:: check if this file is running in an interactive session
echo %CMDCMDLINE% | findstr /L /I %COMSPEC% >NUL 2>&1
if %errorlevel% == 0 set INTERACTIVE=1

:: compile all source files (*.c)
for /R %CD% %%f in (*.c) do (
	echo compiling %%~nf...
	sdcc -mgbz80 --no-std-crt0 -I "%GBDK_DIR%\include" -I "%GBDK_DIR%\include\asm" -c %%~nf.c -o obj\%%~nf.rel
	if %errorlevel% neq 0 goto :ERROR
	set RELs=!RELs!_obj\%%~nf.rel
)

:: fix RELs
set RELs=%RELs:_= %

:: link objects
echo linking...
sdcc -mgbz80 --no-std-crt0 --data-loc 0xc0a0 -L "%GBDK_DIR%\lib" "%GBDK_DIR%\lib\crt0.rel" gb.lib %RELs% -o obj\%TARGET%.ihx
if %errorlevel% neq 0 goto :ERROR

:: make it into a rom
echo making the rom...
call makebin -Z obj\%TARGET%.ihx rom\%TARGET%.gb
if %errorlevel% neq 0 goto :ERROR

:: display rom-size info and launch emulator if build succeeded
if exist tools\CheckRomSpace.exe tools\CheckRomSpace rom\%TARGET%.gb
if exist rom\bgb.exe start rom\bgb rom\%TARGET%.gb
goto :END

:ERROR
if %INTERACTIVE% neq 0 pause

:END
endlocal
echo on
@exit /b 0