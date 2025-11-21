# 🔧 HOW TO ADD MODULES - Thêm Module C++ Mới

**Version:** v77.1  
**Last Updated:** 2025-11-21

---

## 📋 TỔNG QUAN

Module là đơn vị code độc lập trong NewGen AI System. Mỗi module có:
- ✅ Header file (.h)
- ✅ Implementation file (.cpp)
- ✅ Namespace riêng
- ✅ Chức năng cụ thể

---

## 🗂️ CẤU TRÚC MODULE

### **Phân loại modules:**

```
modules/
├── ai/       → AI & Learning modules
├── math/     → Mathematical operations
├── system/   → System infrastructure
└── utils/    → Common utilities
```

**Chọn category phù hợp:**
- **ai/**: Nếu liên quan đến learning, pattern, data processing
- **math/**: Nếu liên quan đến tính toán toán học
- **system/**: Nếu là core system functionality
- **utils/**: Nếu là utilities dùng chung

---

## 🚀 BƯỚC 1: TẠO FILES

### **Step 1.1: Tạo Header File**

**File:** `modules/[category]/module_name.h`

```cpp
#ifndef MODULE_NAME_H
#define MODULE_NAME_H

// 1. Includes
#include <string>
#include <vector>
// ... other includes

// 2. Namespace
namespace category_name {

// 3. Structs/Enums (nếu cần)
struct ModuleData {
    std::string value;
    int count;
};

// 4. Class Declaration
class ModuleName {
public:
    // Constructor & Destructor
    ModuleName();
    ~ModuleName();
    
    // Public methods
    void publicMethod();
    int getValue();
    
private:
    // Private members
    int privateValue;
    std::string data;
    
    // Private methods
    void privateHelper();
};

} // namespace category_name

#endif // MODULE_NAME_H
```

---

### **Step 1.2: Tạo Implementation File**

**File:** `modules/[category]/module_name.cpp`

```cpp
#include "module_name.h"

// Other includes
#include <iostream>
#include "../utils/common.h"

namespace category_name {

// Constructor
ModuleName::ModuleName() {
    privateValue = 0;
    data = "";
    
    // Initialize
    std::cout << "✅ ModuleName initialized\n";
}

// Destructor
ModuleName::~ModuleName() {
    // Cleanup nếu cần
}

// Public methods
void ModuleName::publicMethod() {
    // Implementation
    std::cout << "📌 Running publicMethod\n";
}

int ModuleName::getValue() {
    return privateValue;
}

// Private methods
void ModuleName::privateHelper() {
    // Helper implementation
}

} // namespace category_name
```

---

## 🔨 BƯỚC 2: CẬP NHẬT BUILD SYSTEM

### **File:** `batch/build_modules.bat`

Tìm section compile modules và thêm dòng mới:

```batch
echo [11/13] Compiling your_module.cpp...
g++ -c modules/category/your_module.cpp -o build/your_module.o
if %ERRORLEVEL% neq 0 (
    echo ❌ Lỗi compile your_module.cpp
    pause
    exit /b 1
)
```

**Lưu ý:**
- Tăng số thứ tự [11/13] → [11/14]
- Update tổng số modules
- Thêm vào đúng vị trí dependency order

---

### **Dependency Order:**

```
1. utils/common.cpp              (Không phụ thuộc gì)
2. ai/storage_json.cpp           (Depends: common)
3. ai/math_logger.cpp            (Depends: storage_json)
...
[YOUR MODULE HERE]               (Depends: ?)
...
n. main.cpp                      (Depends: tất cả)
```

---

## 🔗 BƯỚC 3: TÍCH HỢP VÀO MAIN.CPP

### **Option A: Module dùng trực tiếp trong main**

```cpp
// 1. Include header
#include "modules/category/your_module.h"

// 2. Khai báo namespace (optional)
using category_name::YourModule;

int main() {
    // 3. Initialize module
    YourModule module;
    
    // 4. Use module
    module.publicMethod();
    
    // 5. Thêm vào command nếu cần
    DEFINE_COMMAND(yourcommand, "Mô tả command") {
        module.publicMethod();
        return 1;
    }
}
```

---

### **Option B: Module dùng gián tiếp**

Nếu module chỉ được dùng bởi module khác (không dùng trực tiếp trong main):

```cpp
// Không cần include trong main.cpp
// Chỉ include trong module sử dụng nó

// File: modules/ai/some_module.cpp
#include "../category/your_module.h"

void SomeModule::useYourModule() {
    category_name::YourModule mod;
    mod.publicMethod();
}
```

---

## 🧪 BƯỚC 4: BUILD & TEST

### **Step 4.1: Full Rebuild**

```bash
batch\build_modules.bat
> build
```

**Kiểm tra:**
- ✅ Compile không có lỗi
- ✅ Link thành công
- ✅ Version tăng (v77.0 → v78.0)

---

### **Step 4.2: Test Module**

```cpp
// Trong main hoặc test function
YourModule module;
module.publicMethod();

// Check output
// Check behavior
```

---

## 📝 BƯỚC 5: DOCUMENTATION

### **Update files:**

1. **api_reference.md** - Thêm API documentation
```markdown
### YourModule

**Namespace:** `category_name`

**Constructor:**
\`\`\`cpp
YourModule();
\`\`\`

**Methods:**
- `void publicMethod()` - Mô tả chức năng
- `int getValue()` - Lấy giá trị
```

2. **PROJECT_REVIEW.md** - Update module count

3. **TODO.md** - Mark task as done

---

## 📚 VÍ DỤ THỰC TẾ

### **Example 1: Vietnamese Parser (Phase 3)**

#### **Files:**
```
modules/ai/nlp/vietnamese_parser.h
modules/ai/nlp/vietnamese_parser.cpp
```

#### **Header:**
```cpp
#ifndef VIETNAMESE_PARSER_H
#define VIETNAMESE_PARSER_H

#include <string>
#include <vector>

namespace ai {
namespace nlp {

class VietnameseParser {
public:
    VietnameseParser();
    
    // Parse tiếng Việt thành biểu thức toán
    std::string parseToMath(const std::string& input);
    
    // Kiểm tra có phải câu toán không
    bool isMathExpression(const std::string& input);
    
private:
    std::vector<std::string> tokenize(const std::string& text);
    std::string mapKeyword(const std::string& keyword);
};

} // namespace nlp
} // namespace ai

#endif
```

#### **Implementation:**
```cpp
#include "vietnamese_parser.h"
#include <sstream>

namespace ai {
namespace nlp {

VietnameseParser::VietnameseParser() {
    std::cout << "✅ VietnameseParser initialized\n";
}

std::string VietnameseParser::parseToMath(const std::string& input) {
    auto tokens = tokenize(input);
    std::ostringstream result;
    
    for (const auto& token : tokens) {
        std::string mapped = mapKeyword(token);
        result << mapped;
    }
    
    return result.str();
}

bool VietnameseParser::isMathExpression(const std::string& input) {
    // Check logic
    return true;
}

std::vector<std::string> VietnameseParser::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string VietnameseParser::mapKeyword(const std::string& keyword) {
    if (keyword == "cộng") return "+";
    if (keyword == "trừ") return "-";
    // ... more mappings
    return keyword;
}

} // namespace nlp
} // namespace ai
```

#### **Build system update:**
```batch
echo [13/14] Compiling vietnamese_parser.cpp...
g++ -c modules/ai/nlp/vietnamese_parser.cpp -o build/vietnamese_parser.o
```

#### **Usage in main.cpp:**
```cpp
#include "modules/ai/nlp/vietnamese_parser.h"
using ai::nlp::VietnameseParser;

int main() {
    VietnameseParser parser;
    
    std::string vietnameseInput = "tính 2 cộng 3";
    std::string mathExpr = parser.parseToMath(vietnameseInput);
    // mathExpr = "2+3"
    
    // Process với Calculator
    calc.process(mathExpr);
}
```

---

## ✅ CHECKLIST

### **Trước khi thêm module:**
- [ ] Xác định category phù hợp (ai/math/system/utils)
- [ ] Thiết kế interface (public methods)
- [ ] Xác định dependencies

### **Khi tạo module:**
- [ ] Tạo header file với header guard
- [ ] Tạo implementation file
- [ ] Sử dụng namespace
- [ ] Comments tiếng Việt
- [ ] Follow naming conventions

### **Build & Integration:**
- [ ] Update build_modules.bat
- [ ] Thêm vào đúng dependency order
- [ ] Include trong main.cpp (nếu cần)
- [ ] Full rebuild thành công
- [ ] Test module works

### **Documentation:**
- [ ] Update api_reference.md
- [ ] Update PROJECT_REVIEW.md
- [ ] Update TODO.md
- [ ] Thêm usage examples

---

## 🚨 COMMON MISTAKES

### **1. Quên Header Guard**
```cpp
// ❌ BAD
#ifndef VIETNAMESE_PARSER_H
#define VIETNAMESE_PARSER_H
// Code...
// QUÊN #endif

// ✅ GOOD
#ifndef VIETNAMESE_PARSER_H
#define VIETNAMESE_PARSER_H
// Code...
#endif
```

### **2. Sai Dependency Order**
```batch
# ❌ BAD: Compile your_module trước dependency
g++ -c your_module.cpp -o your_module.o
g++ -c dependency.cpp -o dependency.o

# ✅ GOOD: Compile dependency trước
g++ -c dependency.cpp -o dependency.o
g++ -c your_module.cpp -o your_module.o
```

### **3. Namespace Không Khớp**
```cpp
// ❌ BAD
// Header: namespace ai {
// Impl: namespace math {  // Sai!

// ✅ GOOD
// Header: namespace ai {
// Impl: namespace ai {    // Đúng!
```

---

## 📚 TÀI LIỆU LIÊN QUAN

- [CODING_CONVENTIONS.md](CODING_CONVENTIONS.md) - Code style
- [WORKFLOW.md](WORKFLOW.md) - Build process
- [ARCHITECTURE.md](ARCHITECTURE.md) - System structure
- [phase3_guide.md](phase3_guide.md) - Phase 3 examples

---

**Happy Coding! 🚀**
