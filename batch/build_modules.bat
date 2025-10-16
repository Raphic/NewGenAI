@echo off
chcp 65001 >nul

:: TẮT QUICK EDIT MODE để tránh pause khi click
reg add HKCU\Console /v QuickEdit /t REG_DWORD /d 0 /f >nul 2>&1

setlocal enabledelayedexpansion

cd /d "%~dp0"
cd ..

:: Màu mặc định (vàng)
color 0e
title 🔧 MODULAR AI BUILD SYSTEM

:: Kiểm tra tham số dòng lệnh
set "mode=%1"

:: ==================== VERSION MANAGER ====================
set "version_file=data\version.txt"

:: Tạo file version nếu chưa có
if not exist data mkdir data
if not exist "%version_file%" echo 0.0 > "%version_file%"

:: Đọc version hiện tại
set /p current_version=<"%version_file%"

:: Parse major.minor
for /f "tokens=1,2 delims=." %%a in ("%current_version%") do (
    set "major=%%a"
    set "minor=%%b"
)

:: Xử lý version rỗng
if not defined major set "major=0"
if not defined minor set "minor=0"

if "%mode%"=="update" goto :incremental_build
if "%mode%"=="" goto :full_build

:: ==================== FULL REBUILD ====================
:full_build
:: Màu đỏ cho Full Build
color 0c

:: TĂNG MAJOR VERSION
set /a major+=1
set "minor=0"
set "new_version=%major%.%minor%"
echo %new_version% > "%version_file%"

echo ================================================
echo 🔁 BUILD MODULAR AI SYSTEM - CLEAN BUILD
echo 📦 Version: v%new_version%
echo ================================================

:: Xoá sạch build/ và file exe
if exist build (
    echo 🧹 Đang xoá thư mục build...
    rmdir /s /q build
)
if exist modular_ai.exe (
    echo 🗑️  Đang xoá modular_ai.exe cũ...
    
    :: Đóng chương trình cũ nếu đang chạy
    taskkill /F /FI "IMAGENAME eq modular_ai.exe" /T >nul 2>&1
    timeout /t 1 /nobreak >nul
    
    del /f /q modular_ai.exe
)

mkdir build

echo 🔨 Compile tất cả module...
for /R modules %%F in (*.cpp) do (
    echo ⚙️  Compile: %%~nxF
    g++ -c "%%F" -o "build\%%~nF.o"
    if !errorlevel! neq 0 (
        echo ❌ Lỗi compile: %%~nxF
        color 0e
        goto :interactive_menu
    )
)

echo ✅ Tất cả module compile OK!
goto :link_and_run

:: ==================== INCREMENTAL BUILD ====================
:incremental_build
:: Màu xanh cho Update
color 0b

::KIỂM TRA FILE THAY ĐỔI
set "new_version=%major%.%minor%"

echo ================================================
echo 🔄 UPDATE BUILD - CHỈ COMPILE FILE THAY ĐỔI
echo 📦 Version: v%new_version%
echo ================================================

:: Tạo thư mục build nếu chưa có
if not exist build (
    echo ⚠️  Thư mục build chưa tồn tại! Chuyển sang Full Build...
    echo.
    color 0e
    goto :full_build
)

set "compiled_count=0"
set "skipped_count=0"
set "need_relink=0"

echo 🔍 Đang quét các file đã thay đổi...
echo.

:: KIỂM TRA main.cpp (KHÔNG COMPILE, CHỈ CHECK THAY ĐỔI)
if exist main.cpp (
    if not exist modular_ai.exe (
        echo 📝 main.cpp chưa build lần đầu
        set "need_relink=1"
    ) else (
        :: So sánh thời gian main.cpp với modular_ai.exe
        for %%A in (main.cpp) do set "main_time=%%~tA"
        for %%B in (modular_ai.exe) do set "exe_time=%%~tB"
        
        :: Dùng PowerShell so sánh (fix đường dẫn)
        for %%A in (main.cpp) do set "main_path=%%~fA"
        for %%B in (modular_ai.exe) do set "exe_path=%%~fB"
        
        powershell -Command "if ((Get-Item '%CD%\main.cpp').LastWriteTime -gt (Get-Item '%CD%\modular_ai.exe').LastWriteTime) { exit 1 } else { exit 0 }" >nul 2>&1
        
        if !errorlevel! equ 1 (
            echo 🔄 Đã thay đổi: main.cpp
            set "need_relink=1"
        )
    )
)

for /R modules %%F in (*.cpp) do (
    set "source_file=%%F"
    set "obj_file=build\%%~nF.o"
    set "need_compile=0"
    
    :: Kiểm tra nếu .o chưa tồn tại
    if not exist "!obj_file!" (
        set "need_compile=1"
        set "reason=📦 Chưa có .o"
    ) else (
        :: Lấy đường dẫn đầy đủ để tránh lỗi PowerShell
        for %%X in ("!source_file!") do set "full_source=%%~fX"
        for %%Y in ("!obj_file!") do set "full_obj=%%~fY"
        
        :: Dùng PowerShell so sánh thời gian
        powershell -Command "if ((Get-Item '!full_source!').LastWriteTime -gt (Get-Item '!full_obj!').LastWriteTime) { exit 1 } else { exit 0 }" >nul 2>&1
        
        if !errorlevel! equ 1 (
            set "need_compile=1"
            set "reason=Đã thay đổi"
        )
    )
    
    :: Compile nếu cần
    if "!need_compile!"=="1" (
        echo ⚙️  !reason!: %%~nxF
        g++ -c "!source_file!" -o "!obj_file!"
        
        if !errorlevel! neq 0 (
            echo ❌ Lỗi compile: %%~nxF
            color 0e
            goto :interactive_menu
        )
        
        set /a compiled_count+=1
        set "need_relink=1"
    ) else (
        set /a skipped_count+=1
    )
)

