#include "PerceptionLogger.h"

PerceptionLogger::~PerceptionLogger() {
    Close();
}

bool PerceptionLogger::OpenDetectionsLog(const std::string& path) {
    dets_.open(path, std::ios::out | std::ios::trunc);
    return dets_.is_open();
}

void PerceptionLogger::Close() {
    if (dets_.is_open()) dets_.close();
}

void PerceptionLogger::WriteDetectionsHeader() {
    if (!dets_.is_open()) return;
    dets_ << "time_s,step,sensor_name,detection_id,x,y,range_m,bearing_rad,confidence,is_false_positive\n";
}

void PerceptionLogger::LogDetections(double time_s, int step, const std::vector<Detection>& dets) {
    if (!dets_.is_open()) return;

    for (const auto& d : dets) {
        dets_
            << time_s << ","
            << step << ","
            << d.sensor_name << ","
            << d.detection_id << ","
            << d.x_world << ","
            << d.y_world << ","
            << d.range_m << ","
            << d.bearing_rad << ","
            << d.confidence << ","
            << (d.is_false_positive ? 1 : 0)
            << "\n";
    }
}
