#include "data_importer.h"
#include "math_logger.h"
#include "../math/calculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace fs = std::filesystem;

namespace ai {

const std::string DataImporter::IMPORT_FOLDER = "data/import";
const std::string DataImporter::LOG_FILE = "data/imported_files.log";
// ==================== PROGESS BAR ====================
static void showImportProgress(const std::string& filename, int current, int total,
                                int skipped, int solved, int failed) {
    const int barWidth = 50;
    float progress = total > 0 ? (float)current / total : 0;
    int pos = (int)(barWidth * progress);
    
    // Clear và vẽ lại toàn bộ (3 dòng)
    std::cout << "\r\033[2K";  // Clear current line
    std::cout << "\033[1A\033[2K";  // Up 1 line and clear
    std::cout << "\033[1A\033[2K";  // Up 1 line and clear
    
    // Dòng 1: Tên file
    std::cout << filename << "\n";
    
    // Dòng 2: Progress bar
    std::cout << "[";
    for (int i = 0; i < barWidth; i++) {
        std::cout << (i < pos ? "█" : "░");
    }
    std::cout << "] " << (int)(progress * 100) << "% (" 
              << current << "/" << total << ")\n";
    
    // Dòng 3: Stats
    std::cout << "⏭️  Bỏ qua: " << skipped 
              << " 🆕 Giải mới: " << (solved + failed)
              << " ✅ Thành công: " << solved 
              << " ❌ Thất bại: " << failed << std::flush;
}
// ==================== NORMALIZE STRING ====================
std::string DataImporter::normalizeForComparison(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (!std::isspace(c)) {
            result += std::tolower(c);
        }
    }
    return result;
}

// ==================== LẤY DANH SÁCH BÀI ĐÃ GIẢI ====================
std::set<std::string> DataImporter::getExistingProblems() {
    std::set<std::string> existing;
    auto history = logger_.getHistory();
    
    for (const auto& problem : history) {
        existing.insert(normalizeForComparison(problem.input));
    }
    
    return existing;
}

// ==================== IMPORT & SOLVE CSV ====================
ImportStats DataImporter::importAndSolveCSV(const std::string& csvFile, bool skipDuplicates) {
    ImportStats stats = {0, 0, 0, 0, 0};
    
    std::cout << "📥 Đang import từ " << csvFile << "...\n";
    
    auto data = readCSV(csvFile);
    if (data.empty()) {
        std::cout << "❌ File rỗng hoặc không đọc được\n";
        return stats;
    }
    
    // Chuyển sang vector string
    std::vector<std::string> problems;
    for (const auto& td : data) {
        problems.push_back(td.problem);
    }
    
    // GỌI batchSolve
    return batchSolve(problems, skipDuplicates, csvFile);
}

// ==================== IMPORT & SOLVE TEXT ====================
ImportStats DataImporter::importAndSolveText(const std::string& textFile, bool skipDuplicates) {
    std::cout << "📥 Đang import từ " << textFile << "...\n";
    
    auto problems = readTextFile(textFile);
    if (problems.empty()) {
        std::cout << "❌ File rỗng\n";
        return ImportStats{0, 0, 0, 0, 0};
    }
    
    return batchSolve(problems, skipDuplicates, textFile);
}

// ==================== BATCH SOLVE (PHƯƠNG ÁN 3) ====================
ImportStats DataImporter::batchSolve(const std::vector<std::string>& problems, bool skipDuplicates, const std::string& filename) {
    ImportStats stats = {0, 0, 0, 0, 0};
    
    if (problems.empty()) return stats;
    
    stats.totalFound = problems.size();
    std::cout << "📚 Tổng số bài: " << problems.size() << "\n";
    
    if (skipDuplicates) {
        std::cout << "🔍 Chế độ: Vừa giải vừa check trùng\n\n";
    } else {
        std::cout << "⚠️ Chế độ: Giải tất cả (kể cả trùng)\n\n";
    }
    
    // LẤY DANH SÁCH ĐÃ GIẢI
    std::set<std::string> existingProblems;
    if (skipDuplicates) {
        existingProblems = getExistingProblems();
    }
    
    math::Calculator calc(logger_);
    calc.setSilentMode(true);  // TẮT output
    // In header 1 lần
    std::cout << "\n" << filename << "\n";
    std::cout << std::string(50, '░') << " 0% (0/" << problems.size() << ")\n";
    std::cout << "⏭️  Bỏ qua: 0 🆕 Giải mới: 0 ✅ Thành công: 0 ❌ Thất bại: 0\n";
    int processedCount = 0;
    
    for (size_t i = 0; i < problems.size(); i++) {
        std::string problem = problems[i];
        
        // Trim
        problem.erase(0, problem.find_first_not_of(" \t\r\n"));
        problem.erase(problem.find_last_not_of(" \t\r\n") + 1);
        
        if (problem.empty() || problem[0] == '#') continue;
        
        // CHECK TRÙNG
        if (skipDuplicates) {
            std::string normalized = normalizeForComparison(problem);
            if (existingProblems.find(normalized) != existingProblems.end()) {
                stats.duplicates++;
                showImportProgress(filename, i + 1, problems.size(), 
                                stats.duplicates, stats.solved, stats.failed);
                continue;
            }
            existingProblems.insert(normalized);
        }
        
        // GIẢI BÀI
        stats.newProblems++;
        try {
            calc.process(problem);
            stats.solved++;
        } catch (...) {
            stats.failed++;
        }
        
        showImportProgress(filename, i + 1, problems.size(), 
                        stats.duplicates, stats.solved, stats.failed);
    }
    std::cout << "\n\n";  // Xuống dòng sau progress bar
    
    // THỐNG KÊ
    // std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    // std::cout << "📊 KẾT QUẢ:\n";
    // std::cout << "   Tổng tìm thấy: " << stats.totalFound << "\n";
    // if (skipDuplicates && stats.duplicates > 0) {
    //     std::cout << "   ⏭️  Bỏ qua (trùng): " << stats.duplicates << "\n";
    // }
    // std::cout << "   🆕 Giải mới: " << stats.newProblems << "\n";
    // std::cout << "   ✅ Thành công: " << stats.solved << "\n";
    // std::cout << "   ❌ Thất bại: " << stats.failed << "\n";
    // std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    return stats;
}

