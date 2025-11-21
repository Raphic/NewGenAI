# 🏗️ ARCHITECTURE - Kiến Trúc Hệ Thống

**Version:** v77.1  
**Last Updated:** 2025-11-21  
**Type:** Modular C++ Local AI System

---

## 📊 TỔNG QUAN KIẾN TRÚC

### **Phân lớp hệ thống:**

```
┌─────────────────────────────────────────────────────────────┐
│                      USER INTERFACE                         │
│                  (Console - Vietnamese)                     │
│                      main.cpp                               │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                    COMMAND LAYER                            │
│            CommandRegistry + CommandHandler                 │
│          (14 commands: help, train, import, etc.)           │
└─────────────────────────────────────────────────────────────┘
                            ↓
         ┌──────────────────┴──────────────────┐
         ↓                                     ↓
┌──────────────────────┐           ┌──────────────────────┐
│     AI LAYER         │           │    MATH LAYER        │
│                      │           │                      │
│  • MathLogger        │←─────────→│  • Calculator        │
│  • PatternRecognizer │   uses    │  • Statistics        │
│  • AITrainer         │           │                      │
│  • DataImporter      │           └──────────────────────┘
│  • DataExporter      │                    ↓
│  • DataCleaner       │                    │
│                      │                    ↓
└──────────────────────┘           ┌──────────────────────┐
         ↓                         │   STORAGE LAYER      │
         ↓                         │                      │
┌──────────────────────┐           │  • JsonStorage       │
│  STORAGE LAYER       │           │  • File I/O          │
│                      │           │                      │
│  • ai_knowledge.json │           │  Data Files:         │
│  • math_history.json │           │  - version.txt       │
│  • knowledge/*.json  │           │  - *.json            │
│                      │           │  - backups/          │
└──────────────────────┘           └──────────────────────┘
         ↓                                  ↓
┌─────────────────────────────────────────────────────────────┐
│                     UTILS LAYER                             │
│         common.cpp (UTF-8, console, helpers)                │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔄 LUỒNG XỬ LÝ (Data Flow)

### **Flow 1: Math Calculation**

```
User Input: "x^2-5x+6=0"
     ↓
main.cpp (receive input)
     ↓
Check if is command? → No
     ↓
PatternRecognizer.classify(input)
     ↓ (classify as QUADRATIC)
Calculator.process(input)
     ↓ (solve equation)
Result: "x = 2 hoặc x = 3"
     ↓
MathLogger.log(input, result, true)
     ↓ (save to JSON)
Display result to user
     ↓
Suggest hint (PatternRecognizer)
```

---

### **Flow 2: Command Execution**

```
User Input: "train"
     ↓
main.cpp (receive input)
     ↓
CommandRegistry.find("train") → Command found
     ↓
Execute command lambda
     ↓
AITrainer.train()
     ├─ Get history from MathLogger
     ├─ PatternRecognizer.learnFromHistory()
     ├─ Build statistics
     └─ Save to ai_knowledge.json
     ↓
Display success message
```

---

### **Flow 3: Data Import**

```
User Input: "import"
     ↓
DataImporter.smartImport()
     ├─ Scan data/import/ folder
     ├─ Show file list
     ├─ Read imported_files.log
     └─ Display menu
     ↓
User selects file
     ↓
Read file (CSV/TXT)
     ├─ Parse problems
     ├─ Check duplicates (normalize)
     └─ Filter new problems only
     ↓
For each problem:
     ├─ Calculator.process(problem)
     ├─ MathLogger.log(problem, result)
     └─ Count imported
     ↓
Update imported_files.log
     ↓
Display summary
```

---

## 📁 MODULE ORGANIZATION

### **Cấu trúc thư mục:**

```
modules/
├── ai/                          # AI & Machine Learning
│   ├── ai_trainer.*             # Training orchestration
│   ├── data_cleaner.*           # Duplicate cleanup
│   ├── data_exporter.*          # CSV export
│   ├── data_importer.*          # CSV/TXT import
│   ├── math_logger.*            # History logging
│   ├── pattern_recognizer.*    # Pattern classification
│   ├── storage_json.*           # JSON I/O
│   ├── math_problem.h           # Data structures
│   └── nlp/                     # Phase 3: NLP (empty)
│
├── math/                        # Mathematical Engine
│   ├── calculator.*             # Expression evaluator
│   └── statistics.*             # Statistical calculations
│
├── system/                      # System Infrastructure
│   ├── command_handler.*        # Command dispatcher
│   └── command_registry.*       # Dynamic registration
│
└── utils/                       # Utilities
    └── common.*                 # Console UTF-8, helpers
