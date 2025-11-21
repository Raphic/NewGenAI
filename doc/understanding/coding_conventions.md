# 📝 CODING CONVENTIONS - Quy Ước Lập Trình

**Version:** v77.1  
**Last Updated:** 2025-11-21

---

## 🌍 NGÔN NGỮ (Language)

### **Quy tắc:**

| Thành phần | Ngôn ngữ | Ví dụ |
|------------|----------|-------|
| **Code** | English | `calculateAverage()`, `totalSum` |
| **Comments** | Tiếng Việt | `// Tính trung bình các số` |
| **UI/Output** | Tiếng Việt + Emoji | `"✅ Kết quả: 5"` |
| **Documentation** | Tiếng Việt | Tất cả .md files |
| **Variable names** | English | `userInput`, `resultValue` |
| **Function names** | English | `processData()`, `getValue()` |

---

## 📛 NAMING CONVENTIONS

### **1. Namespace**

```cpp
// ✅ GOOD: lowercase, underscore
namespace math_engine {
    // Code
}

namespace ai_trainer {
    // Code
}

// ❌ BAD
namespace MathEngine { }    // PascalCase - không dùng
namespace mathengine { }    // Không có separator
```

---

### **2. Class Names**

```cpp
// ✅ GOOD: PascalCase
class MathLogger { };
class PatternRecognizer { };
class VietnameseParser { };

// ❌ BAD
class mathLogger { };       // camelCase - không dùng
class math_logger { };      // snake_case - không dùng
```

---

### **3. Function Names**

```cpp
// ✅ GOOD: camelCase
void calculateExpression();
int getResultValue();
bool isValidInput();

// ❌ BAD
void CalculateExpression();  // PascalCase - không dùng
void calculate_expression(); // snake_case - không dùng
```

---

### **4. Variable Names**

```cpp
// ✅ GOOD: camelCase, descriptive
int totalSum = 0;
string userInput = "";
double averageValue = 0.0;
bool isValidExpression = false;

// ❌ BAD
int ts = 0;                 // Quá ngắn, không rõ nghĩa
int total_sum = 0;          // snake_case
int TotalSum = 0;           // PascalCase
```

---

### **5. Constants**

```cpp
// ✅ GOOD: UPPER_CASE
const int MAX_SIZE = 100;
const double PI = 3.14159;
const string DEFAULT_PATH = "data/";

// ❌ BAD
const int maxSize = 100;    // camelCase
const int max_size = 100;   // lowercase
```

---

### **6. Private Members**

```cpp
class Example {
private:
    // ✅ GOOD: Thêm prefix hoặc suffix
    int m_count;           // m_ prefix (member)
    string data_;          // _ suffix
    
    // Hoặc không prefix cũng được (tùy preference)
    int count;
    string data;
    
public:
    int publicValue;       // Public không cần prefix
};
```

---

## 📄 FILE ORGANIZATION

### **Header File (.h)**

```cpp
#ifndef MODULE_NAME_H
#define MODULE_NAME_H

// 1. System includes
#include <string>
#include <vector>

// 2. Project includes
#include "modules/utils/common.h"

// 3. Namespace (optional)
namespace module_name {

// 4. Forward declarations (nếu cần)
class OtherClass;

// 5. Structs/Enums
struct DataStructure {
    // Fields
};

enum class Status {
    SUCCESS,
    FAILURE
};

// 6. Class declaration
class ModuleName {
public:
    // Public methods
    ModuleName();
    ~ModuleName();
    
    void publicMethod();
    
private:
    // Private members
    int privateValue;
    
    // Private methods
    void privateMethod();
};

} // namespace module_name

#endif // MODULE_NAME_H
```

---

### **Implementation File (.cpp)**

```cpp
#include "module_name.h"

// Other includes nếu cần
#include <iostream>
#include <algorithm>

// Using statements (trong .cpp, không trong .h)
using std::string;
using std::vector;

namespace module_name {

// Implementation
ModuleName::ModuleName() {
    // Constructor
}

ModuleName::~ModuleName() {
    // Destructor
}

void ModuleName::publicMethod() {
    // Implementation
}

void ModuleName::privateMethod() {
    // Implementation
}

} // namespace module_name
```

---

## 💬 COMMENTS

### **1. Comment TẠI SAO, không phải LÀ GÌ**

```cpp
// ✅ GOOD: Giải thích lý do
// Dùng delta method vì có thể có nghiệm kép
float delta = b*b - 4*a*c;

// Normalize để so sánh không phân biệt hoa/thường
string normalized = toLowerCase(input);

// ❌ BAD: Chỉ mô tả code
// Calculate delta
float delta = b*b - 4*a*c;

// Convert to lowercase
string normalized = toLowerCase(input);
```

---

### **2. Function Comments**

```cpp
/**
 * Tính trung bình của một mảng số
 * 
 * @param values Mảng các số cần tính
 * @return Giá trị trung bình, trả về 0 nếu mảng rỗng
 */
double calculateAverage(const std::vector<double>& values) {
    if (values.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size();
}
```

