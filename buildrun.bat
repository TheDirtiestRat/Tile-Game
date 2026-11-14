@echo off
set PATH=C:\raylib\w64devkit\bin;%PATH%

set INCLUDES=-IC:\raylib\raylib\src
set LDFLAGS=-LC:\raylib\raylib\src

echo Compiling main.cpp script file...

g++ main.cpp %INCLUDES% %LDFLAGS% -lraylib -lopengl32 -lgdi32 -lwinmm -o game.exe

if %ERRORLEVEL% EQU 0 (
    echo Compilation successful! the script is build Running game.exe...
    game.exe
) else (
    echo Compilation failed! Check the error studpid.
)

echo.
echo ---------------------------------------
echo   game.exe has been closed successful
echo ---------------------------------------

pause