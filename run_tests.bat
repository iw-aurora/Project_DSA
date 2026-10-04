@echo off
chcp 65001 > nul
echo ====================================================================
echo        DANG BIEN DICH VA CHAY BO TEST TU DONG (DSA TEST SUITE)
echo ====================================================================

g++ -O2 -std=c++17 -I. tests/automated_test.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o tests/automated_test.exe

if %errorlevel% neq 0 (
    echo [LOI] Khong the bien dich bo test tu dong!
    pause
    exit /b %errorlevel%
)

echo [THANH CONG] Bien dich thanh cong! Dang chay kiem thu toan bo yeu cau...
echo.
tests\automated_test.exe

echo.
pause
