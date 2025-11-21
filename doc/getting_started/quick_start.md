# ⚡ QUICK START - Bắt Đầu Nhanh

**Version:** v77.1  
**Estimated Time:** 5-10 phút

---

## 📋 YÊU CẦU HỆ THỐNG

### **Cần có:**
- ✅ Windows 10/11
- ✅ MinGW (g++ compiler)
- ✅ Command Prompt / PowerShell
- ✅ Text Editor (VS Code, Notepad++, etc.)

### **Không cần:**
- ❌ Visual Studio
- ❌ Database
- ❌ Internet (chạy local)

---

## 🚀 SETUP LẦN ĐẦU

### **Bước 1: Clone/Download Project**

```bash
# Option A: Clone với Git
git clone <repo-url> NewGen
cd NewGen

# Option B: Download ZIP và extract
# Đảm bảo folder name là "NewGen"
```

---

### **Bước 2: Kiểm tra MinGW**

```bash
# Check g++ đã cài chưa
g++ --version

# Kết quả mong đợi:
# g++ (MinGW...) 8.x.x hoặc cao hơn
```

**Nếu chưa có g++:**
1. Download MinGW: https://sourceforge.net/projects/mingw/
2. Install và add vào PATH
3. Restart terminal

---

### **Bước 3: Build Project**

```bash
# Chạy build system
batch\build_modules.bat

# Chọn option
> build

# Đợi compile (~30-60 giây)
# Nếu thành công sẽ hiển thị:
# ✅ Build thành công! Version: v77.1
```

---

### **Bước 4: Chạy Program**

```bash
# Trong build system, chọn
> run

# Hoặc chạy trực tiếp
modular_ai.exe
```

**Kết quả:**
```
================================================================================
                    🤖 MODULAR AI SYSTEM v77.1 - Nâng cao 🤖
================================================================================
📚 Đã load 60 problems từ lịch sử
🧠 AI đã sẵn sàng!

📌 Gõ 'help' để xem danh sách lệnh
📌 Gõ 'calculator' để xem hướng dẫn toán học

Nhập lệnh hoặc phép tính:
>
```

---

## ✅ TEST FEATURES

### **1. Test Calculator:**

```bash
# Nhập vào program
> 2+3
# Kết quả: 5

> x^2-5x+6=0
# Kết quả: x = 2 hoặc x = 3

> sqrt(16)
# Kết quả: 4
```

---

### **2. Test Commands:**

```bash
> help
# Hiển thị tất cả lệnh

> calculator
# Xem hướng dẫn toán học

> stats
# Xem thống kê sử dụng
```

---

### **3. Test AI:**

```bash
> train
# Train AI từ history

> suggest
# AI gợi ý method

> classify x^2-4=0
# AI phân loại problem type
```

---

## 🎓 NEXT STEPS - TIẾP THEO LÀM GÌ?

### **1. Đọc Documentation** 📚

Theo thứ tự:
1. **[PROJECT_REVIEW.md](PROJECT_REVIEW.md)** - Hiểu tổng quan dự án
2. **[WORKFLOW.md](WORKFLOW.md)** - Học quy trình làm việc
3. **[THINKING_PROCESS.md](THINKING_PROCESS.md)** - Hiểu tư duy thiết kế

---

### **2. Thử Code** 💻

```bash
# 1. Mở file để xem
modules/math/calculator.cpp

# 2. Sửa một dòng nhỏ (ví dụ: thêm comment)

# 3. Build lại
batch\build_modules.bat > update

# 4. Test
modular_ai.exe
```

---

### **3. Xem Tasks** ✅

```bash
# Mở TODO.md
# Xem task hiện tại và next steps
```

---

## 📁 PROJECT STRUCTURE

```
NewGen/
├── main.cpp                   # Entry point
├── modular_ai.exe             # Executable
│
├── modules/                   # Source code
│   ├── ai/                    # AI modules
│   ├── math/                  # Math modules
│   ├── system/                # System modules
│   └── utils/                 # Utilities
│
├── data/                      # Data files
│   ├── math_history.json      # Learning data
│   ├── ai_knowledge.json      # AI patterns
│   └── knowledge/             # Knowledge Base
│
├── batch/                     # Build system
│   └── build_modules.bat      # Build script
│
├── build/                     # Compiled .o files
│
└── doc/                       # Documentation
    ├── README.md              # ← BẮT ĐẦU TỪ ĐÂY
    ├── QUICK_START.md         # ← BẠN ĐANG Ở ĐÂY
    └── ... (other docs)
```

---

## 🚨 TROUBLESHOOTING - XỬ LÝ LỖI

### **Lỗi: "g++ không được nhận dạng"**

**Nguyên nhân:** MinGW chưa cài hoặc chưa add vào PATH

**Giải pháp:**
1. Install MinGW
2. Add `C:\MinGW\bin` vào System PATH
3. Restart terminal
4. Chạy lại `g++ --version`

---

### **Lỗi: "Build failed"**

**Nguyên nhân:** Code có syntax error

**Giải pháp:**
1. Check error message (file nào, dòng nào)
2. Mở file và fix lỗi
3. Build lại

---

### **Lỗi: "modular_ai.exe không tìm thấy"**

**Nguyên nhân:** Build chưa thành công

**Giải pháp:**
```bash
# Xóa build cũ và build lại
batch\build_modules.bat > build
```

---

### **Program crash khi chạy**

**Nguyên nhân:** Data files corrupted

**Giải pháp:**
```bash
# Restore từ backup
copy data\backup\math_history_backup.json data\math_history.json

# Hoặc rollback version
batch\build_modules.bat > rollback
```

---

## 💡 TIPS CHO NGƯỜI MỚI

### **1. Dùng Incremental Build**
```bash
# Nhanh hơn full rebuild
batch\build_modules.bat > update
```

---

### **2. Backup trước khi thử nghiệm**
```bash
# Build system tự động backup
# Nếu lỗi → rollback ngay
batch\build_modules.bat > rollback
```

---

### **3. Đọc PROJECT_REVIEW.md**
File này có **TẤT CẢ** thông tin về dự án:
- Architecture
- Modules
- Commands
- Issues
- Recommendations

---

### **4. Check TODO.md mỗi ngày**
Biết đang làm gì, làm tiếp gì

---

### **5. Follow WORKFLOW.md**
Làm việc đúng quy trình → Ít lỗi hơn

---

## 📞 CẦN TRỢ GIÚP?

### **Đọc docs:**
1. [README.md](README.md) - Navigation hub
2. [PROJECT_REVIEW.md](PROJECT_REVIEW.md) - Tổng quan
3. [WORKFLOW.md](WORKFLOW.md) - Quy trình làm việc
4. [THINKING_PROCESS.md](THINKING_PROCESS.md) - Tư duy thiết kế

### **Check code:**
- Xem `modules/` folder
- Đọc comments trong code (tiếng Việt)

---

## ✅ CHECKLIST - ĐÃ SETUP XONG?

- [ ] MinGW đã cài và g++ chạy được
- [ ] Project đã clone/download
- [ ] Build thành công (`batch\build_modules.bat > build`)
- [ ] Program chạy được (`modular_ai.exe`)
- [ ] Test calculator: `2+3` → `5`
- [ ] Test command: `help` → Hiện danh sách lệnh
- [ ] Đọc README.md navigation
- [ ] Đọc PROJECT_REVIEW.md tổng quan

**Nếu tất cả ✅ → Bạn đã sẵn sàng code! 🚀**

---

**Next:** Đọc [WORKFLOW.md](WORKFLOW.md) để học quy trình làm việc hàng ngày.
