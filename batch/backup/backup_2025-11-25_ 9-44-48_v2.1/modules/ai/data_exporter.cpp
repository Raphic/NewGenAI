#include "data_exporter.h"
#include <fstream>
#include <iostream>
#include <map>

namespace ai {

void DataExporter::exportToCSV(const std::string& outputFile) {
    auto history = logger_.getHistory();

    std::ofstream csv(outputFile);
    if (!csv.is_open()) {
        std::cerr << "❌ Không thể tạo file CSV\n";
        return;
    }

    csv << "Input,Type,Result,Success,Timestamp\n";

    for (const auto& problem : history) {
        csv << "\"" << problem.input << "\",";
        csv << "\"" << problem.type << "\",";
        csv << "\"" << problem.result << "\",";
        csv << (problem.success ? "true" : "false") << ",";
        csv << problem.timestamp << "\n";
    }

    csv.close();
    std::cout << "✅ Đã xuất " << history.size()
              << " bài toán ra " << outputFile << "\n";
}

void DataExporter::showStatsSummary() {
    auto history = logger_.getHistory();

    if (history.empty()) {
        std::cout << "⚠️ Chưa có dữ liệu\n";
        return;
    }

    std::map<std::string, int> typeCounts;
    int successCount = 0;

    for (const auto& problem : history) {
        typeCounts[problem.type]++;
        if (problem.success) successCount++;
    }

    std::cout << "\n📊 TỔNG QUAN DỮ LIỆU\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Tổng số bài: " << history.size() << "\n";
    std::cout << "Thành công: " << successCount << " ("
              << (successCount * 100.0 / history.size()) << "%)\n";
    std::cout << "Thất bại: " << (history.size() - successCount) << "\n\n";
    std::cout << "Phân loại:\n";

    for (const auto& pair : typeCounts) {
        std::cout << "  • " << pair.first << ": " << pair.second << " bài\n";
    }

    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
}

} // namespace ai
