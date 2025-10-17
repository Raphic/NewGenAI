@echo off
chcp 65001 >nul
:: TẮT QUICK EDIT MODE để tránh pause khi click
reg add HKCU\Console /v QuickEdit /t REG_DWORD /d 0 /f >nul 2>&1
:: Cấu hình môi trường một lần duy nhất
setlocal EnableExtensions 
setlocal EnableDelayedExpansion
:: ============================================
:: LƯU ĐƯỜNG DẪN GỐC PROJECT
:: ============================================
cd /d "%~dp0"
cd ..
set "PROJECT_ROOT=%CD%"
endlocal & set "PROJECT_ROOT=%PROJECT_ROOT%" & setlocal EnableDelayedExpansion
color 0e

:: Khởi tạo các biến toàn cục
set "SCRIPT_VERSION=1.0"
set "need_rebuild=0"
set "last_rollback_version="
title 🔧 MODULAR AI BUILD SYSTEM

:: Kiểm tra tham số dòng lệnh
set "mode=%1"

:: ==================== VERSION MANAGER ====================
set "log_file=data\build_log.txt"

:: Tạo file version nếu chưa có
if not exist data mkdir data
rem Bỏ ghi version.txt vì không dùng nữa

:: ======== VERSION MANAGER ========
call :read_version
goto :after_version_init

::Đọc và xử lý version
:read_version
setlocal EnableDelayedExpansion
set "backup_dir=batch\backup"
set "current_version=0.0"

if exist "%backup_dir%" (
    rem lấy thư mục mới nhất
    for /f "delims=" %%B in ('dir /b /ad /o-n "%backup_dir%"') do (
        set "folder=%%B"
        rem tìm vị trí _v cuối cùng, rồi cắt phần sau nó
        set "verstr=!folder!"
        :strip_loop
        for /f "delims=_v" %%a in ("!verstr!") do (
            if "%%a"=="" (
                rem không còn _v ở chuỗi, dừng
            ) else (
                set "after=!verstr:*_v=!"
                if not "!after!"=="!verstr!" (
                    set "verstr=!after!"
                    goto :strip_loop
                )
            )
        )
        for /f "tokens=1 delims= " %%v in ("!verstr!") do (
            if not "%%v"=="" (
                endlocal & set "current_version=%%v" & exit /b
            )
        )
    )
)
endlocal & set "current_version=0.0"
exit /b

:after_version_init
:wait_for_command
:: ============================================
:: FIX: ĐẢM BẢO ĐÚNG WORKING DIRECTORY
:: ============================================
cd /d "%PROJECT_ROOT%"

:: Reset màu và xoá lệnh cũ
color 0e
if not defined cmd (
    call :show_menu
    set /p "cmd=👉 Nhập lệnh: "
)

if /i "%cmd%"=="rollback" goto :do_rollback
if /i "%cmd%"=="update" goto :do_update
if /i "%cmd%"=="build" goto :do_rebuild
if /i "%cmd%"=="run" goto :do_run
if /i "%cmd%"=="clear" goto :do_clear
if /i "%cmd%"=="exit" goto :do_exit

echo.
echo ❌ Lệnh không hợp lệ: "%cmd%"
set "cmd="
goto :wait_for_command

