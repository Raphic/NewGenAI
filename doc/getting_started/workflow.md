# 🔧 WORKFLOW - Quy Trình Làm Việc

**Version:** v77.1  
**Last Updated:** 2025-11-21

---

## 🏗️ BUILD & RUN COMMANDS

### **Sử dụng Build System (Khuyên dùng)**

#### **1. Incremental Build (Cập nhật nhanh)**
Dùng khi bạn chỉ sửa 1-2 files, build system sẽ chỉ compile files thay đổi.

```bash
# Chạy build system
batch\build_modules.bat

# Chọn option
> update
```

**Kết quả:**
- ✅ Tự động detect files thay đổi
- ✅ Compile chỉ files cần thiết
- ✅ Version tăng minor (v77.1 → v77.2)
- ✅ Thời gian: ~5-10 giây

---

#### **2. Full Rebuild (Build lại toàn bộ)**
Dùng khi:
- Thêm module mới
- Sửa header files quan trọng
- Sau khi rollback
- Khi incremental build lỗi

```bash
batch\build_modules.bat
> build
```

**Kết quả:**
- ✅ Auto-backup trước khi build
- ✅ Clean tất cả .o files
- ✅ Compile lại tất cả modules
- ✅ Version tăng major (v77.1 → v78.0)
- ✅ Thời gian: ~30-60 giây

---

#### **3. Run Program (Chạy chương trình)**

```bash
batch\build_modules.bat
> run
```

Hoặc chạy trực tiếp:
```bash
modular_ai.exe
```

---

#### **4. Rollback (Khôi phục version cũ)**

```bash
batch\build_modules.bat
> rollback
```

**Workflow:**
1. Hiển thị danh sách backups
2. Chọn version muốn restore
3. Tự động restore files
4. Hỏi có muốn rebuild không

---

#### **5. Other Commands**

```bash
# Clear screen
> clear

# Exit build system
> exit
```

---

## 💻 DEVELOPMENT CYCLE (Quy trình phát triển)

### **Workflow hàng ngày:**

```
1. Bắt đầu ngày
   ├─ Mở TODO.md → Xem task cần làm
   ├─ Check version hiện tại (data/version.txt)
   └─ Review code đang dở

2. Code new feature / Fix bug
   ├─ Edit file .cpp/.h
   ├─ Thêm comments tiếng Việt
   ├─ Follow CODING_CONVENTIONS.md
   └─ Save file

3. Build & Test
   ├─ Run `build_modules.bat > update`
   ├─ Check compile errors
   ├─ Nếu lỗi → Fix → Update lại
   └─ Nếu OK → Run program

4. Test manually
   ├─ Chạy modular_ai.exe
   ├─ Test feature vừa code
   ├─ Test commands liên quan
   └─ Check output đúng không

5. Nếu major change
   ├─ Run `build_modules.bat > build` (full rebuild)
   └─ Test kỹ hơn

6. Commit work (optional)
   ├─ Update TODO.md (mark [x] done)
   ├─ Update CHANGELOG nếu cần
   └─ Git commit (nếu dùng Git)

7. Kết thúc ngày
   └─ Update TODO.md với progress
```

---

## 🔨 MANUAL BUILD (Nếu batch script lỗi)

### **Compile thủ công:**

```bash
# Compile từng module
g++ -c modules/utils/common.cpp -o build/common.o
g++ -c modules/ai/storage_json.cpp -o build/storage_json.o
g++ -c modules/ai/math_logger.cpp -o build/math_logger.o
g++ -c modules/ai/pattern_recognizer.cpp -o build/pattern_recognizer.o
g++ -c modules/ai/data_importer.cpp -o build/data_importer.o
g++ -c modules/ai/data_exporter.cpp -o build/data_exporter.o
g++ -c modules/ai/data_cleaner.cpp -o build/data_cleaner.o
g++ -c modules/ai/ai_trainer.cpp -o build/ai_trainer.o
g++ -c modules/math/calculator.cpp -o build/calculator.o
g++ -c modules/math/statistics.cpp -o build/statistics.o
g++ -c modules/system/command_handler.cpp -o build/command_handler.o
g++ -c modules/system/command_registry.cpp -o build/command_registry.o

# Link executable
g++ -I. main.cpp build/*.o -o modular_ai.exe

# Run
modular_ai.exe
```

---

## 📝 CODING CONVENTIONS

### **Ngôn ngữ:**
- ✅ **Code:** English (functions, variables)
- ✅ **Comments:** Tiếng Việt
- ✅ **UI/Output:** Tiếng Việt + Emoji
- ✅ **Documentation:** Tiếng Việt

