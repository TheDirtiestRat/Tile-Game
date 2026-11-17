
@echo off

set PATH=C:\raylib\w64devkit\bin;%PATH%

set INCLUDES=-IC:\raylib\raylib\src -Iheader
set LDFLAGS=-LC:\raylib\raylib\src

echo.
echo =======================================
echo       COMPILING RAYLIB GAME
echo =======================================

g++ main.cpp src\*.cpp %INCLUDES% %LDFLAGS% -lraylib -lopengl32 -lgdi32 -lwinmm -o game.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilation successful!
    echo Running game.exe...
    game.exe
) else (
    echo.
    echo Compilation failed! Check the errors above.
)

echo.
echo =======================================
echo       GAME CLOSED
echo =======================================

pause
