#include "calculator.h"
#include "../ai/math_logger.h"
#include "../ai/math_problem.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <ctime>

namespace math {

// Hằng số
const double Calculator::PI = 3.14159265358979323846;
const double Calculator::E = 2.71828182845904523536;

// ==================== CONSTRUCTOR ====================
Calculator::Calculator(ai::MathLogger& loggerRef, bool silent) 
    : logger_(loggerRef), silent_(silent) {}

// ==================== PHÁT HIỆN LOẠI INPUT ====================
InputType Calculator::detectInputType(const std::string& input) {
    // Hệ phương trình (có dấu ;)
    if (input.find(';') != std::string::npos) {
        return InputType::SYSTEM_EQ;
    }
    
    // Có dấu '=' → Phương trình
    if (input.find('=') != std::string::npos) {
        if (input.find("x^2") != std::string::npos || input.find("x²") != std::string::npos) {
            return InputType::QUADRATIC_EQ;
        }
        if (input.find('x') != std::string::npos) {
            return InputType::LINEAR_EQ;
            }
    }
    return InputType::EXPRESSION;
}

// ==================== TÍNH TOÁN BIỂU THỨC NÂNG CAO ====================
double Calculator::evaluateAdvanced(const std::string& expression) {
    size_t pos = 0;
    return parseExpression(expression, pos);
}

void Calculator::skipSpaces(const std::string& expr, size_t& pos) {
    while (pos < expr.length() && std::isspace(expr[pos])) {
        pos++;
    }
}

// Phân tích biểu thức (cấp thấp nhất: +, -)
double Calculator::parseExpression(const std::string& expr, size_t& pos) {
    double result = parseTerm(expr, pos);
    
    while (pos < expr.length()) {
        skipSpaces(expr, pos);
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op == '+' || op == '-') {
            pos++;
            double right = parseTerm(expr, pos);
            result = (op == '+') ? result + right : result - right;
        } else {
            break;
        }
    }
    
    return result;
}

// Phân tích số hạng (*, /)
double Calculator::parseTerm(const std::string& expr, size_t& pos) {
    double result = parsePower(expr, pos);
    
    while (pos < expr.length()) {
        skipSpaces(expr, pos);
        if (pos >= expr.length()) break;
        
        char op = expr[pos];
        if (op == '*' || op == '/') {
            pos++;
            double right = parsePower(expr, pos);
            if (op == '*') {
                result *= right;
            } else {
                if (std::abs(right) < 1e-10) {
                    throw std::runtime_error("Chia cho 0");
                }
                result /= right;
            }
        } else {
            break;
        }
    }
    
    return result;
}

// Phân tích lũy thừa (^)
double Calculator::parsePower(const std::string& expr, size_t& pos) {
    double result = parseUnary(expr, pos);
    
    skipSpaces(expr, pos);
    if (pos < expr.length() && expr[pos] == '^') {
        pos++;
        double exponent = parsePower(expr, pos); // Right associative
        result = std::pow(result, exponent);
    }
    
    return result;
}

// Phân tích unary (+, -)
double Calculator::parseUnary(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    if (pos < expr.length() && (expr[pos] == '+' || expr[pos] == '-')) {
        char op = expr[pos];
        pos++;
        double value = parseUnary(expr, pos);
        return (op == '-') ? -value : value;
    }
    
    return parseFactor(expr, pos);
}

// Phân tích factor (số, hàm, biến, ngoặc)
double Calculator::parseFactor(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    // Ngoặc ()
    if (pos < expr.length() && expr[pos] == '(') {
        pos++;
        double result = parseExpression(expr, pos);
        skipSpaces(expr, pos);
        if (pos < expr.length() && expr[pos] == ')') {
            pos++;
        }
        return result;
    }
    
    // Hàm hoặc hằng số
    if (pos < expr.length() && std::isalpha(expr[pos])) {
        return parseFunction(expr, pos);
    }
    
    // Số
    return parseNumber(expr, pos);
}

