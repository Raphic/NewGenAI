#pragma once
#include "math_logger.h"
#include <string>
#include <iostream>

namespace ai {

class DataCleaner {
public:
    explicit DataCleaner(MathLogger& loggerRef) : logger(loggerRef) {}

    void cleanup(bool autoBackup = true);

private:
    MathLogger& logger;
};

} // namespace ai
