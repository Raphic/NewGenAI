#include "ai_trainer.h"

namespace ai {

void AITrainer::train() {
    auto history = logger.getHistory();

    if (history.empty()) {
        std::cout << "⚠️ Chưa có dữ liệu để huấn luyện. Hãy giải vài bài toán trước!\n";
        return;
    }

    std::cout << "🤖 Đang huấn luyện AI từ lịch sử...\n";
    recognizer.learnFromHistory(history);
    recognizer.saveKnowledge();
    std::cout << "✅ Huấn luyện thành công! Dữ liệu đã được lưu.\n";
}

} // namespace ai
