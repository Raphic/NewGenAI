#pragma once
#include <string>

namespace ai {

struct MathProblem {
    std::string input;
    std::string type;
    std::string result;
    bool success = false;
    long long timestamp = 0;
};

} // namespace ai
