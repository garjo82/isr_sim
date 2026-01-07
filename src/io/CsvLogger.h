// DEPRECATED:
// Replaced by GroundTruthLogger and PerceptionLogger.
// Kept temporarily for reference during refactor.

#pragma once

#include <fstream>
#include <string>

class Simulation;

class CsvLogger {
public:
    CsvLogger() = default;
    ~CsvLogger();

    // Open/close
    bool OpenStateLog(const std::string& path);
    bool OpenEventLog(const std::string& path);
    void Close();

    // Logging
    void WriteStateHeader();
    void WriteEventHeader();

    void LogState(const Simulation& sim);
    void LogDetectionsProximity(const Simulation& sim, double range_m);

private:
    std::ofstream state_;
    std::ofstream events_;
};
