#pragma once
#include "math_logger.h"
#include "pattern_recognizer.h"
#include <iostream>

namespace ai {

class AITrainer {
public:
    AITrainer(MathLogger& logRef, PatternRecognizer& recogRef)
        : logger(logRef), recognizer(recogRef) {}

    void train();

private:
    MathLogger& logger;
    PatternRecognizer& recognizer;
};

} // namespace ai
