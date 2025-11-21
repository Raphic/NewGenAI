# 📖 HƯỚNG DẪN THÊM MÔN HỌC MỚI VÀO KNOWLEDGE BASE

## 🎯 Mục đích tài liệu

Hướng dẫn chi tiết cách thêm môn học mới (Địa lý, Lịch sử, Tiếng Anh,...) vào hệ thống Knowledge Base.

---

## 📋 BƯỚC 1: Tạo file JSON cho môn học mới

### Quy tắc đặt tên file:
- File nằm trong: `data/knowledge/`
- Tên file: `<tên môn học>.json` (chữ thường, không dấu)
- Ví dụ: `geography.json`, `history.json`, `english.json`

### Template JSON chuẩn:

```json
{
  "domain": "<tên môn học>",
  "version": "1.0",
  "description": "Kiến thức môn <Tên đầy đủ>",
  "topics": [
    {
      "id": "<domain>_<category>_<tên ngắn gọn>",
      "domain": "<tên môn học>",
      "category": "<phân loại>",
      "name": "<tên chủ đề>",
      "aliases": ["<cách gọi khác 1>", "<cách gọi khác 2>"],
      "definition": "<định nghĩa ngắn gọn>",
      "formula": "<công thức nếu có, để trống nếu không>",
      "explanation": "<giải thích chi tiết>",
      "examples": ["<ví dụ 1>", "<ví dụ 2>"],
      "related_topics": ["<topic liên quan 1>", "<topic liên quan 2>"],
      "metadata": {
        "difficulty": "easy|medium|hard",
        "grade_level": "<cấp học>",
        "<key tùy chỉnh>": "<value>"
      }
    }
  ]
}
```

---

## 🌏 VÍ DỤ: Thêm môn ĐỊA LÝ

### File: `data/knowledge/geography.json`

```json
{
  "domain": "geography",
  "version": "1.0",
  "description": "Kiến thức môn Địa lý",
  "topics": [
    {
      "id": "geography_physical_climate",
      "domain": "geography",
      "category": "physical_geography",
      "name": "khí hậu nhiệt đới",
      "aliases": [
        "tropical climate",
        "khí hậu nóng ẩm",
        "vùng nhiệt đới"
      ],
      "definition": "Khí hậu có nhiệt độ cao quanh năm và lượng mưa lớn",
      "formula": "",
      "explanation": "Khí hậu nhiệt đới nằm giữa hai chí tuyến (23.5°N - 23.5°S). Đặc điểm: nhiệt độ trung bình >25°C, lượng mưa >1500mm/năm, có 2 mùa rõ rệt (mưa và khô).",
      "examples": [
        "Việt Nam: khí hậu nhiệt đới gió mùa",
        "Amazon: khí hậu nhiệt đới ẩm",
        "Đông Nam Á: nhiệt đới gió mùa"
      ],
      "related_topics": [
        "khí hậu ôn đới",
        "khí hậu hàn đới",
        "gió mùa",
        "lượng mưa"
      ],
      "metadata": {
        "difficulty": "medium",
        "grade_level": "10",
        "region": "tropical zone",
        "coordinates": "23.5°N - 23.5°S"
      }
    },
    {
      "id": "geography_human_urbanization",
      "domain": "geography",
      "category": "human_geography",
      "name": "đô thị hóa",
      "aliases": [
        "urbanization",
        "thành thị hóa"
      ],
      "definition": "Quá trình dân cư tập trung vào các thành phố",
      "formula": "Tỷ lệ đô thị hóa = (Dân số đô thị / Tổng dân số) × 100%",
      "explanation": "Đô thị hóa là xu hướng di dân từ nông thôn ra thành phố, làm tăng tỷ lệ dân số đô thị. Nguyên nhân: công nghiệp hóa, tìm kiếm việc làm, cơ sở hạ tầng tốt hơn.",
      "examples": [
        "TP.HCM: tỷ lệ đô thị hóa ~90%",
        "Việt Nam 2020: ~37% dân số sống ở đô thị",
        "Tokyo, New York: siêu đô thị"
      ],
      "related_topics": [
        "dân số",
        "công nghiệp hóa",
        "di dân",
        "quy hoạch đô thị"
      ],
      "metadata": {
        "difficulty": "medium",
        "grade_level": "11",
        "trend": "increasing"
      }
    }
  ]
}
```

