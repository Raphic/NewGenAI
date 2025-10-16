@echo off
chcp 65001 >nul
title 🚀 ADVANCED MODULAR AI SYSTEM
color 0a
setlocal enabledelayedexpansion

cd /d "%~dp0"
cd ..

echo ================================================
echo 🚀 ADVANCED MODULAR AI SYSTEM - SMART BUILD MODE
echo ================================================
echo.

:: Tạo thư mục build nếu chưa có
if not exist build mkdir build

:: Duyệt mọi file .cpp trong modules/, chỉ build cái nào mới hoặc thay đổi
echo 🔍 Kiểm tra và compile các module...
for /R modules %%F in (*.cpp) do (
    set "SRC=%%F"
    set "OBJ=build\%%~nF.o"

    if not exist "!OBJ!" (
        echo ⚙️  Compile mới: %%~nxF
        g++ -c "!SRC!" -o "!OBJ!"
        if !errorlevel! neq 0 (
            echo ❌ Lỗi compile: %%~nxF
            pause
            exit /b
        )
    ) else (
        for %%A in ("!SRC!") do for %%B in ("!OBJ!") do (
            if %%~tA GTR %%~tB (
                echo ♻️ Recompile (đã thay đổi): %%~nxF
                g++ -c "!SRC!" -o "!OBJ!"
                if !errorlevel! neq 0 (
                    echo ❌ Lỗi compile: %%~nxF
                    pause
                    exit /b
                )
            )
        )
    )
)

echo ✅ Toàn bộ module compile OK!
echo 🔗 Linking executable...
g++ -I. main.cpp build\*.o -o modular_ai.exe
if %errorlevel% neq 0 (
    echo ❌ Link thất bại!
    pause
    exit /b
)

echo ✅ Build hoàn tất!
echo.
echo 🚀 Đang chạy Modular AI...
echo ================================================
echo.

modular_ai.exe

echo.
echo -----------------------------------------------
echo ✅ Chương trình đã kết thúc.
pause
exit /b