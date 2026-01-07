#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../sensors/Detection.h"

class PerceptionLogger {
public:
    PerceptionLogger() = default;
    ~PerceptionLogger();

    bool OpenDetectionsLog(const std::string& path);
    void Close();

    void WriteDetectionsHeader();
    void LogDetections(double time_s, int step, const std::vector<Detection>& dets);

private:
    std::ofstream dets_;
};
