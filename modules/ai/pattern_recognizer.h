#pragma once
#include <string>
#include <vector>
#include <map>
#include "math_logger.h"

namespace ai {

// ==================== PHÂN LOẠI DẠNG TOÁN ====================
enum class MathCategory {
    ARITHMETIC,
    ALGEBRA_LINEAR,
    ALGEBRA_QUADRATIC,
    TRIGONOMETRY,
    LOGARITHM,
    POWER,
    MIXED,
    UNKNOWN
};

// ==================== CẤU TRÚC ĐẶC TRƯNG ====================
struct ProblemFeatures {
    bool hasVariable;
    bool hasSquare;
    bool hasTrig;
    bool hasLog;
    bool hasPower;
    bool hasEquation;
    int operatorCount;
    std::vector<std::string> keywords;
};

// ==================== THỐNG KÊ DẠNG TOÁN ====================
struct CategoryStats {
    int totalCount;
    int successCount;
    double successRate;
    std::vector<std::string> examples;
};

class PatternRecognizer {
public:
    // ===== Phân loại =====
    MathCategory classify(const std::string& problem) const;
    ProblemFeatures extractFeatures(const std::string& problem) const;

    // ===== Học từ lịch sử =====
    void learnFromHistory(const std::vector<MathProblem>& history);

    // ===== Lưu / tải kiến thức =====
    bool saveKnowledge(const std::string& filename = "data/ai_knowledge.json") const; // ✅ thêm const
    bool loadKnowledge(const std::string& filename = "data/ai_knowledge.json");
    bool hasKnowledge() const;

    // ===== Thống kê =====
    void showCategoryStats() const; // ✅ thêm const
    std::map<MathCategory, CategoryStats> getStats() const;

    // ===== Gợi ý =====
    std::string suggestMethod(const std::string& problem) const; // ✅ thêm const
    std::vector<std::string> findSimilarProblems(const std::string& problem, int limit = 5) const; // ✅ thêm const
    std::string categoryToString(MathCategory cat) const;

private:
    std::map<MathCategory, CategoryStats> stats_;
    std::vector<MathProblem> learnedProblems_;

    bool containsKeyword(const std::string& text, const std::string& keyword) const;
    double calculateSimilarity(const std::string& prob1, const std::string& prob2) const;
    MathCategory stringToCategory(const std::string& str) const; // 🧩 thêm dòng này
};

// ==================== HÀM TIỆN ÍCH NGOÀI CLASS ====================
// 👇 Thêm dòng này để fix “stringToCategory undefined”
MathCategory stringToCategory(const std::string& str);

} // namespace ai
