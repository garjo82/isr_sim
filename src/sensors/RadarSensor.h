#pragma once

#include "RadarConfig.h"
#include "Detection.h"

#include <cstdint>
#include <random>
#include <string>
#include <vector>

class Simulation;

class RadarSensor {
public:
    explicit RadarSensor(RadarConfig cfg);

    const std::string& name() const { return cfg_.name; }

    // Returns detections for this tick (no truth IDs)
    std::vector<Detection> Update(const Simulation& sim);

private:
    RadarConfig cfg_;

    std::mt19937 rng_;
    std::normal_distribution<double> n_range_;
    std::normal_distribution<double> n_bearing_;
    std::normal_distribution<double> n_snr_;
    std::uniform_real_distribution<double> u01_;

    std::uint64_t next_detection_id_ = 1;

    double ComputeConfidence(double range_m, double bearing_rad);
    bool ShouldMiss(double range_m);

    Detection MakeDetectionFromMeasurement(
        double drone_x, double drone_y, double drone_heading,
        double range_m, double bearing_rad,
        double confidence, bool is_false_positive);

    Detection MakeFalsePositive(
        double drone_x, double drone_y, double drone_heading);
};
