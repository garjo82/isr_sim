#pragma once

#include <fstream>
#include <string>

class Simulation;

class GroundTruthLogger {
public:
    GroundTruthLogger() = default;
    ~GroundTruthLogger();

    bool OpenStateLog(const std::string& path);
    void Close();

    void WriteStateHeader();
    void LogState(const Simulation& sim);

private:
    std::ofstream state_;
};
