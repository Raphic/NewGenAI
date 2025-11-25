#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "math_problem.h"  // Chứa struct MathProblem

namespace ai {

// Forward declaration để tránh include vòng
class IStorage;

class MathLogger {
public:
    // Constructor linh hoạt (có thể truyền backend hoặc để mặc định)
    explicit MathLogger(IStorage* storage = nullptr)
        : storage_(storage) {}

    // ======= GHI / ĐỌC DỮ LIỆU =======
    void log(const MathProblem& problem);
    std::vector<MathProblem> getHistory();

    // ======= THỐNG KÊ / BÁO CÁO =======
    void showStats();
    int getUniqueCount();

    // ======= DỌN DỮ LIỆU =======
    int removeDuplicates();  // Xóa bài trùng lặp
    void backupHistory();    // Backup trước khi cleanup

private:
    IStorage* storage_;  // Con trỏ backend lưu trữ (JSON, SQL, Remote...)
    static const std::string LOG_FILE; // File JSON mặc định
};

} // namespace ai
