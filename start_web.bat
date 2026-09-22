@echo off
chcp 65001 > nul
echo ====================================================================
echo        KHOI DONG HE THONG WEB DSA (NEXT.JS + ANTD + C++ CORE)
echo ====================================================================

echo [1/3] Kiem tra va bien dich C++ DSA Bridge...
g++ -O2 -std=c++17 -I. -o dsa_bridge.exe dsa_bridge.cpp src/core_minhanh/*.cpp src/core_mytra/*.cpp src/core_phat/*.cpp src/core_trang/*.cpp src/core_tra/*.cpp
if errorlevel 1 (
    echo [LOI] Khong the bien dich dsa_bridge.exe!
    pause
    exit /b 1
)

echo [2/3] Khoi chay Backend API Server (Port 5000)...
start "DSA Backend API (Port 5000)" cmd /k "node server.js"

echo [3/3] Khoi chay Frontend React App (Port 3000)...
cd web_dsa
start "DSA Frontend React (Port 3000)" cmd /k "npm run dev"

echo.
echo ====================================================================
echo   He thong dang khoi chay!
echo   Frontend: http://localhost:3000
echo   Backend : http://localhost:5000
echo ====================================================================

