# 🎯 TODO - NewGen AI Project

**Last Updated:** 2025-11-21  
**Current Phase:** Phase 3 Preparation  
**Current Version:** v77.1

---

## 🔥 IN PROGRESS (Đang làm)

- [/] Reorganize documentation structure
  - [x] Create README.md navigation hub
  - [x] Create TODO.md
  - [ ] Create WORKFLOW.md
  - [ ] Create THINKING_PROCESS.md
  - [ ] Create other documentation files
  - [ ] Delete obsolete files

---

## 📋 NEXT UP (Việc tiếp theo)

### **Cleanup Tasks** (Dọn dẹp)
- [ ] Delete empty folders:
  - [ ] `modules/data/` (thư mục rỗng)
  - [ ] `modules/unified/` (thư mục rỗng)
- [ ] Delete unused file: `data/config.json`
- [ ] Clean old backups trong `batch/backup/` (giữ lại 20 bản mới nhất)

---

### **Phase 3: Language Module** (Module ngôn ngữ)

#### **3.1. Knowledge Base System**
- [ ] Tạo folder `modules/knowledge/`
- [ ] Implement `KnowledgeBase` class
  - [ ] `knowledge_base.h` - Header file
  - [ ] `knowledge_base.cpp` - Implementation
  - [ ] Load JSON files từ `data/knowledge/`
  - [ ] Search functionality
  - [ ] Topic lookup
- [ ] Test loading existing JSON files:
  - [ ] math.json
  - [ ] physics.json
  - [ ] chemistry.json
  - [ ] literature.json

#### **3.2. Vietnamese NLP Parser**
- [ ] Tạo folder `modules/ai/nlp/`
- [ ] Implement `Tokenizer`
  - [ ] `tokenizer.h/cpp`
  - [ ] Tách câu thành tokens
  - [ ] Normalize text (lowercase, trim)
- [ ] Implement `KeywordMapper`
  - [ ] `keyword_mapper.h/cpp`
  - [ ] Map từ tiếng Việt → ký hiệu toán
  - [ ] Support aliases
- [ ] Implement `VietnameseParser`
  - [ ] `vietnamese_parser.h/cpp`
  - [ ] Parse "tính 2 cộng 3" → "2+3"
  - [ ] Handle special patterns ("x bình phương" → "x^2")
- [ ] Test với Vietnamese input

#### **3.3. Context Analyzer** (Ngữ cảnh)
- [ ] Implement `ContextAnalyzer`
  - [ ] `context_analyzer.h/cpp`
  - [ ] Remember variables từ câu trước
  - [ ] Xử lý đại từ ("nó", "kết quả đó")
  - [ ] Context memory

#### **3.4. Conversation System**
- [ ] Implement `IntentClassifier`
  - [ ] Detect intent: CALCULATE, EXPLAIN, DEFINE, etc.
  - [ ] Detect domain: MATH, PHYSICS, etc.
- [ ] Implement `ResponseGenerator`
  - [ ] Format responses đẹp
  - [ ] Template system
- [ ] Implement `ConversationManager`
  - [ ] Lưu history hội thoại
  - [ ] Session management

#### **3.5. Integration**
- [ ] Update `main.cpp`
  - [ ] Thêm language preprocessing layer
  - [ ] Wire up all Phase 3 components
- [ ] Update `build_modules.bat`
  - [ ] Compile các module mới
- [ ] End-to-end testing

---

## 🚧 BLOCKED / WAITING (Đang chờ)

- None currently

---

## ✅ RECENTLY COMPLETED (Hoàn thành gần đây)

### **2025-11-21**
- [x] Created comprehensive PROJECT_REVIEW.md (871 dòng)
- [x] Created language_module_plan.md
- [x] Reviewed entire codebase structure
- [x] Started documentation reorganization

### **Before 2025-11-21 (Phase 1-2)**
- [x] Build System với version management
- [x] Backup system (473 versions)
- [x] MathLogger implementation
- [x] PatternRecognizer with 8 categories
- [x] DataImporter/Exporter
- [x] 14 commands trong main.cpp
- [x] JSON Knowledge Base schemas (4 domains, 14 topics)

---

## 💡 IDEAS / FUTURE ENHANCEMENTS (Ý tưởng tương lai)

### **Phase 4: Reasoning Engine**
- [ ] Case-Based Reasoning
- [ ] Similarity search trong history
- [ ] Predict results từ cases tương tự

### **Phase 5: ML Capabilities**
- [ ] Lightweight ML models
- [ ] Feature vector extraction
- [ ] Model training pipeline

### **Phase 6: Distributed Learning**
- [ ] Network sync layer
- [ ] Centralized database
- [ ] REST/gRPC API
- [ ] Multi-node learning

### **Code Quality Improvements**
- [ ] Add unit tests
- [ ] Refactor long functions
- [ ] Improve const-correctness
- [ ] Move duplicate code to utils

---

## 📊 PROGRESS TRACKING

**Phase 1:** ████████████████████ 100% ✅  
**Phase 2:** ████████████████████ 100% ✅  
**Phase 3:** ████░░░░░░░░░░░░░░░░  20% 🚧 (JSON schemas ready)  
**Phase 4:** ░░░░░░░░░░░░░░░░░░░░   0% 🔮  
**Phase 5:** ░░░░░░░░░░░░░░░░░░░░   0% 🔮  
**Phase 6:** ░░░░░░░░░░░░░░░░░░░░   0% 🔮  

---

## 🎯 SPRINT GOALS (Mục tiêu ngắn hạn)

### **Sprint 1 (Week 1-2): Knowledge Base**
- Implement KnowledgeBase class
- Test loading JSON files
- Basic search functionality

### **Sprint 2 (Week 3): Vietnamese Parser**
- Tokenizer + KeywordMapper
- Basic Vietnamese → Math parsing
- Test với examples

### **Sprint 3 (Week 4): Integration**
- Context + Conversation
- Wire up all components
- End-to-end testing

---

**Note:** File này nên được cập nhật mỗi ngày khi làm việc!