echo.
echo ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
echo 📊 Kết quả quét:
echo    ✅ Compiled: !compiled_count! file(s)
echo    ⏭️  Skipped:  !skipped_count! file(s)
echo ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
echo.

:: Kiểm tra nếu không có file nào thay đổi
if "!need_relink!"=="0" (
    echo ℹ️  Không có file nào thay đổi!
    echo 📦 Version giữ nguyên: v!new_version!
    echo.
    
    if exist modular_ai.exe (
        echo 🚀 Khởi động chương trình hiện tại...
        
        :: Đóng chương trình cũ nếu đang chạy
        taskkill /F /FI "IMAGENAME eq modular_ai.exe" /T >nul 2>&1
        timeout /t 1 /nobreak >nul
        
        start modular_ai.exe
        echo.
        echo ✅ Đã khởi động Modular AI!
        goto :wait_for_command
    ) else (
        echo ❌ Chưa có modular_ai.exe! Hãy chạy Full Build trước.
        goto :wait_for_command
    )
)

:: CÓ FILE THAY ĐỔI - TĂNG VERSION VÀ LINK LẠI
set /a minor+=1
set "new_version=!major!.!minor!"
echo !new_version! > "%version_file%"
echo 📦 Version mới: v!new_version!
echo.

:: NHẢY VÀO LINK & RUN (có taskkill ở đây)
goto :link_and_run

:: ==================== LINK & RUN ====================
:link_and_run
:: KILL PROCESS TRƯỚC KHI LINK
echo 🔗 Đang chuẩn bị link executable...

:: Đóng chương trình cũ nếu đang chạy
taskkill /F /FI "IMAGENAME eq modular_ai.exe" /T >nul 2>&1
timeout /t 1 /nobreak >nul

:: BẮT ĐẦU LINK
echo 🔗 Đang link executable...
g++ -I. main.cpp build\*.o -o modular_ai.exe

if %errorlevel% neq 0 (
    echo ❌ Build thất bại!
    color 0e
    goto :wait_for_command
)

echo ✅ Build thành công!
echo.
echo 🚀 Khởi động chương trình...

start modular_ai.exe

echo.
echo --------------------------------------------
echo ✅ Đã khởi động Modular AI!

:: Reset về màu vàng
color 0e

:: ==================== INTERACTIVE MENU ====================
:interactive_menu
echo.
echo ================================================
echo 💬 MENU - Gõ lệnh để tiếp tục:
echo ================================================
echo   update  - Update build (compile file thay đổi)
echo   rebuild - Full rebuild (xóa và build lại)
echo   run     - Chạy chương trình
echo   clear   - Xóa màn hình
echo   exit    - Thoát
echo ================================================
echo.

:: ==================== WAIT FOR COMMAND ====================
:wait_for_command
set /p "cmd=👉 Nhập lệnh: "

if /i "%cmd%"=="update" (
    echo.
    goto :incremental_build
)

if /i "%cmd%"=="rebuild" (
    echo.
    goto :full_build
)

if /i "%cmd%"=="run" (
    if exist modular_ai.exe (
        echo.
        echo 🚀 Khởi động chương trình...
        
        :: Đóng chương trình cũ nếu đang chạy
        taskkill /F /FI "IMAGENAME eq modular_ai.exe" /T >nul 2>&1
        timeout /t 1 /nobreak >nul
        
        start modular_ai.exe
        echo ✅ Đã khởi động!
    ) else (
        echo ❌ Chưa có modular_ai.exe! Hãy build trước.
    )
    echo.
    goto :wait_for_command
)

if /i "%cmd%"=="clear" (
    cls
    color 0e
    title 🔧 MODULAR AI BUILD SYSTEM
    echo ================================================
    echo 💬 MENU - Gõ lệnh để tiếp tục:
    echo ================================================
    echo   update  - Update build (compile file thay đổi)
    echo   rebuild - Full rebuild (xóa và build lại)
    echo   run     - Chạy chương trình
    echo   clear   - Xóa màn hình
    echo   exit    - Thoát
    echo ================================================
    echo.
    goto :wait_for_command
)

if /i "%cmd%"=="exit" (
    :: BẬT LẠI QUICK EDIT MODE
    reg add HKCU\Console /v QuickEdit /t REG_DWORD /d 1 /f >nul 2>&1

    echo.
    echo 👋 Tạm biệt!
    exit /b 0
)

:: Lệnh không hợp lệ
echo.
echo ❌ Lệnh không hợp lệ: "%cmd%"
goto :wait_for_command