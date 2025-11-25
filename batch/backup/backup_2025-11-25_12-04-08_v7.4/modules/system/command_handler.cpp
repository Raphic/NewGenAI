#include "command_handler.h"
#include "command_registry.h"
#include "../utils/common.h"
#include "../math/calculator.h"
#include <iostream>
#include <cstdlib>

namespace sys {
    bool CommandHandler::handleCommand(const std::string &input) {
        if (input == "exit") {
            handleExit();
            return true;
        } else if (input == "clear") {
            handleClear();
            return true;
        } else if (input == "help") {
            handleHelp();
            return true;
        } else if (input == "modules") {
            handleModules();
            return true;
        }
        return false;
    }

    void CommandHandler::handleClear() {
        std::cout << "🧹 Đang clear màn hình...\n";
        system("batch\\clear.bat");
    }

    void CommandHandler::handleExit() {
        std::cout << "👋 Tạm biệt!\n";
        exit(0);
    }

    void CommandHandler::handleHelp() {
        math::Calculator::showAdvancedHelp();
    }

    void CommandHandler::handleModules() {
        utils::listAvailableModules();
    }
    void CommandHandler::showHelp() {
        handleHelp();
    }

} // namespace sys