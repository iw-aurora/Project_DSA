@echo off
echo ====================================================
echo        DANG BIEN DICH VÀ CHAY PROJECT_DSA
echo ====================================================
g++ main.cpp src/StudentDatabase.cpp -o main.exe

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
