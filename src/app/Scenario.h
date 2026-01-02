#pragma once

#include <cstdint>

class Simulation;

struct ScenarioConfig {
    double world_min_x = 0.0;
    double world_max_x = 100.0;
    double world_min_y = -50.0;
    double world_max_y = 50.0;

    int target_count = 25;

    // Drone
    double drone_x = 0.0;
    double drone_y = 0.0;
    double drone_heading = 0.0;   // radians
    double drone_speed_mps = 5.0;
};

// Populates sim.world() deterministically using sim.rng().
// Assumes sim.Reset(dt, seed) has already been called.
void BuildScenario(Simulation& sim, const ScenarioConfig& cfg);
