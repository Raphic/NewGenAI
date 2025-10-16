@echo off
chcp 65001 >nul
title 🔁 REBUILD MODULAR AI SYSTEM
color 0e
setlocal enabledelayedexpansion

cd /d "%~dp0"
cd ..

echo ================================================
echo 🔁 REBUILD MODULAR AI SYSTEM - CLEAN BUILD
echo ================================================
echo.

:: Xoá sạch build/ và file exe
if exist build (
    echo 🧹 Đang xoá thư mục build...
    rmdir /s /q build
)
if exist modular_ai.exe (
    echo 🗑️  Đang xoá modular_ai.exe cũ...
    del /f /q modular_ai.exe
)

mkdir build

echo 🔨 Compile tất cả module...
for /R modules %%F in (*.cpp) do (
    echo ⚙️  Compile: %%~nxF
    g++ -c "%%F" -o "build\%%~nF.o"
    if !errorlevel! neq 0 (
        echo ❌ Lỗi compile: %%~nxF
        pause
        exit /b
    )
)

echo ✅ Tất cả module compile OK!
echo 🔗 Đang link executable...
g++ -I. main.cpp build\*.o -o modular_ai.exe

if %errorlevel% neq 0 (
    echo ❌ Rebuild thất bại!
    pause
    exit /b
)

echo ✅ Rebuild thành công!
echo.
echo 🚀 Khởi động chương trình...
start modular_ai.exe

echo.
echo --------------------------------------------
echo ✅ Đã khởi động Modular AI!
pause
exit /b