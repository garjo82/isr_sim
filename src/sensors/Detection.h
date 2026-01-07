#pragma once

#include <cstdint>
#include <string>

struct Detection {
    std::string sensor_name;
    std::uint64_t detection_id = 0;

    // Estimated measurement in world coordinates
    double x_world = 0.0;
    double y_world = 0.0;

    // Useful debug fields (measured in sensor frame)
    double range_m = 0.0;
    double bearing_rad = 0.0;

    // Confidence [0, 1]
    double confidence = 0.0;

    // True if generated as clutter / spurious return
    bool is_false_positive = false;
};