:do_rollback
    echo ================================================
    echo 🔄 DANH SÁCH BACKUP KHÔI PHỤC (chọn 0 để thoát)
    setlocal enabledelayedexpansion
    set "backup_dir=batch\backup"
    set "count=0"
    set "max_show=5"
    set "backup_list="
    for /f "delims=" %%B in ('dir /b /ad /o-n "%backup_dir%"') do (
        set /a count+=1
        if !count! leq !max_show! (
            set "backup_list=!backup_list!%%B;"
        )
    )
    set "shown=0"
    for %%C in (!backup_list!) do (
        set /a shown+=1
        echo   !shown!. %%C
    )
    if !shown! equ 0 (
        echo ❌ Không có bản backup nào để khôi phục!
        endlocal
        set "cmd="
        goto :wait_for_command
    )
    echo ----------------------------------------------------------
    echo 💡 TIP: Version hiển thị ở cuối tên thư mục backup: _v65.0
    echo ----------------------------------------------------------
    echo --------------------------------
    set /p "choice=👉 Nhập số thứ tự backup muốn khôi phục (0 để thoát): "
    if not defined choice (
        echo ❌ Không nhập số thứ tự!
        endlocal
        set "cmd="
        goto :wait_for_command
    )
    if "%choice%"=="0" (
        endlocal
        set "cmd="
        goto :wait_for_command
    )
    set "selected="
    set "idx=0"
    for %%C in (!backup_list!) do (
        set /a idx+=1
        if "!idx!"=="%choice%" set "selected=%%C"
    )
    if not defined selected (
        echo ❌ Số thứ tự không hợp lệ!
        endlocal
        set "cmd="
        goto :wait_for_command
    )
    set "restore_path=%backup_dir%\!selected!"
    echo 🔄 Đang khôi phục từ !selected! ...
    
    :: Tắt process trước khi xóa file
    tasklist /FI "IMAGENAME eq modular_ai.exe" | find /I "modular_ai.exe" >nul && (
        echo ❎ Đang tắt tiến trình modular_ai.exe...
        taskkill /F /IM modular_ai.exe >nul 2>&1
        timeout /t 1 /nobreak >nul
    )
    
    :: Xóa files cũ
    if exist build rmdir /s /q build
    if exist modular_ai.exe del /f /q modular_ai.exe
    if exist data\version.txt del /f /q data\version.txt
    if exist data\build_log.txt del /f /q data\build_log.txt
    
    :: Restore từ backup
    if exist "!restore_path!\build" xcopy /E /I /Y "!restore_path!\build" build >nul
    if exist "!restore_path!\modular_ai.exe" copy /Y "!restore_path!\modular_ai.exe" modular_ai.exe >nul
    if exist "!restore_path!\build_log.txt" copy /Y "!restore_path!\build_log.txt" data\build_log.txt >nul

    :: Parse version từ tên folder TRƯỚC
    set "rollback_version="
    set "folder_name=!selected!"
    for /f "tokens=* delims=" %%x in ("!folder_name!") do (
        set "temp=%%x"
        set "temp=!temp:*_v=!"
    )
    set "rollback_version=!temp!"
    for /f "tokens=1 delims=-_ " %%a in ("!rollback_version!") do (
        set "rollback_version=%%a"
    )

    :: Kiểm tra version hợp lệ
    if "!rollback_version!"=="" set "rollback_version=0.0"

    :: ===== LUÔN TẠO version.txt TỪ TÊN FOLDER =====
    if not exist data mkdir data
    echo !rollback_version!> data\version.txt
    echo 📝 Đã ghi version.txt = v!rollback_version! (từ tên folder)
    :: ===== KẾT THÚC =====

    :: Check exe sau cùng
    set "exe_exists=0"
    if exist modular_ai.exe set "exe_exists=1"

    echo ================================================
    echo 🔁 ROLLBACK MODULAR AI SYSTEM - KHÔI PHỤC THÀNH CÔNG
    echo 📦 Version đã rollback: v!rollback_version!
    if "!exe_exists!"=="1" (
        echo ✅ File modular_ai.exe đã được khôi phục
    ) else (
        echo ❌ CẢNH BÁO: File modular_ai.exe không tồn tại trong backup
    )
    echo ================================================

    :: Export variables ra ngoài
    endlocal & set "current_version=%rollback_version%" & set "new_version=%rollback_version%" & set "last_rollback_version=%rollback_version%" & set "exe_exists=%exe_exists%"

    :: XỬ LÝ 2 TRƯỜNG HỢP: CÓ EXE hoặc KHÔNG CÓ EXE
    if "%exe_exists%"=="1" (
        :: CÓ FILE EXE - Khởi động luôn
        echo.
        echo 🚀 Khởi động chương trình sau rollback...
        start modular_ai.exe
        echo ✅ Đã khởi động Modular AI!
        timeout /t 1 /nobreak >nul
        set "cmd="
        goto :wait_for_command
    )

    :: KHÔNG CÓ FILE EXE - Hỏi rebuild
    echo.
    echo ❗ File modular_ai.exe không tồn tại trong backup
    setlocal enabledelayedexpansion
    set /p "rebuild_choice=👉 Bạn có muốn rebuild lại không? (Y/N): "

    if /i "!rebuild_choice!"=="Y" (
        echo.
        echo 🔄 Tiến hành rebuild sau rollback (version mới nhất + 1)...
        
        :: Tìm version MỚI NHẤT từ tất cả backup
        set "latest_version=0.0"
        set "backup_dir=batch\backup"
        for /f "delims=" %%B in ('dir /b /ad /o-n "%backup_dir%" 2^>nul') do (
            set "latest_folder=%%B"
            set "temp=!latest_folder:*_v=!"
            for /f "tokens=1 delims=-_ " %%v in ("!temp!") do (
                set "latest_version=%%v"
                goto :found_latest_version
            )
        )
        :found_latest_version
        
        :: Tăng major version từ latest
        for /f "tokens=1,2 delims=." %%a in ("!latest_version!") do (
            set /a "new_major=%%a+1"
            set "new_minor=0"
        )
        
        endlocal & set "REBUILD_AFTER_ROLLBACK=1" & set "current_version=%new_major%.%new_minor%" & set "cmd="
        goto :full_build
    )

    :: Chọn N - về menu
    set "cmd="
    endlocal
    goto :wait_for_command

