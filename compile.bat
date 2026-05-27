@echo off
echo.
echo  ================================
echo   SnakeCroccante - Compiler
echo  ================================
echo.
 
if exist SnakeCroccante.exe (
    echo  [*] Removing old executable...
    del SnakeCroccante.exe
)
 
echo  [*] Compiling resources...
windres icon.rc -o icon.o

echo  [*] Compiling...
echo.
 
g++ main.cpp icon.o -o SnakeCroccante.exe -lraylib -lopengl32 -lgdi32 -lwinmm
 
if %errorlevel% == 0 (
    echo.
    echo  [OK] Compilation successful!
    echo  [*] Launching SnakeCroccante...
    echo.
    start SnakeCroccante.exe
) else (
    echo.
    echo  [ERROR] Compilation failed!
    echo.
    pause
)