---

## 📜 VÍ DỤ: Thêm môn LỊCH SỬ

### File: `data/knowledge/history.json`

```json
{
  "domain": "history",
  "version": "1.0",
  "description": "Kiến thức môn Lịch sử",
  "topics": [
    {
      "id": "history_vietnam_august_revolution",
      "domain": "history",
      "category": "vietnam_history",
      "name": "cách mạng tháng 8",
      "aliases": [
        "august revolution",
        "cách mạng tháng tám 1945"
      ],
      "definition": "Cuộc cách mạng giải phóng dân tộc do Đảng Cộng sản lãnh đạo năm 1945",
      "formula": "",
      "explanation": "Cách mạng Tháng Tám 1945 là cuộc tổng khởi nghĩa thành công, lật đổ chính quyền thực dân - phong kiến, thành lập nước Việt Nam Dân chủ Cộng hòa ngày 2/9/1945.",
      "examples": [
        "Khởi nghĩa Hà Nội: 19/8/1945",
        "Tổng khởi nghĩa toàn quốc: 13-19/8/1945",
        "Tuyên bố Độc lập: 2/9/1945"
      ],
      "related_topics": [
        "Việt Minh",
        "Hồ Chí Minh",
        "tuyên ngôn độc lập",
        "thế chiến 2"
      ],
      "metadata": {
        "difficulty": "medium",
        "grade_level": "9",
        "year": "1945",
        "significance": "national independence"
      }
    },
    {
      "id": "history_world_french_revolution",
      "domain": "history",
      "category": "world_history",
      "name": "cách mạng pháp",
      "aliases": [
        "french revolution",
        "cách mạng pháp 1789"
      ],
      "definition": "Cuộc cách mạng tư sản lật đổ chế độ phong kiến ở Pháp năm 1789",
      "formula": "",
      "explanation": "Cách mạng Pháp (1789-1799) là cuộc cách mạng tư sản quan trọng nhất lịch sử thế giới, lật đổ chế độ quân chủ chuyên chế, đưa ra Tuyên ngôn Nhân quyền và Dân quyền.",
      "examples": [
        "Chiếm nhà tù Bastille: 14/7/1789",
        "Tuyên ngôn Nhân quyền: 26/8/1789",
        "Xử tử Vua Louis XVI: 21/1/1793"
      ],
      "related_topics": [
        "chế độ phong kiến",
        "cách mạng tư sản",
        "nhân quyền",
        "Napoleon"
      ],
      "metadata": {
        "difficulty": "hard",
        "grade_level": "11",
        "year": "1789-1799",
        "impact": "global"
      }
    }
  ]
}
```

---

## 🇬🇧 VÍ DỤ: Thêm môn TIẾNG ANH

### File: `data/knowledge/english.json`