```

---

## 🗄️ DATA ARCHITECTURE

### **Cấu trúc data:**

```
data/
├── version.txt                  # Current version (v77.1)
│
├── math_history.json            # Learning data (JSONL format)
│   └── Format: {input, type, result, success, timestamp}
│
├── ai_knowledge.json            # AI patterns (custom format)
│   └── Format: success_flag "problem_text"
│
├── knowledge/                   # Knowledge Base (Phase 3)
│   ├── math.json                # 4 topics: quadratic, sine, circle, derivative
│   ├── physics.json             # 4 topics: Newton, velocity, Ohm, heat
│   ├── chemistry.json           # 4 topics: atom, alkane, acid/base, combustion
│   └── literature.json          # 2 topics: quatrain, novel
│
├── import/                      # Training data import
│   ├── training_basic.txt
│   ├── training_data.csv
│   └── training_test.txt
│
├── export/                      # CSV export directory
│   └── math_history.csv
│
└── backup/                      # Data backups
    └── math_history_backup.json
```

---

## 🔌 DEPENDENCIES (Phụ thuộc)

### **Dependency Graph:**

```
main.cpp
  ↓
  ├─→ modules/math/calculator.h
  │     └─→ modules/ai/math_logger.h
  │           └─→ modules/ai/storage_json.h
  │                 └─→ modules/utils/common.h
  │
  ├─→ modules/ai/pattern_recognizer.h
  │     └─→ modules/ai/math_problem.h
  │           └─→ modules/ai/storage_json.h
  │
  ├─→ modules/ai/data_importer.h
  │     └─→ modules/ai/math_logger.h
  │
  ├─→ modules/ai/data_exporter.h
  │     └─→ modules/ai/math_logger.h
  │
  ├─→ modules/ai/data_cleaner.h
  │     └─→ modules/ai/math_logger.h
  │
  ├─→ modules/ai/ai_trainer.h
  │     ├─→ modules/ai/pattern_recognizer.h
  │     └─→ modules/ai/math_logger.h
  │
  ├─→ modules/system/command_handler.h
  │
  └─→ modules/system/command_registry.h
```

### **Build Order:**

```
1. utils/common.cpp              (No dependencies)
2. ai/storage_json.cpp           (Depends on: common)
3. ai/math_logger.cpp            (Depends on: storage_json)
4. ai/pattern_recognizer.cpp     (Depends on: storage_json)
5. ai/data_importer.cpp          (Depends on: math_logger)
6. ai/data_exporter.cpp          (Depends on: math_logger)
7. ai/data_cleaner.cpp           (Depends on: math_logger)
8. ai/ai_trainer.cpp             (Depends on: pattern_recognizer, math_logger)
9. math/calculator.cpp           (Depends on: math_logger)
10. math/statistics.cpp          (No dependencies)
11. system/command_handler.cpp   (No dependencies)
12. system/command_registry.cpp  (No dependencies)
13. main.cpp                     (Depends on: ALL)
```

---

## 🎯 DESIGN PATTERNS

### **1. Command Pattern**
```cpp
// Command Registry tự động
DEFINE_COMMAND(help, "Hiển thị danh sách lệnh") {
    // Implementation
    return 1;
}

// Register tự động qua macro
CommandRegistry::instance().registerCommand("help", helpCommand, "Hiển thị...");
```

---

### **2. Dependency Injection**
```cpp
// Calculator nhận logger qua constructor
Calculator calc(logger);

