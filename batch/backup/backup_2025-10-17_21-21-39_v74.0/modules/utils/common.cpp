#include "common.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <windows.h>
#include "../system/command_registry.h"

namespace utils {
    
void printHeader(const std::string &title) {
    //auto cmds = sys::CommandRegistry::getAll();
    std::cout << "\n╔════════════════════════════════════════════╗\n";
    std::cout << "║  " << title;
    int padding = 42 - (int)title.length();
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";
    //std::cout << "📝 Lệnh:\n";
    std::cout << "💡 Gõ 'help' để xem danh sách lệnh\n";
    std::cout << "\n";
    
    // for (const auto& cmd : cmds) {
    //     std::cout << "   " << cmd.name;
    //     if (cmd.name.size() < 8)
    //         std::cout << std::string(8 - cmd.name.size(), ' ');
    //     std::cout << " - " << cmd.description << "\n";
    // }
    // std::cout << "\n";
    
}

void clearScreen() {
    system("cls");
}

void setConsoleUTF8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void listAvailableModules() {
    std::cout << "\n=== DANH SÁCH MODULES ===\n";
    std::cout << "🔢 Math: calculator, statistics\n";
    std::cout << "⚙️  System: commands, file_manager\n"; 
    std::cout << "🌐 Language: translator, learner\n";
    std::cout << "🤖 AI: chatbot, predictor\n";
    std::cout << "📊 Data: analyzer, vietlott_processor\n";
    std::cout << "==========================\n\n";
}

std::string getProjectPath() {
    return ".";
}

} // namespace utils