:do_update
    set "cmd="
    goto :incremental_build

:do_rebuild
    set "cmd="
    goto :full_build

:do_run
    if exist modular_ai.exe (
        echo.
        echo 🚀 Khởi động chương trình...
        tasklist /FI "IMAGENAME eq modular_ai.exe" | find /I "modular_ai.exe" >nul && (
            echo ❎ Đang tắt tiến trình modular_ai.exe...
            taskkill /F /IM modular_ai.exe >nul 2>&1
            timeout /t 1 /nobreak >nul
        )
        
        if exist modular_ai.exe (
            start modular_ai.exe
            echo ✅ Đã khởi động!
            timeout /t 1 /nobreak >nul
        ) else (
            echo ❌ LỖI: File modular_ai.exe không tồn tại hoặc bị lỗi!
        )
    ) else (
        echo ❌ Không tìm thấy file modular_ai.exe!
        echo 💡 Hãy thử:
        echo    1. Chạy lệnh build để build lại
        echo    2. Hoặc rollback để khôi phục từ backup
    )
    echo.
    set "cmd="
    goto :wait_for_command

:do_clear
    cls
    color 0e
    title 🔧 MODULAR AI BUILD SYSTEM
    set "cmd="
    goto :wait_for_command

:do_exit
    tasklist /FI "IMAGENAME eq modular_ai.exe" | find /I "modular_ai.exe" >nul && (
        echo ❎ Đang tắt modular_ai.exe...
        taskkill /F /IM modular_ai.exe >nul 2>&1
        timeout /t 1 /nobreak >nul
    )
    reg add HKCU\Console /v QuickEdit /t REG_DWORD /d 1 /f >nul 2>&1
    echo.
    echo 👋 Tạm biệt!
    exit /b 0

:: ==================== FULL REBUILD ====================
:full_build
:: ============================================
:: FIX: ĐẢM BẢO ĐÚNG WORKING DIRECTORY
:: ============================================
cd /d "%PROJECT_ROOT%"

color 0c

:: Đọc version hiện tại
call :read_version
if "%REBUILD_AFTER_ROLLBACK%"=="1" (
    call :read_version
)
if "%current_version%"=="" set "current_version=0.0"
for /f "tokens=1,2 delims=." %%a in ("%current_version%") do (
    set /a "new_major=%%a+1"
    set "new_minor=0"
)
set "new_version=%new_major%.%new_minor%"

setlocal EnableDelayedExpansion
:: Kiểm tra có phải rebuild sau rollback không
if defined REBUILD_AFTER_ROLLBACK (
    set "new_version=!current_version!"
) 
:: Cập nhật version file và biến môi trường
endlocal & set "new_version=%new_version%" & set "current_version=%new_version%"

