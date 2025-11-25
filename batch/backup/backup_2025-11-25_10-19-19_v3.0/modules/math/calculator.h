#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <map>

// Forward declarations
namespace ai {
    class MathLogger;
}

namespace math {

// Enum phân loại input
enum class InputType {
    EXPRESSION,      // Biểu thức: 5 + 3 * 2
    LINEAR_EQ,       // PT bậc 1: 2x + 5 = 0
    QUADRATIC_EQ,    // PT bậc 2: x^2 - 5x + 6 = 0
    SYSTEM_EQ        // Hệ PT: x+y=5; x-y=1
};

// Kết quả giải phương trình
struct EquationResult {
    bool hasSolution;
    std::vector<double> solutions;
    std::string message;
    std::map<std::string, double> variables; // Cho hệ phương trình
};

class Calculator {
public:
    // ✅ Constructor nhận logger reference (giống DataExporter)
    explicit Calculator(ai::MathLogger& loggerRef);
    explicit Calculator(ai::MathLogger& loggerRef, bool silent = false);
    // Tính toán biểu thức nâng cao
    double evaluateAdvanced(const std::string& expression);
    
    // Phát hiện loại input
    InputType detectInputType(const std::string& input);
    
    // Giải phương trình
    EquationResult solveLinear(const std::string& equation);
    EquationResult solveQuadratic(const std::string& equation);
    EquationResult solveSystem(const std::string& eq1, const std::string& eq2);
    
    // Xử lý input tổng hợp
    void process(const std::string& input);
    
    // Hiển thị hướng dẫn chi tiết (static vì không cần logger)
    static void showAdvancedHelp();
    
private:
    ai::MathLogger& logger_;  // ✅ Member variable như DataExporter
    bool silent_;  // Chế độ im lặng khi batch import
    // Parser biểu thức
    double parseExpression(const std::string& expr, size_t& pos);
    double parseTerm(const std::string& expr, size_t& pos);
    double parseFactor(const std::string& expr, size_t& pos);
    double parsePower(const std::string& expr, size_t& pos);
    double parseUnary(const std::string& expr, size_t& pos);
    double parseFunction(const std::string& expr, size_t& pos);
    double parseNumber(const std::string& expr, size_t& pos);
    
    // Hàm hỗ trợ
    void skipSpaces(const std::string& expr, size_t& pos);
    std::string extractFunctionName(const std::string& expr, size_t& pos);
    double applyFunction(const std::string& funcName, double arg);
    
    // Parse phương trình
    bool parseLinearCoeffs(const std::string& eq, double& a, double& b);
    bool parseQuadraticCoeffs(const std::string& eq, double& a, double& b, double& c);
    bool processTerm(const std::string& term, double& a, double& b, double& c, int sign);
    
    // Hằng số
    static const double PI;
    static const double E;
    
    // Convert độ sang radian
    static double toRadians(double degrees);
};

} // namespace math

#endif