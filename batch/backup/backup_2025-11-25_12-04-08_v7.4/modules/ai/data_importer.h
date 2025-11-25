#pragma once
#include <string>
#include <vector>
#include <set>
#include "math_logger.h"

namespace ai {

// ==================== CẤU TRÚC DỮ LIỆU ====================
struct FileInfo {
    std::string filename;
    std::string fullPath;
    bool imported = false;
    int problemCount = 0;
};

struct TrainingData {
    std::string problem;
    std::string answer;
    std::string category;
};

struct ImportStats {
    int totalFound = 0;     // Tổng số bài trong file
    int duplicates = 0;     // Số bài trùng (đã giải rồi)
    int newProblems = 0;    // Số bài mới
    int solved = 0;         // Số bài giải thành công
    int failed = 0;         // Số bài giải thất bại
};


// ==================== CLASS CHÍNH ====================
class DataImporter {
public:
    // 🧠 Constructor nhận logger (Dependency Injection)
    explicit DataImporter(ai::MathLogger& loggerRef) : logger_(loggerRef) {}

    // ========== SMART IMPORT (UI) ==========
    bool smartImport();

    // ========== IMPORT & SOLVE ==========
    ImportStats importAndSolveCSV(const std::string& csvFile, bool skipDuplicates = true);
    ImportStats importAndSolveText(const std::string& textFile, bool skipDuplicates = true);
    ImportStats batchSolve(const std::vector<std::string>& problems, bool skipDuplicates = true, const std::string& filename = "");

    // ========== DUPLICATE CHECK ==========
    std::set<std::string> getExistingProblems();
    static std::string normalizeForComparison(const std::string& input);

    // ========== FILE READERS ==========
    static std::vector<TrainingData> readCSV(const std::string& csvFile);
    static std::vector<std::string> readTextFile(const std::string& textFile);
    static int countProblems(const std::string& filepath);

    // ========== FILE LOG / SCAN ==========
    static std::vector<FileInfo> scanImportFolder();
    static std::set<std::string> getImportedFiles();
    static void markAsImported(const std::string& filename);
    static void resetImportLog();

private:
    ai::MathLogger& logger_;  // 🧩 Tham chiếu đến logger gốc (do main truyền vào)
    static const std::string IMPORT_FOLDER;
    static const std::string LOG_FILE;
};

} // namespace ai