// Phân tích hàm
double Calculator::parseFunction(const std::string& expr, size_t& pos) {
    std::string funcName = extractFunctionName(expr, pos);
    
    // Hằng số
    if (funcName == "pi") return PI;
    if (funcName == "e") return E;
    
    // Hàm cần đối số
    skipSpaces(expr, pos);
    if (pos < expr.length() && expr[pos] == '(') {
        pos++;
        double arg = parseExpression(expr, pos);
        skipSpaces(expr, pos);
        if (pos < expr.length() && expr[pos] == ')') {
            pos++;
        }
        return applyFunction(funcName, arg);
    }
    
    throw std::runtime_error("Hàm không hợp lệ: " + funcName);
}

std::string Calculator::extractFunctionName(const std::string& expr, size_t& pos) {
    std::string name;
    while (pos < expr.length() && std::isalpha(expr[pos])) {
        name += expr[pos];
        pos++;
    }
    return name;
}

double Calculator::applyFunction(const std::string& funcName, double arg) {
    // Lượng giác (đầu vào là độ)
    if (funcName == "sin") return std::sin(toRadians(arg));
    if (funcName == "cos") return std::cos(toRadians(arg));
    if (funcName == "tan") return std::tan(toRadians(arg));
    if (funcName == "asin") return std::asin(arg) * 180.0 / PI;
    if (funcName == "acos") return std::acos(arg) * 180.0 / PI;
    if (funcName == "atan") return std::atan(arg) * 180.0 / PI;
    
    // Căn bậc và logarit
    if (funcName == "sqrt") return std::sqrt(arg);
    if (funcName == "log") return std::log10(arg);
    if (funcName == "ln") return std::log(arg);
    if (funcName == "exp") return std::exp(arg);
    
    // Giá trị tuyệt đối và làm tròn
    if (funcName == "abs") return std::abs(arg);
    if (funcName == "ceil") return std::ceil(arg);
    if (funcName == "floor") return std::floor(arg);
    
    throw std::runtime_error("Hàm không xác định: " + funcName);
}

double Calculator::parseNumber(const std::string& expr, size_t& pos) {
    skipSpaces(expr, pos);
    
    std::string numStr;
    bool hasDecimal = false;
    
    while (pos < expr.length() && (std::isdigit(expr[pos]) || expr[pos] == '.')) {
        if (expr[pos] == '.') {
            if (hasDecimal) break;
            hasDecimal = true;
        }
        numStr += expr[pos];
        pos++;
    }
    
    if (numStr.empty()) {
        throw std::runtime_error("Thiếu số");
    }
    
    return std::stod(numStr);
}

double Calculator::toRadians(double degrees) {
    return degrees * PI / 180.0;
}

// ==================== GIẢI PHƯƠNG TRÌNH ====================
EquationResult Calculator::solveLinear(const std::string& equation) {
    EquationResult result;
    result.hasSolution = false;
    
    double a, b;
    if (!parseLinearCoeffs(equation, a, b)) {
        result.message = "Không thể parse phương trình";
        return result;
    }
    
    if (std::abs(a) < 1e-10) {
        if (std::abs(b) < 1e-10) {
            result.message = "Vô số nghiệm";
        } else {
            result.message = "Vô nghiệm";
        }
        return result;
    }
    
    double x = -b / a;
    result.hasSolution = true;
    result.solutions.push_back(x);
    result.message = "Có 1 nghiệm";
    
    return result;
}

