# HƯỚNG DẪN CHI TIẾT PHASE 3 - Dành cho người mới học C++

## 📚 MỤC LỤC

1. [Tổng quan Architecture](#tổng-quan-architecture)
2. [Component 1: Tokenizer](#component-1-tokenizer)
3. [Component 2: Keyword Mapper](#component-2-keyword-mapper)
4. [Component 3: Vietnamese Parser](#component-3-vietnamese-parser)
5. [Component 4: Knowledge Base](#component-4-knowledge-base)
6. [Component 5: Intent Classifier](#component-5-intent-classifier)
7. [Component 6: Response Generator](#component-6-response-generator)
8. [Component 7: Conversation Manager](#component-7-conversation-manager)
9. [Tích hợp vào Main](#tích-hợp-vào-main)

---

## 🏗️ TỔNG QUAN ARCHITECTURE

### Luồng xử lý mới (Phase 3):

```
┌─────────────────────────────────────────────────────────────┐
│  User Input: "giải thích phương trình bậc 2"                │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│  1. TOKENIZER                                               │
│  "giải thích phương trình bậc 2"                            │
│  → ["giải", "thích", "phương", "trình", "bậc", "2"]        │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│  2. INTENT CLASSIFIER                                       │
│  Detect keywords: "giải thích" → Intent = EXPLAIN          │
│  Detect domain: "phương trình" → Domain = MATH             │
└─────────────────────────────────────────────────────────────┘
                            ↓
            ┌───────────────┴────────────────┐
            ↓                                ↓
┌─────────────────────┐          ┌─────────────────────────┐
│  Intent: CALCULATE  │          │  Intent: EXPLAIN/DEFINE │
│                     │          │                         │
│  Vietnamese Parser  │          │  Knowledge Base         │
│  "2 cộng 3" → "2+3" │          │  Search topic:          │
│        ↓            │          │  "phương trình bậc 2"   │
│   Calculator        │          │        ↓                │
│   Result: 5         │          │  Topic found!           │
└─────────────────────┘          └─────────────────────────┘
            │                                ↓
            └───────────────┬────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│  3. RESPONSE GENERATOR                                      │
│  Format response với template đẹp                           │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│  4. CONVERSATION MANAGER                                    │
│  Lưu lịch sử: User said X, Bot replied Y                   │
└─────────────────────────────────────────────────────────────┘
                            ↓
                    Output to User
```

---

## 📝 COMPONENT 1: TOKENIZER

### Tại sao cần Tokenizer?

**Câu hỏi:** Tại sao không xử lý trực tiếp chuỗi mà phải tách từ?

**Trả lời:** 
- Computer không hiểu ngôn ngữ tự nhiên
- Phải tách thành các đơn vị nhỏ (tokens) để phân tích
- Ví dụ: "tính 2+3" có 3 phần ý nghĩa: ["tính", "2+3"]

### Code mẫu:

```cpp
// tokenizer.h
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

class Tokenizer {
public:
    // Tách câu thành các từ
    std::vector<std::string> tokenize(const std::string& text);
    
    // Chuẩn hóa: lowercase, trim spaces
    std::string normalize(const std::string& text);
    
private:
    // Helper: Kiểm tra ký tự có phải delimiter không
    bool isDelimiter(char c);
};

#endif
```

```cpp
// tokenizer.cpp
#include "tokenizer.h"
#include <algorithm>
#include <cctype>
#include <sstream>

std::vector<std::string> Tokenizer::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string normalized = normalize(text);
    
    // Tách bằng stringstream (tách theo khoảng trắng)
    std::istringstream iss(normalized);
    std::string token;
    while (iss >> token) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

std::string Tokenizer::normalize(const std::string& text) {
    std::string result = text;
    
    // Chuyển lowercase
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    // Loại bỏ dấu câu không cần thiết (giữ lại +, -, *, /, =, ^)
    // Bạn có thể customize theo nhu cầu
    
    return result;
}

bool Tokenizer::isDelimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}
```

### Giải thích code:

1. **`tokenize()`**: 
   - Input: Chuỗi gốc
   - Output: Vector các từ
   - Dùng `std::istringstream` để tách theo khoảng trắng

2. **`normalize()`**:
   - Chuyển chữ hoa → chữ thường để dễ so sánh
   - Ví dụ: "TÍNH" và "tính" đều thành "tính"

3. **`std::transform()`**: 
   - Là hàm STL duyệt qua từng ký tự và chuyển đổi
   - `std::tolower()`: Chuyển thành chữ thường

### Test:
```cpp
Tokenizer tok;
auto tokens = tok.tokenize("Tính 2 Cộng 3");
// Result: ["tính", "2", "cộng", "3"]
```

---

## 🗺️ COMPONENT 2: KEYWORD MAPPER

### Tại sao cần Keyword Mapper?

**Mục đích:** Chuyển từ tiếng Việt → ký hiệu toán học

**Ví dụ:**
- "cộng" → "+"
- "trừ" → "-"
- "nhân" → "*"
- "bình phương" → "^2"

### Code mẫu:

```cpp
// keyword_mapper.h
#ifndef KEYWORD_MAPPER_H
#define KEYWORD_MAPPER_H

#include <string>
#include <map>
#include <vector>

class KeywordMapper {
public:
    KeywordMapper();
    
    // Map một từ khóa sang ký hiệu
    std::string map(const std::string& keyword);
    
    // Thêm mapping mới
    void addMapping(const std::string& keyword, const std::string& symbol);
    
    // Load mappings cho một domain
    void loadDomain(const std::string& domain);
    
private:
    std::map<std::string, std::string> mappings;
    void initDefaultMappings(); // Load mappings mặc định
};

#endif
```

```cpp
// keyword_mapper.cpp
#include "keyword_mapper.h"

KeywordMapper::KeywordMapper() {
    initDefaultMappings();
}

std::string KeywordMapper::map(const std::string& keyword) {
    auto it = mappings.find(keyword);
    if (it != mappings.end()) {
        return it->second;
    }
    return keyword; // Không tìm thấy → trả về nguyên gốc
}

void KeywordMapper::addMapping(const std::string& keyword, const std::string& symbol) {
    mappings[keyword] = symbol;
}

void KeywordMapper::loadDomain(const std::string& domain) {
    // TODO: Load từ file JSON theo domain
    // Ví dụ: data/language/math_keywords.json
}

void KeywordMapper::initDefaultMappings() {
    // Toán học cơ bản
    mappings["cộng"] = "+";
    mappings["thêm"] = "+";
    mappings["plus"] = "+";
    
    mappings["trừ"] = "-";
    mappings["bớt"] = "-";
    mappings["minus"] = "-";
    
    mappings["nhân"] = "*";
    mappings["multiply"] = "*";
    
    mappings["chia"] = "/";
    mappings["divide"] = "/";
    
    mappings["bình phương"] = "^2";
    mappings["mũ"] = "^";
    mappings["power"] = "^";
    
    mappings["căn bậc hai"] = "sqrt";
    mappings["căn"] = "sqrt";
    
    mappings["bằng"] = "=";
    mappings["equals"] = "=";
    
    // Lượng giác
    mappings["sin"] = "sin";
    mappings["cos"] = "cos";
    mappings["tan"] = "tan";
}
```

### Giải thích code:

1. **`std::map<string, string>`**: 
   - Là dictionary trong C++
   - Key: từ khóa tiếng Việt ("cộng")
   - Value: ký hiệu ("+")

2. **`initDefaultMappings()`**:
   - Hardcode các mapping cơ bản
   - Support nhiều cách nói: "cộng", "thêm", "plus" đều → "+"

3. **`map()`**:
   - Tìm keyword trong mappings
   - Nếu tìm thấy → trả về ký hiệu
   - Nếu không → trả về nguyên gốc (có thể là số hoặc biến)

### Test:
```cpp
KeywordMapper mapper;
std::cout << mapper.map("cộng");  // "+"
std::cout << mapper.map("x");     // "x" (không có trong map)
```

---

## 🔄 COMPONENT 3: VIETNAMESE PARSER

### Chức năng chính:

Kết hợp Tokenizer + KeywordMapper để chuyển câu tiếng Việt → biểu thức toán

### Code mẫu:

```cpp
// vietnamese_parser.h
#ifndef VIETNAMESE_PARSER_H
#define VIETNAMESE_PARSER_H

#include "tokenizer.h"
#include "keyword_mapper.h"
#include <string>

class VietnameseParser {
public:
    VietnameseParser();
    
    // Parse câu tiếng Việt sang biểu thức toán
    std::string parseToMathExpression(const std::string& input);
    
    // Kiểm tra có phải câu toán không
    bool isMathExpression(const std::string& input);
    
private:
    Tokenizer tokenizer;
    KeywordMapper mapper;
    
    // Xử lý các pattern đặc biệt
    std::string handleSpecialPatterns(const std::vector<std::string>& tokens);
    
    // Detect phương trình (có dấu "=")
    bool isEquation(const std::vector<std::string>& tokens);
};

#endif
```

```cpp
// vietnamese_parser.cpp
#include "vietnamese_parser.h"
#include <sstream>

VietnameseParser::VietnameseParser() {
    // Constructor tự động init tokenizer và mapper
}

std::string VietnameseParser::parseToMathExpression(const std::string& input) {
    // Bước 1: Tokenize
    auto tokens = tokenizer.tokenize(input);
    
    // Bước 2: Xử lý các pattern đặc biệt trước
    // Ví dụ: "x bình phương" → phải kết hợp thành "x^2"
    std::string processed = handleSpecialPatterns(tokens);
    if (!processed.empty()) {
        return processed;
    }
    
    // Bước 3: Map từng token
    std::ostringstream result;
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string token = tokens[i];
        
        // Bỏ qua từ "tính", "giải"
        if (token == "tính" || token == "giải" || token == "solve") {
            continue;
        }
        
        // Map keyword
        std::string mapped = mapper.map(token);
        result << mapped;
        
        // Không thêm space nếu là operator
        if (mapped != "+" && mapped != "-" && mapped != "*" && 
            mapped != "/" && mapped != "=" && mapped != "^") {
            // result << " "; // Tùy chọn: có cần space không?
        }
    }
    
    return result.str();
}

bool VietnameseParser::isMathExpression(const std::string& input) {
    auto tokens = tokenizer.tokenize(input);
    
    // Check có chứa từ khóa toán không
    for (const auto& token : tokens) {
        std::string mapped = mapper.map(token);
        if (mapped == "+" || mapped == "-" || mapped == "*" || 
            mapped == "/" || mapped == "=" || mapped == "^") {
            return true;
        }
        // Hoặc có số
        if (std::isdigit(token[0])) {
            return true;
        }
    }
    
    return false;
}

std::string VietnameseParser::handleSpecialPatterns(
    const std::vector<std::string>& tokens) {
    
    std::ostringstream result;
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string token = tokens[i];
        
        // Pattern: "x bình phương" → "x^2"
        if (i + 2 < tokens.size() && 
            tokens[i+1] == "bình" && tokens[i+2] == "phương") {
            result << token << "^2";
            i += 2; // Skip "bình phương"
            continue;
        }
        
        // Pattern: "x mũ 2" → "x^2"
        if (i + 2 < tokens.size() && tokens[i+1] == "mũ") {
            result << token << "^" << tokens[i+2];
            i += 2;
            continue;
        }
        
        // Bỏ qua "tính", "giải"
        if (token == "tính" || token == "giải") {
            continue;
        }
        
        // Map thông thường
        result << mapper.map(token);
    }
    
    std::string res = result.str();
    return res.empty() ? "" : res;
}

bool VietnameseParser::isEquation(const std::vector<std::string>& tokens) {
    for (const auto& token : tokens) {
        if (token == "bằng" || token == "=") {
            return true;
        }
    }
    return false;
}
```

### Giải thích code:

1. **`parseToMathExpression()`**:
   - Tokenize → Handle special patterns → Map từng token
   - Bỏ qua từ không cần thiết ("tính", "giải")

2. **`handleSpecialPatterns()`**:
   - Xử lý patterns nhiều từ: "x bình phương" phải nhận biết là 1 cụm
   - Dùng look-ahead: Check token[i+1], token[i+2]

3. **`isMathExpression()`**:
   - Detect có phải câu toán không
   - Check có số hoặc operator

### Test:
```cpp
VietnameseParser parser;

std::string expr1 = parser.parseToMathExpression("tính 2 cộng 3");
// Result: "2+3"

std::string expr2 = parser.parseToMathExpression("x bình phương trừ 5x cộng 6 bằng 0");
// Result: "x^2-5x+6=0"
```

---

## 📚 COMPONENT 4: KNOWLEDGE BASE

### Tại sao cần Knowledge Base?

**Mục đích:** Lưu trữ kiến thức để trả lời câu hỏi như:
- "Định nghĩa phương trình bậc 2"
- "Giải thích định luật Newton"
- "Sin là gì?"

### Thiết kế Schema - MỞ RỘNG CHO TẤT CẢ MÔN HỌC:

```cpp
// knowledge_base.h
#ifndef KNOWLEDGE_BASE_H
#define KNOWLEDGE_BASE_H

#include <string>
#include <vector>
#include <map>
#include "../../ai/storage_json.h" // Reuse JSON storage

namespace knowledge {

// Cấu trúc Topic
struct Topic {
    std::string id;              // "math_quadratic_equation"
    std::string domain;          // "math", "physics", "chemistry", etc.
    std::string category;        // "algebra", "mechanics", "organic", etc.
    std::string name;            // "phương trình bậc 2"
    std::vector<std::string> aliases;  // ["pt bậc 2", "quadratic"]
    std::string definition;      // Định nghĩa ngắn
    std::string formula;         // Công thức (nếu có)
    std::string explanation;     // Giải thích chi tiết
    std::vector<std::string> examples;        // Ví dụ
    std::vector<std::string> related_topics;  // Topics liên quan
    std::map<std::string, std::string> metadata;  // Thông tin bổ sung
};

class KnowledgeBase {
public:
    KnowledgeBase();
    
    // Load một domain (môn học)
    bool loadDomain(const std::string& domain);
    
    // Save domain
    bool saveDomain(const std::string& domain);
    
    // Thêm topic
    void addTopic(const Topic& topic);
    
    // Tìm topic theo tên/alias
    Topic* findTopic(const std::string& query, const std::string& domain = "");
    
    // Search theo keyword
    std::vector<Topic*> search(const std::string& keyword);
    
    // Lấy danh sách domains hiện có
    std::vector<std::string> getAvailableDomains();
    
    // Kiểm tra domain đã load chưa
    bool isDomainLoaded(const std::string& domain);
    
private:
    std::map<std::string, std::vector<Topic>> database;
    std::string dataPath = "data/knowledge/";
    
    // Helper parse JSON
    Topic parseTopicFromJson(const std::string& jsonStr);
    std::string topicToJson(const Topic& topic);
};

} // namespace knowledge

#endif
```

### Giải thích schema:

**Topic Structure:**
```cpp
struct Topic {
    std::string id;  // Unique ID: "math_algebra_quadratic"
                     // Format: domain_category_name
                     
    std::string domain;  // Môn học: "math", "physics", "chemistry",
                        // "literature", "geography", "history", "english"
                        
    std::string category;  // Phân loại trong môn:
                          // Math: "algebra", "geometry", "calculus", "trigonometry"
                          // Physics: "mechanics", "thermodynamics", "electricity"
                          // Chemistry: "organic", "inorganic", "physical"
                          
    std::string name;  // Tên chính: "phương trình bậc 2"
    
    std::vector<std::string> aliases;  // Các cách gọi khác:
                                       // ["pt bậc 2", "phương trình bậc hai", 
                                       //  "quadratic equation"]
                                       // → Giúp search tốt hơn
                                       
    std::string formula;  // Công thức toán học (nếu có)
                         // Dùng UTF-8: "x = (-b ± √Δ) / 2a"
                         
    std::string explanation;  // Giải thích chi tiết
    
    std::vector<std::string> examples;  // Ví dụ minh họa
    
    std::vector<std::string> related_topics;  // Topics liên quan
                                              // → Gợi ý "Xem thêm"
                                              
    std::map<std::string, std::string> metadata;  // Thông tin thêm:
                                                  // {"difficulty": "medium",
                                                  //  "grade_level": "9"}
};
```

---

Tôi sẽ tiếp tục tạo các files mẫu JSON và code implementation. Bạn đã hiểu phần này chưa hay cần tôi giải thích thêm điểm nào?