// Lợi ích:
// - Dễ test (inject mock)
// - Loose coupling
```

---

### **3. Singleton Pattern**
```cpp
// CommandRegistry is singleton
CommandRegistry& registry = CommandRegistry::instance();
```

---

### **4. Strategy Pattern (Future - Phase 3)**
```cpp
// Different parsers for different languages
LanguageParser* parser;
if (lang == "vi") {
    parser = new VietnameseParser();
} else {
    parser = new EnglishParser();
}
```

---

## 🚀 ROADMAP ARCHITECTURE

### **Phase 1-2: COMPLETED** ✅

```
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│   User      │────→│  Calculator  │────→│ MathLogger  │
│  (Console)  │     │              │     │  (JSON)     │
└─────────────┘     └──────────────┘     └─────────────┘
                           ↓                     ↓
                    ┌──────────────┐     ┌─────────────┐
                    │   Pattern    │────→│ AI          │
                    │ Recognizer   │     │ Knowledge   │
                    └──────────────┘     └─────────────┘
```

---

### **Phase 3: PLANNED** 🚧

```
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│   User      │────→│  Vietnamese  │────→│ Calculator  │
│ (Tiếng Việt)│     │   Parser     │     │             │
└─────────────┘     └──────────────┘     └─────────────┘
      ↓                                          ↓
┌──────────────┐                         ┌─────────────┐
│ Intent       │                         │ Knowledge   │
│ Classifier   │                         │ Base        │
└──────────────┘                         └─────────────┘
      ↓                                          ↓
┌──────────────┐                         ┌─────────────┐
│ Response     │                         │ Topics      │
│ Generator    │←────────────────────────│ (JSON)      │
└──────────────┘                         └─────────────┘
```

---

### **Phase 4-6: FUTURE** 🔮

```
┌──────────────┐     ┌──────────────┐     ┌─────────────┐
│ Reasoning    │────→│  ML Models   │────→│ Distributed │
│ Engine (CBR) │     │  (LSTM/etc)  │     │ Learning    │
└──────────────┘     └──────────────┘     └─────────────┘
      ↓                     ↓                     ↓
  Similarity          Feature Vector         Network Sync
   Search             Extraction             Multi-node
```

---

## 📊 STATISTICS

### **Codebase:**
- **C++ Source:** 12 files (~2,500 lines)
- **Headers:** 12 files (~800 lines)
- **Main:** 239 lines
- **Build System:** 791 lines
- **Total:** ~4,330 lines C++

### **Modules:**
- **AI Layer:** 7 modules
- **Math Layer:** 2 modules
- **System Layer:** 2 modules
- **Utils Layer:** 1 module
- **Total:** 12 modules

### **Data:**
- **Math History:** ~60 problems
- **AI Knowledge:** ~70 patterns
- **Knowledge Base:** 4 domains, 14 topics (not integrated yet)

---

## 🎨 ASCII DIAGRAM

```
                    NewGen AI System v77.1
                    
┌─────────────────────────────────────────────────────────┐
│                      MAIN.CPP                           │
│  ┌───────────┐  ┌───────────┐  ┌───────────┐          │
│  │ Commands  │  │   Input   │  │  Output   │          │
│  │ (14 cmds) │  │  Handler  │  │ (UTF-8)   │          │
│  └─────┬─────┘  └─────┬─────┘  └─────┬─────┘          │
└────────┼──────────────┼──────────────┼────────────────┘
         │              │              │
         ↓              ↓              ↓
    ┌────────────────────────────────────────┐
    │         COMMAND REGISTRY               │
    │    (Dynamic command registration)      │
    └────────────────────────────────────────┘
         ↓                             ↓
    ┌──────────┐                 ┌──────────┐
    │ AI Layer │                 │Math Layer│
    └────┬─────┘                 └────┬─────┘
         │                            │
    ┌────┴──────────────────────┬────┴─────┐
    │                           │          │
┌───┴────┐  ┌──────────┐  ┌────┴───┐  ┌──┴────┐
│ Logger │  │ Pattern  │  │  Calc  │  │ Stats │
│        │  │Recognizer│  │        │  │       │
└───┬────┘  └─────┬────┘  └────┬───┘  └───────┘
    │             │            │
    └─────────────┴────────────┘
                  │
            ┌─────┴─────┐
            │  Storage  │
            │   (JSON)  │
            └───────────┘
```

---

**Xem thêm:**
- [PROJECT_REVIEW.md](PROJECT_REVIEW.md) - Phân tích chi tiết
- [WORKFLOW.md](WORKFLOW.md) - Build process
- [phase3_guide.md](phase3_guide.md) - Phase 3 details