:: Log thời gian bắt đầu
set "start_time=%time%"
set "start_h=%start_time:~0,2%"
set "start_m=%start_time:~3,2%"
set "start_s=%start_time:~6,2%"
set /a "start_timestamp=(((1%start_h%)%%100)*3600 + ((1%start_m%)%%100)*60 + ((1%start_s%)%%100))"

if exist "%log_file%" del "%log_file%"
echo ==== BUILD LOG ==== > "%log_file%"
echo Bắt đầu: %start_time% >> "%log_file%"

echo ================================================
echo 🔁 BUILD MODULAR AI SYSTEM - CLEAN BUILD
echo 📦 Version mới: v%new_version%
echo ================================================

:: Hiển thị môi trường build
set "gpp_version=Unknown"
for /f "delims=" %%v in ('g++ --version 2^>nul') do (
    set "gpp_version=%%v"
    goto :break_gpp
)
:break_gpp

echo 🖥️  MÔI TRƯỜNG BUILD
echo --------------------------------
echo   Compiler: %gpp_version%
echo   Đường dẫn g++:
where g++ 2>nul || echo   (Không tìm thấy g++ trong PATH)
echo --------------------------------
echo.

:: Backup trước khi build
set "backup_dir=batch\backup"
if not exist "%backup_dir%" mkdir "%backup_dir%"
set "backup_time=%date:~6,4%-%date:~3,2%-%date:~0,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%"
set "backup_label=backup_%backup_time%_v%new_version%"
set "backup_path=%backup_dir%\%backup_label%"

echo 📦 Đang tạo backup...
mkdir "%backup_path%" 2>nul
mkdir "%backup_path%\modules" 2>nul
mkdir "%backup_path%\build" 2>nul

:: Backup source files (chỉ nếu modules tồn tại)
if exist modules (
    robocopy /E /NFL /NDL /NJH /NJS "modules" "%backup_path%\modules" >nul 2>&1
)
if exist main.cpp copy /Y "main.cpp" "%backup_path%\" >nul 2>&1

:: Backup build folder nếu tồn tại
if exist build (
    robocopy /E /NFL /NDL /NJH /NJS "build" "%backup_path%\build" >nul 2>&1
)

:: Backup exe nếu tồn tại
if exist modular_ai.exe copy /Y modular_ai.exe "%backup_path%\modular_ai.exe" >nul 2>&1

:: Backup version và log
: Backup version và log
if exist "%log_file%" copy /Y "%log_file%" "%backup_path%\build_log.txt" >nul 2>&1
if exist data\version.txt (
    if not exist "%backup_path%\data" mkdir "%backup_path%\data"
    copy /Y data\version.txt "%backup_path%\data\version.txt" >nul 2>&1
)

echo 🗄️  Đã backup vào "%backup_path%"

:: Kill process trước khi xóa files
echo 🔄 Kiểm tra và tắt process cũ...
tasklist /FI "IMAGENAME eq modular_ai.exe" | find /I "modular_ai.exe" >nul && (
    echo ❎ Đang tắt tiến trình modular_ai.exe...
    taskkill /F /IM modular_ai.exe >nul 2>&1
    :wait_process
    timeout /t 1 /nobreak >nul
    tasklist /FI "IMAGENAME eq modular_ai.exe" | find /I "modular_ai.exe" >nul && goto :wait_process
)

:: Xoá sạch build/ và file exe
if exist build (
    echo 🧹 Đang xoá thư mục build...
    rmdir /s /q build
)
if exist modular_ai.exe (
    echo 🗑️  Đang xoá modular_ai.exe cũ...
    del /f /q modular_ai.exe || (
        echo ❌ Không thể xóa file modular_ai.exe. Thử lại...
        timeout /t 2 /nobreak >nul
        del /f /q modular_ai.exe
    )
)

mkdir build
:: ===== GHI VERSION VÀO FILE =====
if not exist data mkdir data
echo %new_version%> data\version.txt
echo 📝 Đã ghi version v%new_version% vào data\version.txt
:: ===== KẾT THÚC =====
setlocal EnableDelayedExpansion
set /a success=0
set /a fail=0

