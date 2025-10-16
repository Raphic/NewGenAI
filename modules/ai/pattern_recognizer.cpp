#include "pattern_recognizer.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
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

std::string PatternRecognizer::categoryToString(MathCategory cat) const {
    switch (cat) {
        case MathCategory::ARITHMETIC: return "Số học";
        case MathCategory::ALGEBRA_LINEAR: return "Phương trình bậc 1";
        case MathCategory::ALGEBRA_QUADRATIC: return "Phương trình bậc 2";
        case MathCategory::TRIGONOMETRY: return "Lượng giác";
        case MathCategory::LOGARITHM: return "Logarit";
        case MathCategory::POWER: return "Lũy thừa";
        case MathCategory::MIXED: return "Hỗn hợp";
        default: return "Chưa xác định";
    }
}

// ==================== LƯU & TẢI KIẾN THỨC ====================
bool PatternRecognizer::saveKnowledge(const std::string& file) const {
    std::ofstream out(file);
    if (!out.is_open()) {
        std::cerr << "❌ Không thể lưu: " << file << "\n";
        return false;
    }

    out << "{\n  \"learned_count\": " << learnedProblems_.size() << ",\n";
    out << "  \"categories\": {\n";
    bool first = true;

    for (const auto& [cat, s] : stats_) {
        if (!first) out << ",\n";
        first = false;

        out << "    \"" << categoryToString(cat) << "\": {\n";
        out << "      \"total\": " << s.totalCount << ",\n";
        out << "      \"success\": " << s.successCount << ",\n";
        out << "      \"rate\": " << s.successRate << ",\n";
        out << "      \"examples\": [";

        for (size_t i = 0; i < s.examples.size(); ++i) {
            if (i > 0) out << ", ";
            out << "\"" << s.examples[i] << "\"";
        }

        out << "]\n    }";
    }

    out << "\n  }\n}\n";
    out.close();
    std::cout << "💾 Đã lưu kiến thức AI vào: " << file << "\n";
    return true;
}

bool PatternRecognizer::loadKnowledge(const std::string& file) {
    std::ifstream in(file);
    if (!in.is_open()) return false;

    stats_.clear();
    std::string line, catName;
    CategoryStats current{};
    bool inCat = false;

    while (std::getline(in, line)) {
        line.erase(0, line.find_first_not_of(" \t"));

        if (line.find("\"") != std::string::npos && line.find(": {") != std::string::npos) {
            if (inCat && !catName.empty())
                stats_[stringToCategory(catName)] = current;

            size_t start = line.find("\"") + 1;
            size_t end = line.find("\"", start);
            catName = line.substr(start, end - start);
            current = CategoryStats{0, 0, 0.0, {}};
            inCat = true;
        }

        if (line.find("\"total\":") != std::string::npos)
            current.totalCount = std::stoi(line.substr(line.find(":") + 1));

        if (line.find("\"success\":") != std::string::npos)
            current.successCount = std::stoi(line.substr(line.find(":") + 1));

        if (line.find("\"rate\":") != std::string::npos)
            current.successRate = std::stod(line.substr(line.find(":") + 1));
    }

    if (inCat && !catName.empty())
        stats_[stringToCategory(catName)] = current;

    in.close();
    if (!stats_.empty()) {
        std::cout << "📂 Đã tải kiến thức AI từ: " << file << "\n";
        std::cout << "   Dạng toán: " << stats_.size() << "\n";
        return true;
    }
    return false;
}

// ==================== CHUYỂN STRING → CATEGORY ====================
MathCategory PatternRecognizer::stringToCategory(const std::string& s) const {
    if (s == "Số học") return MathCategory::ARITHMETIC;
    if (s == "Phương trình bậc 1") return MathCategory::ALGEBRA_LINEAR;
    if (s == "Phương trình bậc 2") return MathCategory::ALGEBRA_QUADRATIC;
    if (s == "Lượng giác") return MathCategory::TRIGONOMETRY;
    if (s == "Logarit") return MathCategory::LOGARITHM;
    if (s == "Lũy thừa") return MathCategory::POWER;
    if (s == "Hỗn hợp") return MathCategory::MIXED;
    return MathCategory::UNKNOWN;
}

bool PatternRecognizer::hasKnowledge() const {
    return !stats_.empty();
}

std::map<MathCategory, CategoryStats> PatternRecognizer::getStats() const {
    return stats_;
}

} // namespace ai
