#pragma once
#include <string>
#include "math_logger.h"

namespace ai {

class DataExporter {
public:
    // ✅ Truyền logger khi khởi tạo (giống DataImporter)
    explicit DataExporter(ai::MathLogger& loggerRef)
        : logger_(loggerRef) {}

    // Xuất dữ liệu ra CSV
    void exportToCSV(const std::string& outputFile);

    // Hiển thị thống kê tóm tắt
    void showStatsSummary();

private:
    ai::MathLogger& logger_;  // Giữ tham chiếu tới logger
};

} // namespace ai
