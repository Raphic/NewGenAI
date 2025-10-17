#include "pattern_recognizer.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>   // Thêm header này cho numeric_limits
#include <map>
#include <vector>
#include <cctype>

namespace ai {

// ==================== PHÂN LOẠI DẠNG TOÁN ====================
MathCategory PatternRecognizer::classify(const std::string& problem) const {
    ProblemFeatures features = extractFeatures(problem);

    if (features.hasEquation && features.hasSquare && features.hasVariable)
        return MathCategory::ALGEBRA_QUADRATIC;

    if (features.hasEquation && features.hasVariable && !features.hasSquare)
        return MathCategory::ALGEBRA_LINEAR;

    if (features.hasTrig)
        return MathCategory::TRIGONOMETRY;

    if (features.hasLog)
        return MathCategory::LOGARITHM;

    if (features.hasPower && !features.hasTrig && !features.hasLog)
        return MathCategory::POWER;

    if ((features.hasTrig || features.hasLog || features.hasPower) &&
        features.operatorCount > 1)
        return MathCategory::MIXED;

    if (!features.hasVariable && !features.hasTrig && !features.hasLog)
        return MathCategory::ARITHMETIC;

    return MathCategory::UNKNOWN;
}

// ==================== TRÍCH XUẤT ĐẶC TRƯNG ====================
ProblemFeatures PatternRecognizer::extractFeatures(const std::string& problem) const {
    ProblemFeatures f{};
    std::string lower = problem;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    f.hasVariable = (lower.find('x') != std::string::npos ||
                     lower.find('y') != std::string::npos);

    f.hasSquare = (lower.find("x^2") != std::string::npos ||
                   lower.find("x²") != std::string::npos ||
                   lower.find("y^2") != std::string::npos);

    std::vector<std::string> trig = {"sin", "cos", "tan", "asin", "acos", "atan"};
    for (const auto& kw : trig) {
        if (containsKeyword(lower, kw)) {
            f.hasTrig = true;
            f.keywords.push_back(kw);
        }
    }

    if (containsKeyword(lower, "log") || containsKeyword(lower, "ln")) {
        f.hasLog = true;
        f.keywords.push_back(containsKeyword(lower, "log") ? "log" : "ln");
    }

    f.hasPower = (problem.find('^') != std::string::npos);
    f.hasEquation = (problem.find('=') != std::string::npos);

    for (char c : problem)
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
            f.operatorCount++;

    return f;
}

// ==================== HỌC TỪ LỊCH SỬ ====================
void PatternRecognizer::learnFromHistory(const std::vector<MathProblem>& history) {
    learnedProblems_ = history;
    stats_.clear();

    for (const auto& p : history) {
        MathCategory cat = classify(p.input);
        auto& stat = stats_[cat];
        stat.totalCount++;
        if (p.success) stat.successCount++;
        if (stat.examples.size() < 5) stat.examples.push_back(p.input);
    }

    for (auto& [cat, stat] : stats_)
        if (stat.totalCount > 0)
            stat.successRate = (double)stat.successCount / stat.totalCount * 100.0;

    std::cout << "✅ Đã học từ " << history.size() << " bài toán!\n";
}

// ==================== HIỂN THỊ THỐNG KÊ ====================
void PatternRecognizer::showCategoryStats() const {
    if (stats_.empty()) {
        std::cout << "⚠️ Chưa có dữ liệu học. Hãy dùng 'train' trước!\n";
        return;
    }

    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         📊 THỐNG KÊ DẠNG TOÁN ĐÃ HỌC                      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    for (const auto& [cat, stat] : stats_) {
        std::cout << "📌 " << categoryToString(cat) << "\n";
        std::cout << "   Tổng số: " << stat.totalCount
                  << " | Thành công: " << stat.successCount
                  << " | Tỷ lệ: " << std::fixed << std::setprecision(1)
                  << stat.successRate << "%\n";
        if (!stat.examples.empty()) {
            std::cout << "   Ví dụ:\n";
            for (size_t i = 0; i < stat.examples.size() && i < 3; i++)
                std::cout << "      • " << stat.examples[i] << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

// ==================== GỢI Ý PHƯƠNG PHÁP ====================
std::string PatternRecognizer::suggestMethod(const std::string& problem) const {
    MathCategory cat = classify(problem);
    switch (cat) {
        case MathCategory::ARITHMETIC:
            return "💡 Phép tính số học: tính ngoặc trước, nhân/chia trước cộng/trừ.";
        case MathCategory::ALGEBRA_LINEAR:
            return "💡 Phương trình bậc 1:\n1. Chuyển vế\n2. Rút gọn\n3. Tính x = -b/a";
        case MathCategory::ALGEBRA_QUADRATIC:
            return "💡 Phương trình bậc 2:\n1. Đưa về ax² + bx + c = 0\n2. Tính Δ = b² - 4ac\n3. x = (-b ± √Δ)/2a";
        case MathCategory::TRIGONOMETRY:
            return "💡 Lượng giác: sin(30°)=0.5, cos(60°)=0.5, tan(45°)=1";
        case MathCategory::LOGARITHM:
            return "💡 Logarit: log(x)=log₁₀x, ln(x)=logₑx";
        case MathCategory::POWER:
            return "💡 Lũy thừa: a^b = a mũ b, 2^3^2 = 2^(3^2)";
        case MathCategory::MIXED:
            return "💡 Bài toán hỗn hợp: tính hàm, sau đó lũy thừa, nhân/chia, cộng/trừ.";
        default:
            return "💡 Chưa nhận dạng được — hãy chia nhỏ bài toán hơn.";
    }
}

// ==================== TÌM BÀI TOÁN TƯƠNG TỰ ====================
std::vector<std::string> PatternRecognizer::findSimilarProblems(
    const std::string& problem, int limit) const {
    std::vector<std::pair<double, std::string>> scored;

    for (const auto& learned : learnedProblems_) {
        double score = calculateSimilarity(problem, learned.input);
        if (score > 0.3)
            scored.push_back({score, learned.input});
    }

    std::sort(scored.begin(), scored.end(),
              [](auto& a, auto& b) { return a.first > b.first; });

    std::vector<std::string> result;
    for (size_t i = 0; i < scored.size() && i < (size_t)limit; i++)
        result.push_back(scored[i].second);

    return result;
}

// ==================== CÔNG CỤ HỖ TRỢ ====================
bool PatternRecognizer::containsKeyword(const std::string& text, const std::string& keyword) const {
    return text.find(keyword) != std::string::npos;
}

double PatternRecognizer::calculateSimilarity(const std::string& p1,
                                              const std::string& p2) const {
    ProblemFeatures f1 = extractFeatures(p1);
    ProblemFeatures f2 = extractFeatures(p2);
    double score = 0.0;

    if (f1.hasVariable == f2.hasVariable) score += 0.2;
    if (f1.hasSquare == f2.hasSquare)     score += 0.2;
    if (f1.hasTrig == f2.hasTrig)         score += 0.2;
    if (f1.hasLog == f2.hasLog)           score += 0.2;
    if (f1.hasEquation == f2.hasEquation) score += 0.2;

    return score;
}

// Lưu / tải kiến thức đã học (định dạng đơn giản: "<success:int> <quoted input>\n")
bool PatternRecognizer::saveKnowledge(const std::string& path) const {
    std::ofstream ofs(path);
    if (!ofs) return false;
    for (const auto& p : learnedProblems_) {
        ofs << (p.success ? 1 : 0) << ' ' << std::quoted(p.input) << '\n';
        if (!ofs) return false;
    }
    return true;
}

bool PatternRecognizer::loadKnowledge(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) return false;
    std::vector<MathProblem> loaded;
    while (ifs) {
        int ok = 0;
        std::string input;
        ifs >> ok;
        if (!ifs) break;
        ifs >> std::ws;
        ifs >> std::quoted(input);
        if (!ifs) break;
        MathProblem mp;
        mp.input = input;
        mp.success = (ok != 0);
        loaded.push_back(mp);
        // consume rest of line if any
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (loaded.empty()) return false;
    learnFromHistory(loaded);
    return true;
}

// Canonical mapping table (sửa ở đây khi thêm/xoá dạng)
static const std::vector<std::pair<MathCategory, const char*>> kCategoryMap = {
    { MathCategory::ARITHMETIC,        "Số học" },
    { MathCategory::ALGEBRA_LINEAR,    "Phương trình bậc 1" },
    { MathCategory::ALGEBRA_QUADRATIC, "Phương trình bậc 2" },
    { MathCategory::TRIGONOMETRY,      "Lượng giác" },
    { MathCategory::LOGARITHM,         "Logarit" },
    { MathCategory::POWER,             "Lũy thừa" },
    { MathCategory::MIXED,             "Hỗn hợp" }
};

// Enum -> string: dùng bảng chung
std::string PatternRecognizer::categoryToString(MathCategory cat) const {
    for (const auto& p : kCategoryMap) {
        if (p.first == cat) return std::string(p.second);
    }
    return "Chưa xác định";
}

// String -> enum: dùng bảng chung
MathCategory stringToCategory(const std::string& str) {
    for (const auto& p : kCategoryMap) {
        if (str == p.second) return p.first;
    }
    return MathCategory::UNKNOWN;
}

// Member delegating (giữ nếu header còn khai báo thành viên)
MathCategory PatternRecognizer::stringToCategory(const std::string& s) const {
    return ::ai::stringToCategory(s);
}

bool PatternRecognizer::hasKnowledge() const {
    return !stats_.empty();
}

std::map<MathCategory, CategoryStats> PatternRecognizer::getStats() const {
    return stats_;
}

} // namespace ai
