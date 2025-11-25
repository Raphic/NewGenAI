#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>

namespace sys {

class CommandHandler {
public:
    static bool handleCommand(const std::string &input);
    static void showHelp();
    
private:
    static void handleRebuild();
    static void handleClear();
    static void handleExit();
    static void handleHelp();
    static void handleModules();
};

} // namespace sys

#endif