EquationResult Calculator::solveQuadratic(const std::string& equation) {
    EquationResult result;
    result.hasSolution = false;
    
    double a, b, c;
    if (!parseQuadraticCoeffs(equation, a, b, c)) {
        result.message = "Không thể parse phương trình";
        return result;
    }
    
    if (std::abs(a) < 1e-10) {
        result.message = "Đây là PT bậc 1, không phải bậc 2";
        return result;
    }
    
    double delta = b * b - 4 * a * c;
    
    if (delta < -1e-10) {
        result.message = "Vô nghiệm (delta < 0)";
    } else if (std::abs(delta) < 1e-10) {
        double x = -b / (2 * a);
        result.hasSolution = true;
        result.solutions.push_back(x);
        result.message = "Nghiệm kép";
    } else {
        double x1 = (-b + std::sqrt(delta)) / (2 * a);
        double x2 = (-b - std::sqrt(delta)) / (2 * a);
        result.hasSolution = true;
        result.solutions.push_back(x1);
        result.solutions.push_back(x2);
        result.message = "Có 2 nghiệm phân biệt";
    }
    
    return result;
}

// Giải hệ phương trình 2 ẩn
EquationResult Calculator::solveSystem(const std::string& eq1, const std::string& eq2) {
    EquationResult result;
    result.hasSolution = false;
    
    // Parse eq1: a1*x + b1*y = c1
    // Parse eq2: a2*x + b2*y = c2
    // Giải bằng Cramer's rule
    
    // (Đơn giản hóa - chỉ hỗ trợ format chuẩn: x+y=5)
    // Bạn có thể mở rộng sau
    
    result.message = "Chức năng hệ PT đang phát triển";
    return result;
}

// ==================== PARSE HỆ SỐ ====================
bool Calculator::parseLinearCoeffs(const std::string& eq, double& a, double& b) {
    size_t pos = eq.find('=');
    if (pos == std::string::npos) return false;
    
    std::string left = eq.substr(0, pos);
    std::string right = eq.substr(pos + 1);
    
    left.erase(remove(left.begin(), left.end(), ' '), left.end());
    right.erase(remove(right.begin(), right.end(), ' '), right.end());
    
    a = 0;
    b = 0;
    
    try {
        b = -std::stod(right);
    } catch (...) {
        b = 0;
    }
    
    std::string term = "";
    int sign = 1;
    
    if (!left.empty() && left[0] != '+' && left[0] != '-') {
        left = "+" + left;
    }
    
    for (size_t i = 0; i < left.length(); i++) {
        char ch = left[i];
        
        if (ch == '+' || ch == '-') {
            if (!term.empty()) {
                double dummy_a = 0.0, dummy_c = 0.0;
                if (!processTerm(term, dummy_a, a, b, sign)) {
                    return false;
                }
                term = "";
            }
            sign = (ch == '+') ? 1 : -1;
        } else {
            term += ch;
        }
    }
    
    if (!term.empty()) {
        double dummy_a = 0.0, dummy_c = 0.0;
        if (!processTerm(term, dummy_a, a, b, sign)) {
            return false;
        }
    }
    
    return true;
}

bool Calculator::parseQuadraticCoeffs(const std::string& eq, double& a, double& b, double& c) {
    size_t pos = eq.find('=');
    if (pos == std::string::npos) return false;
    
    std::string left = eq.substr(0, pos);
    std::string right = eq.substr(pos + 1);
    
    left.erase(remove(left.begin(), left.end(), ' '), left.end());
    right.erase(remove(right.begin(), right.end(), ' '), right.end());
    
    a = 0;
    b = 0;
    c = 0;
    
    try {
        c = -std::stod(right);
    } catch (...) {
        c = 0;
    }
    
    std::string term = "";
    int sign = 1;
    
    if (!left.empty() && left[0] != '+' && left[0] != '-') {
        left = "+" + left;
    }
    
    for (size_t i = 0; i < left.length(); i++) {
        char ch = left[i];
        
        if (ch == '+' || ch == '-') {
            if (!term.empty()) {
                if (!processTerm(term, a, b, c, sign)) {
                    return false;
                }
                term = "";
            }
            sign = (ch == '+') ? 1 : -1;
        } else {
            term += ch;
        }
    }
    
    if (!term.empty()) {
        if (!processTerm(term, a, b, c, sign)) {
            return false;
        }
    }
    
    return true;
}


