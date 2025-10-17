// storage_json.h
#pragma once
#include <vector>
#include <string>
#include "math_problem.h"

namespace ai {

struct MathProblem;

class IStorage {
public:
    virtual ~IStorage() = default;
    virtual bool saveHistory(const std::vector<MathProblem>& history) = 0;
    virtual std::vector<MathProblem> loadHistory() = 0;
    virtual bool appendProblem(const MathProblem& p) = 0;
    virtual bool backup() = 0;
};

class JsonStorage : public IStorage {
public:
    explicit JsonStorage(const std::string& filename); // chỉ khai báo

    bool saveHistory(const std::vector<MathProblem>& history) override;
    std::vector<MathProblem> loadHistory() override;
    bool appendProblem(const MathProblem& p) override;
    bool backup() override;

private:
    std::string filePath_; // đổi tên thành filePath_ cho khớp
};

} // namespace ai