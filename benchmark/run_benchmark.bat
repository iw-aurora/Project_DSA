@echo off
chcp 65001 > nul
echo ====================================================================
echo        DANG BIEN DICH VA CHAY BENCHMARK ENGINE (10M RECORDS)
echo ====================================================================

cd /d "%~dp0.."
g++ -O3 -march=native -funroll-loops -flto -DNDEBUG -std=c++17 -I. benchmark/bench_core.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o benchmark/bench_core.exe

if %errorlevel% neq 0 (
    echo [LOI] Khong the bien dich benchmark/bench_core.exe!
    pause
    exit /b %errorlevel%
)

echo [THANH CONG] Bien dich thanh cong! Dang chay benchmark...
echo ----------------------------------------------------
benchmark\bench_core.exe
echo ----------------------------------------------------
pause