bool Calculator::processTerm(const std::string& term, double& a, double& b, double& c, int sign) {
    std::string t = term;
    t.erase(remove_if(t.begin(), t.end(), ::isspace), t.end());
    if (t.find("x^2") != std::string::npos || t.find("x²") != std::string::npos) {
        size_t xPos = t.find('x');
        std::string coeff = t.substr(0, xPos);
        
        if (coeff.empty()) {
            a += sign * 1;
        } else {
            try {
                a += sign * std::stod(coeff);
            } catch (...) {
                return false;
            }
        }
    } else if (t.find('x') != std::string::npos) {
        size_t xPos = t.find('x');
        std::string coeff = t.substr(0, xPos);
        
        if (coeff.empty()) {
            b += sign * 1;
        } else {
            try {
                b += sign * std::stod(coeff);
            } catch (...) {
                return false;
            }
        }
    } else {
        try {
            c += sign * std::stod(t);
        } catch (...) {
            return false;
        }
    }
    
    return true;
}

// ==================== XỬ LÝ INPUT TỔNG HỢP ====================
void Calculator::process(const std::string& input) {
    // Kiểm tra input rỗng
    if (input.empty()) {
        std::cout << "⚠️ Vui lòng nhập biểu thức\n";
        return;
    }
    
    // Phát hiện loại input
    InputType type = detectInputType(input);
    
    // Biến để log
    ai::MathProblem logEntry;
    logEntry.input = input;
    logEntry.timestamp = std::time(nullptr);
    
    try {
        switch (type) {
            case InputType::EXPRESSION: {
                double result = evaluateAdvanced(input);
                if (!silent_) {
                    std::cout << "🔢 Kết quả = " << result << "\n";
                }
                
                // Ghi log
                logEntry.type = "expression";
                logEntry.result = std::to_string(result);
                logEntry.success = true;
                break;
            }
            
            case InputType::LINEAR_EQ: {
                EquationResult result = solveLinear(input);
                if (!silent_) {
                    std::cout << "\n📐 PHƯƠNG TRÌNH BẬC 1\n";
                    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                    std::cout << "   " << result.message << "\n";
                    if (result.hasSolution && !result.solutions.empty()) {
                        std::cout << "   x = " << result.solutions[0] << "\n";
                    }
                    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                }
                
                // Ghi log
                logEntry.type = "linear_equation";
                logEntry.result = result.message;
                logEntry.success = result.hasSolution;
                break;
            }
            
            case InputType::QUADRATIC_EQ: {
                EquationResult result = solveQuadratic(input);
                if (!silent_) {
                    std::cout << "\n📐 PHƯƠNG TRÌNH BẬC 2\n";
                    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                    std::cout << "   " << result.message << "\n";
                    if (result.hasSolution) {
                        if (result.solutions.size() == 1) {
                            std::cout << "   x = " << result.solutions[0] << "\n";
                        } else {
                            std::cout << "   x₁ = " << result.solutions[0] << "\n";
                            std::cout << "   x₂ = " << result.solutions[1] << "\n";
                        }
                    }
                    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                }
                
                // Ghi log
                logEntry.type = "quadratic_equation";
                logEntry.result = result.message;
                logEntry.success = result.hasSolution;
                break;
            }
            
            case InputType::SYSTEM_EQ: {
                size_t pos = input.find(';');
                std::string eq1 = input.substr(0, pos);
                std::string eq2 = input.substr(pos + 1);
                EquationResult result = solveSystem(eq1, eq2);
                if (!silent_) {
                    std::cout << "\n📐 HỆ PHƯƠNG TRÌNH\n";
                    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                    std::cout << "   " << result.message << "\n";
                    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                }
                
                // Ghi log
                logEntry.type = "system_equation";
                logEntry.result = result.message;
                logEntry.success = result.hasSolution;
                break;
            }
            default: {  // 🔹 thêm khối này
                std::cout << "⚠️ Không nhận dạng được loại biểu thức\n";
                logEntry.type = "unknown";
                logEntry.result = "Unknown input type";
                logEntry.success = false;
                break;
            }
        }
        
        // ✅ Ghi log sau khi xử lý thành công - dùng logger_ như DataExporter
        logger_.log(logEntry);
        
    } catch (const std::exception& e) {
        std::cout << "❌ Lỗi: " << e.what() << "\n";
        
        // Ghi log lỗi
        logEntry.type = "error";
        logEntry.result = e.what();
        logEntry.success = false;
        logger_.log(logEntry);  // ✅ Dùng logger_
        
    } catch (...) {
        std::cout << "❌ Lỗi không xác định\n";
        
        // Ghi log lỗi không xác định
        logEntry.type = "unknown_error";
        logEntry.result = "Unknown exception";
        logEntry.success = false;
        logger_.log(logEntry);  // ✅ Dùng logger_
    }
}

