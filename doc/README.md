# 📚 NewGen AI Project - Documentation Hub

**Version:** v77.1  
**Last Updated:** 2025-11-21  
**Project:** Advanced Modular AI System - Local C++ Implementation

---

## 🚪 BẮT ĐẦU TỪ ĐÂU?

### 👋 **Nếu bạn là member mới / AI agent mới:**

**Đọc theo thứ tự sau để nắm bắt dự án nhanh nhất:**

1. **[getting_started/quick_start.md](getting_started/quick_start.md)** ⚡  
   → Setup môi trường và chạy lần đầu (5-10 phút)

2. **[overview/project_review.md](overview/project_review.md)** 📊  
   → Tổng quan toàn diện: architecture, modules, data, code quality

3. **[getting_started/workflow.md](getting_started/workflow.md)** 🔧  
   → Quy trình làm việc hàng ngày: build, test, commit

4. **[understanding/thinking_process.md](understanding/thinking_process.md)** 🧠  
   → Hiểu tư duy thiết kế và triết lý của developer

5. **[working/todo.md](working/todo.md)** ✅  
   → Xem task hiện tại và next steps

---

## 📖 TÀI LIỆU THEO CHỨC NĂNG

### 🎯 **Hiểu Tổng Quan** (`overview/`)
| File | Mô tả |
|------|-------|
| [project_review.md](overview/project_review.md) | Phân tích toàn diện dự án (871 dòng) |
| [architecture.md](overview/architecture.md) | Sơ đồ kiến trúc hệ thống |

### 🏃 **Bắt Đầu Nhanh** (`getting_started/`)
| File | Mô tả |
|------|-------|
| [quick_start.md](getting_started/quick_start.md) | Setup + chạy lần đầu (5 phút) |
| [workflow.md](getting_started/workflow.md) | Quy trình làm việc hàng ngày |

### 🧠 **Hiểu Sâu Về Code** (`understanding/`)
| File | Mô tả |
|------|-------|
| [thinking_process.md](understanding/thinking_process.md) | Triết lý thiết kế, tư duy developer |
| [coding_conventions.md](understanding/coding_conventions.md) | Code style, naming conventions |

### 📋 **Làm Việc Thực Tế** (`working/`)
| File | Mô tả |
|------|-------|
| [todo.md](working/todo.md) | Task hiện tại (cập nhật hàng ngày) |
| [api_reference.md](working/api_reference.md) | API documentation (đang bổ sung) |

### 📚 **Mở Rộng Tính Năng** (`extending/`)
| File | Mô tả |
|------|-------|
| [phase3_guide.md](extending/phase3_guide.md) | Hướng dẫn implement Phase 3 (592 dòng) |
| [how_to_add_subjects.md](extending/how_to_add_subjects.md) | Thêm môn học vào Knowledge Base (482 dòng) |
| [how_to_add_modules.md](extending/how_to_add_modules.md) | Thêm C++ module mới vào project |

---

## 📁 CẤU TRÚC DOCUMENTATION

```
doc/
├── README.md                    # ← BẠN ĐANG Ở ĐÂY
│
├── overview/                    # Hiểu tổng quan dự án
│   ├── project_review.md
│   └── architecture.md
│
├── getting_started/             # Bắt đầu nhanh
│   ├── quick_start.md
│   └── workflow.md
│
├── understanding/               # Hiểu sâu về code
│   ├── thinking_process.md
│   └── coding_conventions.md
│
├── working/                     # Làm việc hàng ngày
│   ├── todo.md
│   └── api_reference.md
│
└── extending/                   # Mở rộng tính năng
    ├── phase3_guide.md
    ├── how_to_add_subjects.md
    └── how_to_add_modules.md
```

---

## 🎯 ROADMAP DỰ ÁN

### **✅ Phase 1-2: COMPLETED**
- ✅ Logging System (MathLogger)
- ✅ Pattern Recognition (PatternRecognizer)
- ✅ Build System (Incremental + Rollback)
- ✅ 14 Commands implemented

### **🚧 Phase 3: IN PREPARATION**
- 📁 Vietnamese NLP Parser
- 📁 Knowledge Base System
- 📁 Conversation Manager
- 📁 Intent Classifier

### **🔮 Phase 4-6: FUTURE**
- 🔮 Reasoning Engine (Case-Based Reasoning)
- 🔮 ML Capabilities
- 🔮 Distributed Learning

---

## 🛠️ COMMANDS NHANH

### **Build & Run:**
```bash
# Incremental build (khuyên dùng)
batch\build_modules.bat
> update

# Full rebuild
batch\build_modules.bat
> build

# Run program
batch\build_modules.bat
> run
```

### **Trong chương trình:**
```
help        - Xem tất cả lệnh
calculator  - Xem khả năng toán học
train       - Train AI từ history
stats       - Thống kê sử dụng
import      - Import training data
export      - Export ra CSV
```

---

## 📞 HỖ TRỢ & LIÊN HỆ

**Nếu gặp vấn đề:**
1. Đọc [getting_started/quick_start.md](getting_started/quick_start.md) - Troubleshooting section
2. Xem [overview/project_review.md](overview/project_review.md) - Issues & Recommendations
3. Check [working/todo.md](working/todo.md) - Known issues

**Muốn contribute:**
1. Đọc [getting_started/workflow.md](getting_started/workflow.md)
2. Đọc [understanding/coding_conventions.md](understanding/coding_conventions.md)
3. Check [working/todo.md](working/todo.md) cho tasks có thể làm

---

**Happy Coding! 🚀**
