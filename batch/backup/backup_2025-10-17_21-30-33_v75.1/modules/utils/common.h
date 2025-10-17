#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector> 

namespace utils {
    void printHeader(const std::string &title);
    void clearScreen();
    void setConsoleUTF8();
    
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    void listAvailableModules();
    std::string getProjectPath();
}

#endif