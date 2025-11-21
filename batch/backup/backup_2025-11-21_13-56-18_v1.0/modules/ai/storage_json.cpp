// storage_json.cpp
#include "math_logger.h"
#include "storage_json.h"
#include "math_problem.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace ai {

// 🟩 Triển khai constructor
JsonStorage::JsonStorage(const std::string& filename)
    : filePath_(filename) {}

// 🟩 Triển khai các phương thức
bool JsonStorage::saveHistory(const std::vector<MathProblem>& history) {
    std::ofstream file(filePath_, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "❌ Không thể ghi file: " << filePath_ << "\n";
        return false;
    }
    for (const auto& p : history) {
        file << "{"
             << "\"input\":\"" << p.input << "\","
             << "\"type\":\"" << p.type << "\","
             << "\"result\":\"" << p.result << "\","
             << "\"success\":" << (p.success ? "true" : "false") << ","
             << "\"timestamp\":" << p.timestamp
             << "}\n";
    }
    file.close();
    return true;
}
std::vector<MathProblem> JsonStorage::loadHistory() {
    std::vector<MathProblem> history;
    std::ifstream file(filePath_);
    if (!file.is_open()) {
        std::cerr << "⚠️ Không tìm thấy file: " << filePath_ << "\n";
        return history;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("{") == std::string::npos) continue;

        MathProblem p;
        auto get = [&](const std::string& key) -> std::string {
            size_t pos = line.find("\"" + key + "\":");
            if (pos == std::string::npos) return "";
            size_t start = line.find("\"", pos + key.size() + 3);
            size_t end = line.find("\"", start + 1);
            return line.substr(start + 1, end - start - 1);
        };

        p.input = get("input");
        p.type = get("type");
        p.result = get("result");
        p.success = (line.find("\"success\":true") != std::string::npos);
        p.timestamp = 0; // tạm thời

        if (!p.input.empty()) history.push_back(p);
    }
    file.close();
    return history;
}
bool JsonStorage::appendProblem(const MathProblem& p) {
    std::ofstream file(filePath_, std::ios::app);
    if (!file.is_open()) return false;

    file << "{"
         << "\"input\":\"" << p.input << "\","
         << "\"type\":\"" << p.type << "\","
         << "\"result\":\"" << p.result << "\","
         << "\"success\":" << (p.success ? "true" : "false") << ","
         << "\"timestamp\":" << p.timestamp
         << "}\n";
    file.close();
    return true;
}

bool JsonStorage::backup() {
    try {
        std::filesystem::create_directories("data/backup"); // đảm bảo thư mục tồn tại
        std::filesystem::copy_file(
            filePath_,
            "data/backup/math_history_backup.json",
            std::filesystem::copy_options::overwrite_existing);
        return true;
    } catch (...) {
        std::cerr << "⚠️ Không thể backup file.\n";
        return false;
    }
}

} // namespace ai