// ==================== ĐỌC CSV ====================
std::vector<TrainingData> DataImporter::readCSV(const std::string& csvFile) {
    std::vector<TrainingData> data;
    std::ifstream file(csvFile);
    
    if (!file.is_open()) {
        std::cerr << "❌ Không thể mở file: " << csvFile << "\n";
        return data;
    }
    
    std::string line;
    bool isFirstLine = true;
    
    while (std::getline(file, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            if (line.find("problem") != std::string::npos) continue;
        }
        
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string problem, answer, category;
        
        if (line[0] == '"') {
            size_t endQuote = line.find('"', 1);
            problem = line.substr(1, endQuote - 1);
            
            if (endQuote + 1 < line.length()) {
                std::string rest = line.substr(endQuote + 2);
                std::stringstream restSS(rest);
                std::getline(restSS, answer, ',');
                std::getline(restSS, category, ',');
            }
        } else {
            std::getline(ss, problem, ',');
            std::getline(ss, answer, ',');
            std::getline(ss, category, ',');
        }
        
        TrainingData td;
        td.problem = problem;
        td.answer = answer;
        td.category = category;
        data.push_back(td);
    }
    
    file.close();
    return data;
}

// ==================== ĐỌC TEXT ====================
std::vector<std::string> DataImporter::readTextFile(const std::string& textFile) {
    std::vector<std::string> problems;
    std::ifstream file(textFile);
    
    if (!file.is_open()) {
        std::cerr << "❌ Không thể mở file: " << textFile << "\n";
        return problems;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (!line.empty() && line[0] != '#') {
            problems.push_back(line);
        }
    }
    
    file.close();
    return problems;
}

// ==================== ĐẾM BÀI ====================
int DataImporter::countProblems(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return 0;
    
    int count = 0;
    std::string line;
    bool isFirstLine = true;
    
    while (std::getline(file, line)) {
        if (isFirstLine && line.find("problem") != std::string::npos) {
            isFirstLine = false;
            continue;
        }
        isFirstLine = false;
        
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (!line.empty() && line[0] != '#') count++;
    }
    
    file.close();
    return count;
}

// ==================== QUÉT THƯ MỤC ====================
std::vector<FileInfo> DataImporter::scanImportFolder() {
    std::vector<FileInfo> files;
    
    if (!fs::exists(IMPORT_FOLDER)) {
        fs::create_directories(IMPORT_FOLDER);
        std::cout << "📁 Đã tạo thư mục: " << IMPORT_FOLDER << "\n";
        return files;
    }
    
    auto importedFiles = getImportedFiles();
    
    for (const auto& entry : fs::directory_iterator(IMPORT_FOLDER)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            std::string ext = entry.path().extension().string();
            
            if (ext == ".txt" || ext == ".csv" || ext == ".json") {
                FileInfo info;
                info.filename = filename;
                info.fullPath = entry.path().string();
                info.imported = (importedFiles.find(filename) != importedFiles.end());
                info.problemCount = countProblems(info.fullPath);
                files.push_back(info);
            }
        }
    }
    
    std::sort(files.begin(), files.end(), 
              [](const FileInfo& a, const FileInfo& b) {
                  if (a.imported != b.imported) return !a.imported;
                  return a.filename < b.filename;
              });
    
    return files;
}

std::set<std::string> DataImporter::getImportedFiles() {
    std::set<std::string> imported;
    std::ifstream file(LOG_FILE);
    
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) imported.insert(line);
        }
        file.close();
    }
    
    return imported;
}