echo 🔨 Compile tất cả module...
for /R modules %%F in (*.cpp) do (
    g++ -c "%%F" -o "build\%%~nF.o"
    if !errorlevel! neq 0 (
        echo ❌ Lỗi compile: %%~nxF
        echo ❌ Lỗi compile: %%~nxF >> "%log_file%"
        set /a fail+=1
    ) else (
        echo ✅ Thành công: %%~nxF
        echo ✅ Thành công: %%~nxF >> "%log_file%"
        set /a success+=1
    )
)

:: Log thời gian kết thúc
set "end_time=%time%"
set "end_h=%end_time:~0,2%"
set "end_m=%end_time:~3,2%"
set "end_s=%end_time:~6,2%"
set /a "end_timestamp=(((1%end_h%)%%100)*3600 + ((1%end_m%)%%100)*60 + ((1%end_s%)%%100))"
set /a "duration_sec=end_timestamp-start_timestamp"
set /a "duration_min=duration_sec/60"
set /a "duration_sec_rem=duration_sec%%60"

echo Kết thúc: %end_time% >> "%log_file%"
echo ============================== >> "%log_file%"
echo Thành công: !success! >> "%log_file%"
echo Lỗi: !fail! >> "%log_file%"
echo Thời gian build: !duration_min! phút !duration_sec_rem! giây >> "%log_file%"

:: Hiển thị báo lỗi riêng nếu có
if !fail! gtr 0 (
    echo ===== DANH SÁCH FILE LỖI =====
    findstr /C:"❌ Lỗi compile:" "%log_file%"
    echo ⏱️ Thời gian build: !duration_min! phút !duration_sec_rem! giây
    endlocal
    goto :wait_for_command
) else (
    echo ✅ Build thành công tất cả module!
    echo ⏱️ Thời gian build: !duration_min! phút !duration_sec_rem! giây
)

endlocal
goto :link_and_run

:: ==================== INCREMENTAL BUILD ====================
:incremental_build
:: ============================================
:: FIX: ĐẢM BẢO ĐÚNG WORKING DIRECTORY
:: ============================================
cd /d "%PROJECT_ROOT%"

color 0b

:: Log thời gian bắt đầu
set "start_time=%time%"
set "start_h=%start_time:~0,2%"
set "start_m=%start_time:~3,2%"
set "start_s=%start_time:~6,2%"
set /a "start_timestamp=(((1%start_h%)%%100)*3600 + ((1%start_m%)%%100)*60 + ((1%start_s%)%%100))"

echo ================================================
echo 🔄 UPDATE BUILD - CHỈ COMPILE FILE THAY ĐỔI
echo 📦 Version hiện tại: v%current_version%
echo ================================================

:: Tạo thư mục build nếu chưa có - CHUYỂN SANG FULL BUILD
if not exist build (
    echo ⚠️  Thư mục build chưa tồn tại! Chuyển sang Full Build...
    echo.
    set "cmd="
    goto :full_build
)

:: Kiểm tra modular_ai.exe có tồn tại không
if not exist modular_ai.exe (
    echo ⚠️  File modular_ai.exe chưa tồn tại! Chuyển sang Full Build...
    echo.
    set "cmd="
    goto :full_build
)

setlocal EnableDelayedExpansion
set "compiled_count=0"
set "skipped_count=0"
set "need_relink=0"

echo 🔍 Đang quét các file đã thay đổi...
echo.

:: Kiểm tra main.cpp
if exist main.cpp (
    if not exist modular_ai.exe (
        echo 📝 main.cpp chưa build lần đầu
        set "need_relink=1"
    ) else (
        powershell -Command "if ((Get-Item '%CD%\main.cpp').LastWriteTime -gt (Get-Item '%CD%\modular_ai.exe').LastWriteTime) { exit 1 } else { exit 0 }" >nul 2>&1
        
        if !errorlevel! equ 1 (
            echo 🔄 Đã thay đổi: main.cpp
            set "need_relink=1"
        )
    )
)