```json
{
  "domain": "english",
  "version": "1.0",
  "description": "Kiến thức môn Tiếng Anh",
  "topics": [
    {
      "id": "english_grammar_present_perfect",
      "domain": "english",
      "category": "grammar",
      "name": "thì hiện tại hoàn thành",
      "aliases": [
        "present perfect",
        "present perfect tense",
        "HTHT"
      ],
      "definition": "Thì diễn tả hành động đã xảy ra và có liên quan đến hiện tại",
      "formula": "S + have/has + V3/V-ed",
      "explanation": "Present Perfect dùng cho hành động bắt đầu trong quá khứ và còn tiếp diễn đến hiện tại, hoặc vừa mới hoàn thành và có ảnh hưởng đến hiện tại.",
      "examples": [
        "I have lived here for 5 years. (Tôi đã sống ở đây 5 năm)",
        "She has just finished her homework. (Cô ấy vừa hoàn thành bài tập)",
        "Have you ever been to Paris? (Bạn đã từng đến Paris chưa?)"
      ],
      "related_topics": [
        "present perfect continuous",
        "past simple",
        "thì quá khứ đơn",
        "irregular verbs"
      ],
      "metadata": {
        "difficulty": "medium",
        "grade_level": "8",
        "tense_type": "perfect",
        "time_markers": "for, since, just, already, yet, ever, never"
      }
    },
    {
      "id": "english_vocabulary_idiom",
      "domain": "english",
      "category": "vocabulary",
      "name": "thành ngữ tiếng anh",
      "aliases": [
        "idiom",
        "idioms",
        "thành ngữ"
      ],
      "definition": "Cụm từ có ý nghĩa khác với nghĩa đen của các từ tạo thành",
      "formula": "",
      "explanation": "Idiom là cách diễn đạt đặc trưng trong tiếng Anh, có nghĩa bóng không thể suy ra từ nghĩa đen. Việc sử dụng idiom giúp giao tiếp tự nhiên hơn.",
      "examples": [
        "Break the ice = Phá vỡ sự im lặng, bắt đầu trò chuyện",
        "Piece of cake = Dễ như ăn bánh (rất dễ)",
        "Cost an arm and a leg = Rất đắt",
        "Under the weather = Không khỏe"
      ],
      "related_topics": [
        "phrasal verbs",
        "collocation",
        "từ vựng nâng cao"
      ],
      "metadata": {
        "difficulty": "hard",
        "grade_level": "10",
        "usage": "informal, conversational"
      }
    }
  ]
}
```

---

## 🎨 GUIDELINES CHO TỪNG FIELD

### **1. `id`** (Identifier)
- Format: `<domain>_<category>_<short_name>`
- Chữ thường, dùng `_` thay space
- Phải unique trong toàn bộ knowledge base
- Ví dụ: `geography_physical_climate`, `history_vietnam_august_revolution`

### **2. `domain`** (Môn học)
- Danh sách domains chuẩn:
  - `math`, `physics`, `chemistry`, `biology`
  - `literature`, `geography`, `history`
  - `english`, `vietnamese`, `art`, `music`
  - `computer_science`, `economics`, `philosophy`

### **3. `category`** (Phân loại)
- Tùy theo môn học, ví dụ:
  - Math: `algebra`, `geometry`, `calculus`, `statistics`
  - Physics: `mechanics`, `electricity`, `optics`, `thermodynamics`
  - Geography: `physical_geography`, `human_geography`, `regional`
  - History: `vietnam_history`, `world_history`, `modern_history`

### **4. `name`** (Tên chủ đề)
- Tên tiếng Việt, viết thường
- Ngắn gọn, dễ hiểu
- Ví dụ: "phương trình bậc 2", "định luật newton", "cách mạng pháp"

### **5. `aliases`** (Tên gọi khác)
- Mảng các cách gọi khác nhau
- Bao gồm: viết tắt, tiếng Anh, cách gọi thông dụng
- Giúp search tốt hơn
- Ví dụ: `["pt bậc 2", "quadratic equation", "phương trình bậc hai"]`

### **6. `definition`** (Định nghĩa)
- Định nghĩa ngắn gọn, 1-2 câu
- Dễ hiểu, tránh thuật ngữ phức tạp
- Trả lời câu hỏi "X là gì?"

### **7. `formula`** (Công thức)
- Nếu có công thức toán học/hóa học
- Dùng UTF-8: √, ≠, ≤, ±, π, ²,³
- Để trống `""` nếu không có

### **8. `explanation`** (Giải thích)
- Giải thích chi tiết, đầy đủ
- Có thể nhiều đoạn
- Bao gồm: nguyên lý, cách dùng, lưu ý

### **9. `examples`** (Ví dụ)
- Mảng các ví dụ minh họa
- Cụ thể, dễ hiểu
- Nên có 2-5 ví dụ

### **10. `related_topics`** (Topics liên quan)
- Mảng tên các topics liên quan
- Dùng `name` của topic khác
- Giúp gợi ý "Xem thêm"

