#include "command_registry.h"
#include <iostream> // Có thể cần để debug nếu muốn

namespace sys {

std::vector<Command> CommandRegistry::commands_;
std::mutex CommandRegistry::mutex_;

void CommandRegistry::addName(const std::string& name, const std::string& desc) {
    std::lock_guard<std::mutex> lock(mutex_);
    // Kiểm tra xem lệnh đã tồn tại chưa để tránh trùng lặp
    for (const auto& cmd : commands_) {
        if (cmd.name == name) {
            // std::cout << "DEBUG: Command '" << name << "' already registered. Skipping.\n"; // Để debug
            return; // Đã tồn tại, không thêm nữa
        }
    }
    commands_.push_back({name, desc});
    // std::cout << "DEBUG: Registered command: " << name << " - " << desc << "\n"; // Để debug
}

std::vector<Command> CommandRegistry::getAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    return commands_;
}

} // namespace sys