void DataImporter::markAsImported(const std::string& filename) {
    std::ofstream file(LOG_FILE, std::ios::app);
    if (file.is_open()) {
        file << filename << "\n";
        file.close();
    }
}

void DataImporter::resetImportLog() {
    std::ofstream file(LOG_FILE, std::ios::trunc);
    file.close();
    std::cout << "✅ Đã xóa lịch sử import.\n";
}

// ==================== SMART IMPORT UI ====================
void DataImporter::smartImport() {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              📚 SMART IMPORT SYSTEM                        ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "🔍 Đang quét " << IMPORT_FOLDER << "...\n\n";
    auto files = scanImportFolder();
    
    if (files.empty()) {
        std::cout << "⚠️ Không tìm thấy file!\n💡 Đặt file vào: " << IMPORT_FOLDER << "\n";
        return;
    }
    
    std::cout << "📋 DANH SÁCH:\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    int newCount = 0, totalProbs = 0, newProbs = 0;
    for (size_t i = 0; i < files.size(); i++) {
        const auto& f = files[i];
        std::cout << "  [" << (i+1) << "] " << (f.imported ? "✅" : "🆕") << " │ "
                  << f.filename << " (" << f.problemCount << " bài)\n";
        
        totalProbs += f.problemCount;
        if (!f.imported) {
            newCount++;
            newProbs += f.problemCount;
        }
    }
    
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📊 Tổng: " << files.size() << " file | " << totalProbs << " bài\n";
    std::cout << "   🆕 Mới: " << newCount << " file | " << newProbs << " bài\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    std::cout << "📌 CHỌN:\n";
    std::cout << "  1. Học file mới (" << newCount << ")\n";
    std::cout << "  2. Chọn file cụ thể\n";
    std::cout << "  3. Học lại toàn bộ\n";
    std::cout << "  4. Xóa lịch sử\n";
    std::cout << "  0. Hủy\n\n";
    std::cout << "Chọn: ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "1") {
        if (newCount == 0) {
            std::cout << "✅ Đã học hết!\n";
            return;
        }
        
        for (const auto& f : files) {
            if (!f.imported) {
                std::cout << "\n━━━━ 📖 " << f.filename << " ━━━━\n";
                
                ImportStats stats;
                if (f.fullPath.find(".json") != std::string::npos) {
                    // JSON → Append trực tiếp vào math_history.json
                    std::cout << "📦 JSON file - Append trực tiếp (không re-solve)...\n";
                    
                    std::ifstream src(f.fullPath);
                    std::ofstream dst("data/math_history.json", std::ios::app);
                    
                    if (src.is_open() && dst.is_open()) {
                        dst << src.rdbuf();
                        src.close();
                        dst.close();
                        std::cout << "✅ Đã merge vào math_history.json\n";
                        std::cout << "💡 Gõ 'cleanup' để xóa bài trùng (nếu có)\n";
                    } else {
                        std::cout << "❌ Lỗi khi mở file\n";
                    }
                    
                    stats = {0, 0, 0, 0, 0};
                } else {
                    stats = (f.fullPath.find(".csv") != std::string::npos) 
                        ? importAndSolveCSV(f.fullPath)
                        : importAndSolveText(f.fullPath);
                }
                
                markAsImported(f.filename);
            }
        }
        std::cout << "💡 Gõ 'train' để AI học\n";
        
    } else if (choice == "2") {
        std::cout << "Nhập số (VD: 1 3 5): ";
        std::string sel;
        std::getline(std::cin, sel);
        
        std::istringstream iss(sel);
        int idx;
        while (iss >> idx) {
            if (idx > 0 && idx <= (int)files.size()) {
                const auto& f = files[idx-1];
                std::cout << "\n━━━━ 📖 " << f.filename << " ━━━━\n";
                ImportStats stats = (f.fullPath.find(".csv") != std::string::npos)
                    ? importAndSolveCSV(f.fullPath)
                    : importAndSolveText(f.fullPath);
                markAsImported(f.filename);
            }
        }
        std::cout << "💡 Gõ 'train'\n";
        
    } else if (choice == "3") {
        std::cout << "Chắc chắn? (y/n): ";
        std::string confirm;
        std::getline(std::cin, confirm);
        if (confirm == "y") {
            resetImportLog();
            for (const auto& f : files) {
                std::cout << "\n━━━━ 📖 " << f.filename << " ━━━━\n";
                ImportStats stats = (f.fullPath.find(".csv") != std::string::npos)
                    ? importAndSolveCSV(f.fullPath)
                    : importAndSolveText(f.fullPath);
                markAsImported(f.filename);
            }
            std::cout << "💡 Gõ 'train'\n";
        }
        
    } else if (choice == "4") {
        resetImportLog();
    }
}

} // namespace ai