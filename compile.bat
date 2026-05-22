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
 
echo  [*] Compiling...
echo.
 
g++ main.cpp -o SnakeCroccante.exe -lraylib -lopengl32 -lgdi32 -lwinmm
 
if %errorlevel% == 0 (
    echo.
    echo  [OK] Compilation successful!
    echo  [*] Launching TrisCroccante...
    echo.
    start SnakeCroccante.exe
) else (
    echo.
    echo  [ERROR] Compilation failed!
    echo.
    pause
)
 