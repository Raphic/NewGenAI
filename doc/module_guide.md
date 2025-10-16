# Hướng dẫn Modules
NewGen/
├── main.cpp
├── modules/
│   ├── math/
│   │   ├── calculator.h
│   │   └── calculator.cpp
│   ├── system/
│   │   ├── command_handler.h
│   │   └── command_handler.cpp
│   └── utils/
│       ├── common.h
│       └── common.cpp
├── batch/
│   ├── run_ai.bat
│   ├── rebuild.bat
│   └── clear.bat
└── data/
    └── version.txt
    
## Cấu trúc thư mục

modules/
├── math/ # Toán học
├── system/ # Hệ thống
├── language/ # Ngôn ngữ
├── ai/ # AI
├── utils/ # Tiện ích
└── data/ # Xử lý data

## Thêm module mới
1. Tạo .h và .cpp trong thư mục phù hợp
2. Thêm vào rebuild.bat
3. Import trong main.cpp (nếu cần)