// ==================== HƯỚNG DẪN NÂNG CAO ====================
void Calculator::showAdvancedHelp() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         🧮 HƯỚNG DẪN MÁY TÍNH NÂNG CAO                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    
    std::cout << "📌 PHÉP TÍNH CƠ BẢN:\n";
    std::cout << "   +, -, *, /          Cộng, trừ, nhân, chia\n";
    std::cout << "   ^                   Lũy thừa (2^3 = 8)\n";
    std::cout << "   ()                  Ngoặc đơn\n";
    std::cout << "   Ví dụ: 5 + 3 * (10 - 4)^2\n";
    std::cout << "\n";
    
    std::cout << "📌 HÀM LƯỢNG GIÁC (đầu vào là ĐỘ):\n";
    std::cout << "   sin(30)             Sin 30°\n";
    std::cout << "   cos(45)             Cos 45°\n";
    std::cout << "   tan(60)             Tan 60°\n";
    std::cout << "   asin(0.5)           Arcsin (kết quả: độ)\n";
    std::cout << "   acos(0.5)           Arccos\n";
    std::cout << "   atan(1)             Arctan\n";
    std::cout << "\n";
    
    std::cout << "📌 HÀM TOÁN HỌC:\n";
    std::cout << "   sqrt(16)            Căn bậc 2\n";
    std::cout << "   log(100)            Logarit cơ số 10\n";
    std::cout << "   ln(e)               Logarit tự nhiên\n";
    std::cout << "   exp(2)              e^2\n";
    std::cout << "   abs(-5)             Giá trị tuyệt đối\n";
    std::cout << "   ceil(3.2)           Làm tròn lên\n";
    std::cout << "   floor(3.8)          Làm tròn xuống\n";
    std::cout << "\n";
    
    std::cout << "📌 HẰNG SỐ:\n";
    std::cout << "   pi                  3.14159...\n";
    std::cout << "   e                   2.71828...\n";
    std::cout << "   Ví dụ: 2*pi*5, e^2\n";
    std::cout << "\n";
    
    std::cout << "📌 PHƯƠNG TRÌNH BẬC 1:\n";
    std::cout << "   2x + 5 = 0\n";
    std::cout << "   -3x = 9\n";
    std::cout << "   5x - 10 = 0\n";
    std::cout << "\n";
    
    std::cout << "📌 PHƯƠNG TRÌNH BẬC 2:\n";
    std::cout << "   x^2 - 5x + 6 = 0\n";
    std::cout << "   2x^2 + 3x - 5 = 0\n";
    std::cout << "   x^2 = 4\n";
    std::cout << "   -x^2 + 4x - 3 = 0\n";
    std::cout << "\n";
    
    std::cout << "📌 VÍ DỤ TỔNG HỢP:\n";
    std::cout << "   sin(30) + cos(60)^2\n";
    std::cout << "   sqrt(16) * log(100)\n";
    std::cout << "   (2 + 3)^2 / sqrt(25)\n";
    std::cout << "   2*pi*5 + e^2\n";
    std::cout << "\n";
    
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "💡 Gõ 'clear' để xóa màn hình\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "\n";
}

} // namespace math