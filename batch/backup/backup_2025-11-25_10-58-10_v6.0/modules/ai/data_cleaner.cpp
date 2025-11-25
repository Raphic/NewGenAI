#include "data_cleaner.h"

namespace ai {

void DataCleaner::cleanup(bool autoBackup) {
    std::cout << "⚠️ Sẽ xóa các bài toán trùng lặp trong lịch sử.\n";
    if (autoBackup)
        std::cout << "   File backup sẽ được tạo tự động.\n";
    std::cout << "   Tiếp tục? (y/n): ";

    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "y" || confirm == "Y") {
        if (autoBackup) logger.backupHistory();
        int removed = logger.removeDuplicates();

        if (removed > 0) {
            std::cout << "💡 Gõ 'train' để AI học lại từ dữ liệu đã làm sạch.\n";
        } else {
            std::cout << "✅ Không có bài toán trùng lặp nào cần xóa.\n";
        }
    } else {
        std::cout << "❌ Đã hủy thao tác.\n";
    }
}

} // namespace ai
