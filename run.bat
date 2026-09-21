@echo off
echo ====================================================
echo        DANG BIEN DICH VA CHAY PROJECT_DSA
echo ====================================================

g++ main.cpp src/*.cpp src/core_minhanh/*.cpp src/core_mytra/*.cpp src/core_phat/*.cpp src/core_trang/*.cpp src/core_tra/*.cpp -o main.exe

if %errorlevel% neq 0 (
    echo [LOI] Bien dich THAT BAI! Vui long kiem tra lai trinh bien dich g++.
    pause
    exit /b %errorlevel%
)

echo [THANH CONG] Bien dich thanh cong! Dang chay main.exe...
echo ----------------------------------------------------
main.exe
echo ----------------------------------------------------
pause
