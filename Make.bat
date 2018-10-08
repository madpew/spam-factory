echo off
set BIN=..\..\bin
set OBJ=obj
set ROM=rom

if not exist %OBJ% mkdir %OBJ%
if not exist %ROM% mkdir %ROM%

call %BIN%\gbdk-n-compile.bat music.c -o %OBJ%\music.rel
if %errorlevel% neq 0 goto error
call %BIN%\gbdk-n-compile.bat timer.c -o %OBJ%\timer.rel
if %errorlevel% neq 0 goto error
call %BIN%\gbdk-n-compile.bat gfx.c -o %OBJ%\gfx.rel
if %errorlevel% neq 0 goto error
call %BIN%\gbdk-n-compile.bat sfx.c -o %OBJ%\sfx.rel
if %errorlevel% neq 0 goto error
call %BIN%\gbdk-n-compile.bat gamestate.c -o %OBJ%\gamestate.rel
if %errorlevel% neq 0 goto error
call %BIN%\gbdk-n-compile.bat main.c -o %OBJ%\main.rel
if %errorlevel% neq 0 goto error

call %BIN%\gbdk-n-link.bat %OBJ%\music.rel %OBJ%\timer.rel %OBJ%\sfx.rel %OBJ%\gfx.rel %OBJ%\gamestate.rel %OBJ%\main.rel -o %OBJ%\spam.ihx
if %errorlevel% neq 0 goto error
call %BIN%\gbdk-n-make-rom.bat %OBJ%\spam.ihx %ROM%\spam.gb
if %errorlevel% neq 0 goto error
%ROM%\bgb %ROM%\spam.gb
goto end

:error
pause

:end
exit