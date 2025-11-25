#pragma once
#include <string>
#include <vector>
#include <mutex>

namespace sys {

struct Command {
    std::string name;
    std::string description;
};

class CommandRegistry {
public:
    static void addName(const std::string& name, const std::string& desc);
    static std::vector<Command> getAll();
private:
    static std::vector<Command> commands_;
    static std::mutex mutex_;
};

// ✅ Helper để auto-register lệnh
struct CommandRegistrar {
    CommandRegistrar(const std::string& name, const std::string& desc) {
        CommandRegistry::addName(name, desc);
    }
};

} // namespace sys

// ✅ MACRO để định nghĩa lệnh - CHỈ SỬA 1 CHỖ
#define DEFINE_COMMAND(cmd_name, cmd_desc) \
    static sys::CommandRegistrar _auto_reg_##cmd_name{#cmd_name, cmd_desc}; \
    if (false) goto cmd_label_##cmd_name; \
    cmd_label_##cmd_name: \
    if (input == #cmd_name)
    