---

### **3. TODO Comments**

```cpp
// TODO: Implement system equation solver
// TODO(khoa): Add error handling cho division by zero
// FIXME: Bug khi input có khoảng trắng thừa
// NOTE: Hàm này chỉ work với UTF-8 encoding
```

---

## 🎨 CODE STYLE

### **1. Indentation**

```cpp
// ✅ GOOD: 4 spaces (hoặc 1 tab)
void example() {
    if (condition) {
        doSomething();
    }
}

// ❌ BAD: Inconsistent spacing
void example() {
  if (condition) {
      doSomething();
  }
}
```

---

### **2. Braces**

```cpp
// ✅ GOOD: Opening brace cùng dòng
if (condition) {
    doSomething();
}

void function() {
    // Code
}

// ❌ BAD: Không dùng braces cho single line (dễ lỗi)
if (condition)
    doSomething();

// ✅ GOOD: Luôn dùng braces
if (condition) {
    doSomething();
}
```

---

### **3. Spacing**

```cpp
// ✅ GOOD: Space sau keywords, operators
if (x == 5) {
    int result = a + b;
}

for (int i = 0; i < 10; i++) {
    // Loop
}

// ❌ BAD
if(x==5){
    int result=a+b;
}

for(int i=0;i<10;i++){
}
```

---

### **4. Line Length**

```cpp
// ✅ GOOD: Giới hạn ~80-100 characters
string message = "This is a long message that needs "
                 "to be split into multiple lines";

// ❌ BAD: Quá dài
string message = "This is a very very very very very very long message that goes beyond 100 characters and is hard to read";
```

---

## 🔧 BEST PRACTICES

### **1. Function Length**

```cpp
// ✅ GOOD: Function ngắn, single responsibility
void processInput(string input) {
    auto parsed = parse(input);
    if (!validate(parsed)) return;
    auto result = calculate(parsed);
    display(result);
    log(input, result);
}

// ❌ BAD: Function quá dài (>50 lines)
void processInput(string input) {
    // 100 lines of code doing everything
}
```

---

### **2. Const Correctness**

```cpp
// ✅ GOOD: Dùng const khi có thể
double calculateSum(const std::vector<double>& values) {
    double sum = 0;
    for (const auto& value : values) {
        sum += value;
    }
    return sum;
}

// ❌ BAD: Không dùng const
double calculateSum(std::vector<double>& values) {
    double sum = 0;
    for (auto value : values) {
        sum += value;
    }
    return sum;
}
```

---

### **3. Error Handling**

```cpp
// ✅ GOOD: Check errors và handle
bool loadFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "❌ Lỗi: Không mở được file " << path << "\n";
        return false;
    }
    // Process file
    return true;
}

// ❌ BAD: Không check errors
void loadFile(const string& path) {
    ifstream file(path);
    // Assume file opened successfully
}
```

---

### **4. DRY (Don't Repeat Yourself)**

```cpp
// ✅ GOOD: Extract common code
string normalize(const string& text) {
    string result = toLowerCase(text);
    result = trim(result);
    return result;
}

string input1 = normalize(rawInput1);
string input2 = normalize(rawInput2);

// ❌ BAD: Duplicate code
string input1 = toLowerCase(rawInput1);
input1 = trim(input1);

string input2 = toLowerCase(rawInput2);
input2 = trim(input2);
```

---

## 🏗️ PROJECT-SPECIFIC CONVENTIONS

### **1. Module Structure**

```
modules/[category]/
    ├── module_name.h      # Header
    └── module_name.cpp    # Implementation
```

---

### **2. Namespace per Module**

```cpp
// Mỗi module có namespace riêng
namespace math {
    class Calculator { };
}

namespace ai {
    class MathLogger { };
}
```

---

### **3. Data Structures**

```cpp
// Struct cho data, class cho behavior
struct MathProblem {
    string input;
    string type;
    string result;
    bool success;
    time_t timestamp;
};

class MathLogger {
    void log(const MathProblem& problem);
};
```

---

### **4. Emoji Usage**

```cpp
// UI output: Dùng emoji cho visual
cout << "✅ Thành công!\n";
cout << "❌ Lỗi!\n";
cout << "📊 Thống kê:\n";
cout << "🎯 Mục tiêu:\n";
cout << "🧠 AI đã học:\n";

// Code comments: Không dùng emoji
// Tính tổng các phần tử (không dùng ✅ 🎯)
```

---

## ✅ CHECKLIST TRƯỚC KHI COMMIT

- [ ] Code follow naming conventions
- [ ] Comments bằng tiếng Việt, giải thích WHY
- [ ] Function length < 50 lines
- [ ] Dùng const khi có thể
- [ ] Error handling đầy đủ
- [ ] Không có code duplicate
- [ ] Braces format đúng
- [ ] Spacing consistent
- [ ] File có header guard (#ifndef)
- [ ] Namespace đúng format

---

**Next:** Xem [WORKFLOW.md](WORKFLOW.md) để biết cách build và test code.