:: Quét modules
for /R modules %%F in (*.cpp) do (
    set "source_file=%%F"
    set "obj_file=build\%%~nF.o"
    set "need_compile=0"
    
    if not exist "!obj_file!" (
        set "need_compile=1"
        set "reason=📦 Chưa có .o"
    ) else (
        for %%X in ("!source_file!") do set "full_source=%%~fX"
        for %%Y in ("!obj_file!") do set "full_obj=%%~fY"
        
        powershell -Command "if ((Get-Item '!full_source!').LastWriteTime -gt (Get-Item '!full_obj!').LastWriteTime) { exit 1 } else { exit 0 }" >nul 2>&1
        
        if !errorlevel! equ 1 (
            set "need_compile=1"
            set "reason=🔄 Đã thay đổi"
        )
    )
    
    if "!need_compile!"=="1" (
        echo ⚙️  !reason!: %%~nxF
        g++ -c "!source_file!" -o "!obj_file!"
        
        if !errorlevel! neq 0 (
            echo ❌ Lỗi compile: %%~nxF
            echo.
            echo 💡 Hãy kiểm tra lỗi và thử lại!
            color 0e
            endlocal
            set "cmd="
            goto :wait_for_command
        )
        
        set /a compiled_count+=1
        set "need_relink=1"
    ) else (
        set /a skipped_count+=1
    )
)

:: Log thời gian kết thúc
set "end_time=%time%"
set "end_h=%end_time:~0,2%"
set "end_m=%end_time:~3,2%"
set "end_s=%end_time:~6,2%"
set /a "end_timestamp=(((1%end_h%)%%100)*3600 + ((1%end_m%)%%100)*60 + ((1%end_s%)%%100))"
set /a "duration_sec=end_timestamp-start_timestamp"
set /a "duration_min=duration_sec/60"
set /a "duration_sec_rem=duration_sec%%60"

echo.
echo ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
echo 📊 Kết quả quét:
echo    ✅ Compiled: !compiled_count! file(s)
echo    ⏭️  Skipped:  !skipped_count! file(s)
echo    ⏱️  Thời gian: !duration_min! phút !duration_sec_rem! giây
echo ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
echo.

:: Không có file nào thay đổi
if "!need_relink!"=="0" (
    echo ℹ️  Không có file nào thay đổi!
    echo 📦 Version giữ nguyên: v%current_version%
    echo.
    
    if exist modular_ai.exe (
        echo 🚀 Khởi động chương trình hiện tại...
        
        tasklist /FI "IMAGENAME eq modular_ai.exe" | find /I "modular_ai.exe" >nul && (
            echo ❎ Đang tắt tiến trình modular_ai.exe...
            taskkill /F /IM modular_ai.exe >nul 2>&1
            timeout /t 1 /nobreak >nul
        )
        
        start modular_ai.exe
        echo ✅ Đã khởi động Modular AI!
    ) else (
        echo ❌ Chưa có modular_ai.exe! Hãy chạy lệnh build trước.
    )
    
    echo.
    endlocal
    set "cmd="
    goto :wait_for_command
)

:: Có file thay đổi - Tăng minor version
echo 📦 Có file thay đổi! Tăng minor version...

:: Đọc lại version từ biến current_version
for /f "tokens=1,2 delims=." %%a in ("%current_version%") do (
    set "major=%%a"
    set "minor=%%b"
)

set /a minor+=1
set "new_version=!major!.!minor!"
:: ===== GHI VERSION VÀO FILE =====
if not exist data mkdir data
echo !new_version!> data\version.txt
echo 📝 Đã cập nhật version v!new_version! vào data\version.txt
:: ===== KẾT THÚC =====
:: ===== THÊM BACKUP Ở ĐÂY =====
echo 📦 Đang tạo backup trước khi link...
set "backup_dir=batch\backup"
if not exist "!backup_dir!" mkdir "!backup_dir!"
set "backup_time=%date:~6,4%-%date:~3,2%-%date:~0,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%"
set "backup_label=backup_!backup_time!_v!new_version!"
set "backup_path=!backup_dir!\!backup_label!"

mkdir "!backup_path!" 2>nul
mkdir "!backup_path!\build" 2>nul