### **11. `metadata`** (Thông tin bổ sung)
- Object với các key-value tùy chỉnh
- Thường có:
  - `difficulty`: "easy", "medium", "hard"
  - `grade_level`: "6", "7", "8",... "12"
  - Các field domain-specific khác

---

## ⚙️ BƯỚC 2: Cập nhật Code để Load Domain Mới

### Không cần sửa code!

System đã được thiết kế để **tự động phát hiện** file JSON mới trong `data/knowledge/`.

Chỉ cần:
1. Tạo file JSON theo template
2. Restart chương trình
3. Gõ lệnh `domains` để xem danh sách

---

## 🧪 BƯỚC 3: Test Domain Mới

### Test commands:

```bash
# 1. Kiểm tra domain đã load
domains

# 2. Search topic
search <từ khóa>

# 3. Định nghĩa
định nghĩa <topic name>

# 4. Giải thích
giải thích <topic name>
```

### Ví dụ test Geography:

```
User: "domains"
Bot: "📚 Các môn học hiện có:
      • math (Toán học)
      • physics (Vật lý)
      • chemistry (Hóa học)
      • geography (Địa lý) ← NEW!"

User: "định nghĩa khí hậu nhiệt đới"
Bot: "📘 Khí hậu nhiệt đới là khí hậu có nhiệt độ cao quanh năm..."
```

---

## 📊 CHECKLIST KHI THÊM MÔN MỚI

- [ ] Đặt tên file đúng format: `<domain>.json`
- [ ] Có field `domain`, `version`, `description`
- [ ] Mỗi topic có đầy đủ required fields
- [ ] `id` unique, format đúng
- [ ] `aliases` đầy đủ (tiếng Việt + tiếng Anh)
- [ ] `definition` ngắn gọn, dễ hiểu
- [ ] `explanation` chi tiết
- [ ] Có ít nhất 2-3 ví dụ trong `examples`
- [ ] `related_topics` link đến topics khác
- [ ] `metadata` có `difficulty` và `grade_level`
- [ ] File JSON valid (dùng JSONLint check)
- [ ] Test load và search thành công

---

## 💡 TIPS VIẾT KNOWLEDGE TỐT

### 1. **Ngôn ngữ đơn giản**
- Tránh thuật ngữ quá chuyên sâu
- Giải thích như nói với học sinh

### 2. **Ví dụ cụ thể**
- Dùng số liệu thực tế
- Ví dụ gần gũi với đời sống

### 3. **Liên kết topics**
- Luôn có `related_topics`
- Tạo network kiến thức

### 4. **Metadata đầy đủ**
- Giúp AI classify topic tốt hơn
- Hỗ trợ filter theo độ khó, cấp học

### 5. **Cập nhật thường xuyên**
- Thêm topics mới
- Sửa lỗi, bổ sung ví dụ

---

## 🚀 FUTURE ENHANCEMENTS

Khi knowledge base lớn hơn, có thể:

### 1. **Sub-categories**
```json
"category": "mechanics",
"subcategory": "kinematics"
```

### 2. **Media references**
```json
"media": {
  "images": ["diagram1.png"],
  "videos": ["demo.mp4"]
}
```

### 3. **Prerequisites**
```json
"prerequisites": ["phương trình bậc nhất"]
```

### 4. **Difficulty scoring**
```json
"metadata": {
  "difficulty_score": 7.5,
  "estimated_time": "30 minutes"
}
```

---

## 📝 SUMMARY

**Để thêm môn học mới:**
1. Tạo file `data/knowledge/<domain>.json`
2. Follow template JSON
3. Viết ít nhất 3-5 topics
4. Test với commands

**Môn học đề xuất thêm tiếp:**
- 🌏 Geography (Địa lý) - DONE trong ví dụ
- 📜 History (Lịch sử) - DONE trong ví dụ
- 🇬🇧 English (Tiếng Anh) - DONE trong ví dụ
- 🧬 Biology (Sinh học)
- 💻 Computer Science (Tin học)
- 🎨 Art (Mỹ thuật)
- 🎵 Music (Âm nhạc)

Hệ thống đã sẵn sàng mở rộng vô hạn! 🚀
