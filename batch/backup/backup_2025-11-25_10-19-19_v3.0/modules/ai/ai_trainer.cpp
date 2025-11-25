#include "ai_trainer.h"

namespace ai {

// Hàm hiển thị progress bar đơn giản (không dùng thư viện)
static void showProgress(int current, int total, int errors = 0) {
    const int barWidth = 50;
    float progress = (float)current / total;
    int pos = (int)(barWidth * progress);
    
    std::cout << "\r📚 Đang học: [";
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) {
            std::cout << "█";
        } else {
            std::cout << "░";
        }
    }
    std::cout << "] " << (int)(progress * 100) << "% (" 
              << current << "/" << total << ")";
    
    if (errors > 0) {
        std::cout << " ❌ " << errors << " lỗi";
    }
    
    std::cout << std::flush;
}

void AITrainer::train() {
    auto history = logger.getHistory();

    if (history.empty()) {
        std::cout << "⚠️ Chưa có dữ liệu để huấn luyện. Hãy giải vài bài toán trước!\n";
        return;
    }

    std::cout << "🤖 Đang huấn luyện AI từ lịch sử...\n\n";
    
    // Hiển thị progress bar
    int total = history.size();
    int errors = 0;
    
    for (int i = 0; i < total; i++) {
        if (!history[i].success) errors++;
        showProgress(i + 1, total, errors);
    }
    
    std::cout << "\n\n";
    
    // Thực hiện training
    std::cout << "🧠 Đang phân tích patterns...\n";
    recognizer.learnFromHistory(history);
    recognizer.saveKnowledge();
    
    std::cout << "\n✅ Huấn luyện thành công!\n";
    std::cout << "   📊 Tổng số bài: " << total << "\n";
    std::cout << "   ✔️ Thành công: " << (total - errors) << "\n";
    std::cout << "   ❌ Lỗi: " << errors << "\n";
}

} // namespace ai