if exist build robocopy /E /NFL /NDL /NJH /NJS "build" "!backup_path!\build" >nul 2>&1
if exist modular_ai.exe copy /Y modular_ai.exe "!backup_path!\modular_ai.exe" >nul 2>&1
if exist modules robocopy /E /NFL /NDL /NJH /NJS "modules" "!backup_path!\modules" >nul 2>&1
if exist main.cpp copy /Y main.cpp "!backup_path!\main.cpp" >nul 2>&1
if exist data\version.txt (
    if not exist "!backup_path!\data" mkdir "!backup_path!\data"
    copy /Y data\version.txt "!backup_path!\data\version.txt" >nul 2>&1
)

echo 🗄️  Đã backup vào "!backup_path!"
:: ===== KẾT THÚC THÊM =====
echo.

:: Export tất cả biến cần thiết
endlocal & (
    set "new_version=!new_version!"
    set "current_version=!new_version!"
    set "duration_min=!duration_min!"
    set "duration_sec_rem=!duration_sec_rem!"
    set "last_rollback_version="
)
:: Ghi version mới ra ngoài để menu đọc
set "last_rollback_version="

:: Parse lại major.minor từ new_version
for /f "tokens=1,2 delims=." %%a in ("%new_version%") do (
    set "major=%%a"
    set "minor=%%b"
)
goto :link_and_run
cd /d "%PROJECT_ROOT%"
echo 🔗 Đang chuẩn bị link executable...
:: ==================== LINK & RUN ====================
:link_and_run
:: ============================================
:: FIX: ĐẢM BẢO ĐÚNG WORKING DIRECTORY
:: ============================================
cd /d "%PROJECT_ROOT%"
echo 🔗 Đang chuẩn bị link executable...

:: Đóng chương trình cũ nếu đang chạy
tasklist /FI "IMAGENAME eq modular_ai.exe" | find /I "modular_ai.exe" >nul && (
    echo ❎ Đang tắt tiến trình modular_ai.exe...
    taskkill /F /IM modular_ai.exe >nul 2>&1
    timeout /t 1 /nobreak >nul
)

:: BẮT ĐẦU LINK
echo 🔗 Đang link executable...
g++ -I. main.cpp build\*.o -o modular_ai.exe

if %errorlevel% neq 0 (
    echo ❌ Link thất bại!
    echo 💡 Kiểm tra lỗi link và thử lại.
    color 0e
    set "cmd="
    goto :wait_for_command
)

echo ================================================
echo ✅ BUILD MODULAR AI SYSTEM - THÀNH CÔNG!

:: Hiển thị version
if defined new_version (
    echo 📦 Version: v%new_version%
) else (
    echo 📦 Version: v%current_version%
)

:: Hiển thị thời gian nếu có
if defined duration_min (
    echo ⏱️  Thời gian build: %duration_min% phút %duration_sec_rem% giây
)
echo ================================================
echo.

echo 🚀 Khởi động chương trình...
start modular_ai.exe
echo ✅ Đã khởi động Modular AI!
echo.

:: Reset về màu vàng
color 0e

:: Đọc lại version trước khi về menu
call :read_version
set "last_rollback_version=%current_version%"

:: Reset flags
set "REBUILD_AFTER_ROLLBACK="
set "cmd="

goto :wait_for_command

:: ==================== SHOW MENU ====================
:show_menu
setlocal EnableDelayedExpansion
echo.
echo ================================================
echo 💬 MENU - Gõ lệnh để tiếp tục:
echo ================================================

:: ============================================
:: FIX: ĐỌC VERSION VỚI DELAYEDEXPANSION
:: ============================================
set "display_version=0.0"

call :read_version
set "display_version=%current_version%"
:: Loại bỏ khoảng trắng
set "display_version=!display_version: =!"
:: Nếu có version rollback thì ưu tiên
if defined last_rollback_version (
    set "display_version=!last_rollback_version!"
)

echo 📦 Version hiện tại: v%display_version%
echo ------------------------------------------------
echo   update   - Update build (chỉ compile file thay đổi)
echo   build    - Build mới hoàn toàn (tăng major version)
echo   rollback - Khôi phục từ backup
echo   run      - Chạy chương trình 
echo   clear    - Xóa màn hình
echo   exit     - Thoát
echo ================================================
echo.
endlocal
exit /b