### **Naming:**
```cpp
// Namespace: lowercase
namespace math_engine { }

// Class: PascalCase
class MathLogger { }

// Function: camelCase
void calculateExpression() { }

// Variable: camelCase
int resultValue = 0;

// Constant: UPPER_CASE
const int MAX_SIZE = 100;
```

### **File Organization:**
```cpp
// Header file (.h)
#ifndef MODULE_NAME_H
#define MODULE_NAME_H

// Includes
// Namespace declaration
// Class/struct definitions
// Function declarations

#endif

// Implementation file (.cpp)
#include "module_name.h"

// Other includes
// Function implementations
```

### **Comments:**
```cpp
// ✅ GOOD: Giải thích TẠI SAO
// Dùng delta method vì phương trình có thể có nghiệm kép
float delta = b*b - 4*a*c;

// ❌ BAD: Chỉ mô tả code làm gì
// Calculate delta
float delta = b*b - 4*a*c;
```

---

## 🧪 TESTING WORKFLOW

### **Manual Testing:**

```bash
# 1. Run program
modular_ai.exe

# 2. Test commands
help              # Xem tất cả lệnh
calculator        # Test math functions
stats             # Check statistics
train             # Train AI
import            # Test import
export            # Test export

# 3. Test math
2+3
x^2-5x+6=0
sqrt(16)
sin(30)

# 4. Test pattern recognition
classify x^2-4=0
suggest
analyze

# 5. Exit
exit
```

---

## 📂 MODULE STRUCTURE

### **Khi thêm module mới:**

1. **Tạo files:**
   ```
   modules/[category]/module_name.h
   modules/[category]/module_name.cpp
   ```

2. **Update build_modules.bat:**
   - Thêm vào list compile
   - Thêm vào list link

3. **Include trong main.cpp (nếu cần):**
   ```cpp
   #include "modules/[category]/module_name.h"
   ```

4. **Test compile:**
   ```bash
   batch\build_modules.bat > build
   ```

Xem chi tiết: [how_to_add_modules.md](how_to_add_modules.md)

---

## 🗂️ DATA FILES ORGANIZATION

### **Cấu trúc data/:**

```
data/
├── version.txt              # Version hiện tại (v77.1)
├── math_history.json        # Learning data (~60 problems)
├── ai_knowledge.json        # Pattern data (~70 entries)
│
├── knowledge/               # Knowledge Base (Phase 3)
│   ├── math.json
│   ├── physics.json
│   ├── chemistry.json
│   └── literature.json
│
├── import/                  # Import training data
│   ├── training_basic.txt
│   ├── training_data.csv
│   └── training_test.txt
│
├── export/                  # Export directory
│   └── math_history.csv
│
└── backup/
    └── math_history_backup.json
```

---

## 🔄 VERSION MANAGEMENT

### **Version Format:** `vMAJOR.MINOR`

**Major Version** (v77 → v78):
- Increment khi: Full rebuild
- Trigger: Command `build`

**Minor Version** (v77.1 → v77.2):
- Increment khi: Incremental build
- Trigger: Command `update`

### **Check version:**
```bash
# Method 1: Đọc file
type data\version.txt

# Method 2: Trong program
# Header sẽ hiển thị version khi run
```

---

## 🚨 TROUBLESHOOTING

### **Build lỗi:**
1. Check syntax errors trong code
2. Check missing includes
3. Thử full rebuild: `build_modules.bat > build`
4. Nếu vẫn lỗi → Rollback: `build_modules.bat > rollback`

### **Program crash:**
1. Check build_log.txt
2. Review code vừa thêm
3. Rollback về version stable
4. Debug từng module

### **Data corrupted:**
1. Check `data/backup/math_history_backup.json`
2. Restore từ backup
3. Re-import training data

---

## 📚 USEFUL COMMANDS REFERENCE

### **Trong chương trình:**

| Command | Chức năng |
|---------|-----------|
| `help` | Hiển thị tất cả lệnh |
| `calculator` | Xem khả năng toán học |
| `stats` | Thống kê sử dụng |
| `clear` | Xóa màn hình |
| `train` | Train AI từ history |
| `save` | Lưu AI knowledge |
| `load` | Load AI knowledge |
| `clean` | Xóa duplicates |
| `analyze` | Phân tích patterns |
| `suggest` | Gợi ý method |
| `import` | Import training data |
| `export` | Export sang CSV |
| `summary` | Tóm tắt data |
| `classify <problem>` | Phân tích problem |
| `exit` | Thoát chương trình |

---

**Happy Coding! 🚀**
