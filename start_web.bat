@echo off
chcp 65001 > nul
echo ====================================================================
echo        KHOI DONG HE THONG WEB DSA (NEXT.JS + ANTD + C++ CORE)
echo ====================================================================

echo [1/3] Kiem tra va bien dich C++ DSA Bridge...
g++ -O2 -std=c++17 -I. -o dsa_bridge.exe dsa_bridge.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp
if errorlevel 1 (
    echo [LOI] Khong the bien dich dsa_bridge.exe!
    pause
    exit /b 1
)

echo [2/3] Khoi chay Backend API Server (Port 5000)...
start "DSA Backend API (Port 5000)" cmd /k "cd web_dsa && npm run server"

echo [3/3] Khoi chay Frontend React App (Port 3000)...
start "DSA Frontend React (Port 3000)" cmd /k "cd web_dsa && npm run dev"

echo.
echo ====================================================================
echo   He thong dang khoi chay!
echo   Frontend: http://localhost:3000
echo   Backend : http://localhost:5000
echo ====================================================================
