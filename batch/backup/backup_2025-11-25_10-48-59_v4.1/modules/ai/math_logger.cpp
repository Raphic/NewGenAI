#include "storage_json.h"
#include "math_logger.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>
#include <cctype>

namespace ai {

const std::string MathLogger::LOG_FILE = "data/math_history.json";

// ==================== HELPER: PARSE JSON ====================
static std::string extractJsonValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":\"";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        searchKey = "\"" + key + "\":";
        pos = json.find(searchKey);
        if (pos == std::string::npos)
            return "";
        
        pos += searchKey.length();
        size_t endPos = json.find_first_of(",}", pos);
        return json.substr(pos, endPos - pos);
    }
    
    pos += searchKey.length();
    size_t endPos = json.find("\"", pos);
    if (endPos == std::string::npos)
        return "";
    
    return json.substr(pos, endPos - pos);
}

// ==================== LOG BÀI TOÁN ====================
void MathLogger::log(const MathProblem& problem) {
    if (storage_) {
        storage_->appendProblem(problem);
        return;
    }

    std::ofstream file(LOG_FILE, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "⚠️ Không thể mở file log\n";
        return;
    }
    
    file << "{"
         << "\"input\":\"" << problem.input << "\","
         << "\"type\":\"" << problem.type << "\","
         << "\"result\":\"" << problem.result << "\","
         << "\"success\":" << (problem.success ? "true" : "false") << ","
         << "\"timestamp\":" << problem.timestamp
         << "}\n";
    file.close();
}

// ==================== ĐỌC LỊCH SỬ ====================
std::vector<MathProblem> MathLogger::getHistory() {
    if (storage_)
        return storage_->loadHistory();

    std::vector<MathProblem> history;
    std::ifstream file(LOG_FILE);
    if (!file.is_open())
        return history;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line == "{}" || line.find("input") == std::string::npos)
            continue;

        MathProblem p;
        p.input = extractJsonValue(line, "input");
        p.type = extractJsonValue(line, "type");
        p.result = extractJsonValue(line, "result");

        std::string successStr = extractJsonValue(line, "success");
        p.success = (successStr == "true");

        std::string timestampStr = extractJsonValue(line, "timestamp");
        try {
            p.timestamp = std::stoll(timestampStr);
        } catch (...) {
            p.timestamp = 0;
        }

        if (!p.input.empty())
            history.push_back(p);
    }

    file.close();
    return history;
}

// ==================== NORMALIZE ====================
static std::string normalizeInput(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (!std::isspace(c))
            result += std::tolower(c);
    }
    return result;
}

// ==================== HIỂN THỊ STATS ====================
void MathLogger::showStats() {
    auto history = getHistory();
    if (history.empty()) {
        std::cout << "📊 Chưa có dữ liệu\n";
        return;
    }

    std::map<std::string, int> typeCounts;
    int successCount = 0;
    for (const auto& problem : history) {
        typeCounts[problem.type]++;
        if (problem.success) successCount++;
    }

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              📊 THỐNG KÊ TỔNG QUAN                         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    std::cout << "📈 Tổng số bài đã giải: " << history.size() << "\n";
    std::cout << "   ✅ Thành công: " << successCount << " (" 
              << (successCount * 100.0 / history.size()) << "%)\n";
    std::cout << "   ❌ Thất bại: " << (history.size() - successCount) << "\n\n";

    std::cout << "📋 Phân loại:\n";
    for (const auto& pair : typeCounts)
        std::cout << "   • " << pair.first << ": " << pair.second << " bài\n";

    std::cout << "\n💡 Gõ 'train' để AI học từ dữ liệu này\n";
    std::cout << "💡 Gõ 'analyze' để xem phân tích chi tiết sau khi train\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
}

// ==================== ĐẾM UNIQUE ====================
int MathLogger::getUniqueCount() {
    auto history = getHistory();
    std::set<std::string> uniqueInputs;
    for (const auto& problem : history)
        uniqueInputs.insert(normalizeInput(problem.input));
    return uniqueInputs.size();
}

// ==================== BACKUP ====================
void MathLogger::backupHistory() {
    if (storage_) {
        storage_->backup();
        return;
    }

    std::string backupFile = LOG_FILE + ".backup";
    std::ifstream src(LOG_FILE, std::ios::binary);
    std::ofstream dst(backupFile, std::ios::binary);

    if (!src.is_open() || !dst.is_open()) {
        std::cerr << "⚠️ Không thể backup file log\n";
        return;
    }

    dst << src.rdbuf();
    src.close();
    dst.close();

    std::cout << "✅ Đã backup: " << backupFile << "\n";
}

// ==================== XÓA TRÙNG ====================
int MathLogger::removeDuplicates() {
    auto history = getHistory();
    int originalCount = history.size();
    if (originalCount == 0) {
        std::cout << "⚠️ Không có dữ liệu\n";
        return 0;
    }

    std::cout << "🔍 Đang phân tích " << originalCount << " bài toán...\n";
    if (storage_) storage_->backup();
    backupHistory();

    std::set<std::string> seen;
    std::vector<MathProblem> unique;
    int skipped = 0;

    for (const auto& problem : history) {
        if (problem.input.empty()) {
            skipped++;
            continue;
        }
        std::string normalized = normalizeInput(problem.input);
        if (seen.find(normalized) == seen.end()) {
            seen.insert(normalized);
            unique.push_back(problem);
        }
    }

    int duplicateCount = originalCount - unique.size() - skipped;
    if (duplicateCount == 0 && skipped == 0) {
        std::cout << "✅ Không có dữ liệu trùng lặp\n";
        return 0;
    }

    std::ofstream file(LOG_FILE, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "❌ Không thể ghi file\n";
        return 0;
    }

    for (const auto& problem : unique) {
        file << "{"
             << "\"input\":\"" << problem.input << "\","
             << "\"type\":\"" << problem.type << "\","
             << "\"result\":\"" << problem.result << "\","
             << "\"success\":" << (problem.success ? "true" : "false") << ","
             << "\"timestamp\":" << problem.timestamp
             << "}\n";
    }
    file.close();

    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "🧹 CLEANUP HOÀN TẤT\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "   Trước: " << originalCount << " bài\n";
    std::cout << "   Sau:   " << unique.size() << " bài\n";
    std::cout << "   🗑️  Đã xóa: " << duplicateCount << " bài trùng\n";
    if (skipped > 0)
        std::cout << "   ⚠️  Bỏ qua: " << skipped << " bài lỗi\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

    return duplicateCount;
}

} // namespace ai
