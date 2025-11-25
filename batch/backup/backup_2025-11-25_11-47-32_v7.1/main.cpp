#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "modules/math/calculator.h"
#include "modules/system/command_handler.h"
#include "modules/system/command_registry.h"
#include "modules/utils/common.h"
#include "modules/ai/math_logger.h"
#include "modules/ai/pattern_recognizer.h"
#include "modules/ai/data_exporter.h"
#include "modules/ai/data_importer.h"
#include "modules/ai/storage_json.h"
#include "modules/ai/data_cleaner.h"
#include "modules/ai/ai_trainer.h"

using namespace ai;
// ======== HÀM ĐỌC VERSION ========
std::string getVersion() {
    std::ifstream file("data/version.txt");
    if (file.is_open()) {
        std::string version;
        std::getline(file, version);
        file.close();
        return version;
    }
    return "0.0";
}

// Thêm helper trim an toàn
static inline void trimString(std::string &s) {
    const char* ws = " \t\n\r\f\v";
    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos) { s.clear(); return; }
    size_t end = s.find_last_not_of(ws);
    s = s.substr(start, end - start + 1);
}

int main() {
    utils::setConsoleUTF8();

    std::string version = getVersion();
    utils::printHeader("ADVANCED MODULAR AI SYSTEM === (v" + version + ")");

    // ======== KHỞI TẠO MODULES ========
    JsonStorage storage("data/math_history.json");
    MathLogger logger(&storage);
    PatternRecognizer recognizer;
    DataImporter importer(logger);
    DataExporter exporter(logger);
    math::Calculator calc(logger);

    std::string lastInput;
    
    // ✅ ĐỊNH NGHĨA TẤT CẢ LỆNH Ở ĐÂY - CHỈ SỬA 1 CHỖ!
    auto runCommand = [&](const std::string& input) -> int {
        DEFINE_COMMAND(exit, "Thoát chương trình") {
            std::cout << "👋 Tạm biệt!\n";
            return 2; // signal to exit
        }
        
        DEFINE_COMMAND(help, "Hiển thị danh sách lệnh") {
            auto cmds = sys::CommandRegistry::getAll();
            std::cout << "\n╔════════════════════════════════════════════╗\n";
            std::cout << "║        DANH SÁCH LỆNH HỆ THỐNG             ║\n";
            std::cout << "╚════════════════════════════════════════════╝\n\n";
            std::cout << "📝 CÁC LỆNH KHẢ DỤNG:\n\n";
            size_t max_len = 10;
            for (const auto& cmd : cmds) {
                if (cmd.name.length() > max_len) max_len = cmd.name.length();
            }
            for (const auto& cmd : cmds) {
                std::cout << "   " << cmd.name;
                std::cout << std::string(max_len + 2 - cmd.name.size(), ' ');
                std::cout << "- " << cmd.description << "\n";
            }
            std::cout << "\n🧮 TÍNH TOÁN: Nhập biểu thức toán học\n";
            std::cout << "   Ví dụ: 2+2, sqrt(16), sin(30), x^2+3x-4=0\n\n";
            return 1; //đã xử lý
        }
        
        DEFINE_COMMAND(calculator, "Hướng dẫn các dạng toán") {
            math::Calculator::showAdvancedHelp();
            return 1;
        }
        
        DEFINE_COMMAND(stats, "Thống kê sử dụng") {
            logger.showStats();
            return 1;
        }
        
        DEFINE_COMMAND(clear, "Xóa màn hình") {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            utils::printHeader("ADVANCED MODULAR AI SYSTEM === (v" + version + ")");
            return 1;
        }
        
        DEFINE_COMMAND(train, "Huấn luyện AI") {
            ai::AITrainer trainer(logger, recognizer);
            trainer.train();
            return 1;
        }
        
        DEFINE_COMMAND(save, "Lưu dữ liệu đã học") {
            if (!recognizer.hasKnowledge()) {
                std::cout << "⚠️ AI chưa học gì. Gõ 'train' trước!\n";
            } else {
                recognizer.saveKnowledge();
            }
            return 1;
        }
        
        DEFINE_COMMAND(load, "Tải dữ liệu đã học") {
            if (recognizer.loadKnowledge()) {
                std::cout << "✅ Đã tải kiến thức thành công\n";
            } else {
                std::cout << "❌ Không tìm thấy file knowledge\n";
            }
            return 1;
        }
        
        DEFINE_COMMAND(clean, "Xoá các bài toán trùng lặp") {
            ai::DataCleaner cleaner(logger);
            cleaner.cleanup();
            return 1;
        }
        
        DEFINE_COMMAND(analyze, "Thống kê dạng toán") {
            recognizer.showCategoryStats();
            return 1;
        }
        
        DEFINE_COMMAND(suggest, "Gợi ý phương pháp giải") {
            if (lastInput.empty()) {
                std::cout << "⚠️ Hãy nhập một bài toán trước!\n";
            } else {
                std::cout << "\n📘 Gợi ý cho: " << lastInput << "\n";
                std::cout << recognizer.suggestMethod(lastInput) << "\n\n";
                auto similar = recognizer.findSimilarProblems(lastInput, 3);
                if (!similar.empty()) {
                    std::cout << "🔍 Bài toán tương tự:\n";
                    for (const auto& s : similar)
                        std::cout << "   • " << s << "\n";
                    std::cout << "\n";
                }
            }
            return 1;
        }
        
        DEFINE_COMMAND(import, "Nhập dữ liệu (tự động train AI)") {
            bool hasNewData = importer.smartImport();
            
            if (!hasNewData) {
                std::cout << "\n✅ Không có dữ liệu mới\n";
                std::cout << "💡 Bỏ qua cleanup và train\n\n";
                return 1;
            }
            
            // Tự động clean duplicates
            std::cout << "\n🧹 Đang kiểm tra duplicate TRONG history...\n";
            int removed = logger.removeDuplicates();
            if (removed <= 0) {
                std::cout << "✅ Không tìm thấy bài trùng (history sạch)\n";
            }
            // Tự động train AI
            std::cout << "\n🤖 Đang train AI từ dữ liệu mới...\n";
            ai::AITrainer trainer(logger, recognizer);
            trainer.train();
            
            std::cout << "\n✅ Hoàn tất! AI đã học xong.\n";
            std::cout << "💡 Gõ tên bài toán để test AI\n\n";
            return 1;
        }
        
        DEFINE_COMMAND(export, "Xuất lịch sử toán ra CSV") {
            exporter.exportToCSV("data/export/math_history.csv");
            return 1;
        }
        
        DEFINE_COMMAND(summary, "Tổng hợp thống kê dữ liệu") {
            exporter.showStatsSummary();
            return 1;
        }
        
        return 0; // Không match lệnh nào
    };
    
    // ✅ Gọi 1 lần để force initialize tất cả static variables (đăng ký lệnh)
    runCommand("");
    
    // ======== TẢI KIẾN THỨC AI ========
    if (recognizer.loadKnowledge()) {
        std::cout << "✅ AI đã sẵn sàng (đã tải kiến thức trước đó)\n";
    } else {
        std::cout << "💡 AI chưa học. Gõ 'import' để nhập dữ liệu, sau đó 'train'\n";
    }
    std::cout << "\n";

    // ======== VÒNG LẶP LỆNH ========
    while (true) {
        std::cout << "Bạn : ";
        std::string input;
        std::getline(std::cin, input);

        // Trim spaces (dùng helper an toàn)
        trimString(input);

        if (input.empty()) continue;

        // ✅ Xử lý lệnh qua lambda
        int commandResult = runCommand(input);
        if (commandResult == 2) break; // Exit
        if (commandResult == 1) continue; // Lệnh đã xử lý, skip phần giải toán
        
        // ======== LỆNH CLASSIFY (có tham số) ========
        if (input.rfind("classify ", 0) == 0) {
            std::string problem = input.substr(9);
            auto category = recognizer.classify(problem);
            auto features = recognizer.extractFeatures(problem);

            std::cout << "\n🔬 PHÂN TÍCH DẠNG TOÁN\n";
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << "Bài toán: " << problem << "\n";
            std::cout << "Dạng: " << recognizer.categoryToString(category) << "\n";
            std::cout << "\nĐặc trưng:\n";
            std::cout << "  • Có biến: " << (features.hasVariable ? "Có" : "Không") << "\n";
            std::cout << "  • Có x²: " << (features.hasSquare ? "Có" : "Không") << "\n";
            std::cout << "  • Có lượng giác: " << (features.hasTrig ? "Có" : "Không") << "\n";
            std::cout << "  • Có logarit: " << (features.hasLog ? "Có" : "Không") << "\n";
            std::cout << "  • Có lũy thừa: " << (features.hasPower ? "Có" : "Không") << "\n";
            std::cout << "  • Là phương trình: " << (features.hasEquation ? "Có" : "Không") << "\n";
            std::cout << "  • Số phép toán: " << features.operatorCount << "\n";

            if (!features.keywords.empty()) {
                std::cout << "  • Từ khóa: ";
                for (const auto& kw : features.keywords)
                    std::cout << kw << " ";
                std::cout << "\n";
            }
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
            continue;
        }
        
        // ======== GIẢI TOÁN ========
        lastInput = input;

        auto category = recognizer.classify(input);
        std::cout << "🤖 AI nhận dạng: " << recognizer.categoryToString(category) << "\n";

        calc.process(input);
        std::cout << "\n💡 Gõ 'suggest' để xem hướng dẫn chi tiết\n";
    }

    return 0;
}