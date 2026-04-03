#pragma once
#include <string>
#include <vector>
#include "../sensors/Detection.h"
#include "../sim/World.h"

class JsonSerializer {
public:
    // Transforms the entire world state and current detections into one JSON string
    static std::string SerializeFrame(const World& world,
        const std::vector<Detection>& detections,
        double time_s,
        int step);
};