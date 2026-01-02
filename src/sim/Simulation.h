#pragma once
#include "World.h"

#include <cstdint>
#include <random>

class Simulation {
public:
    // Establishes deterministic run state and clears world/time counters.
    void Reset(double dt, std::uint32_t seed);

    // Runs the sim for N fixed steps using the current world + config.
    void RunForSteps(int steps);

    // Accessors (useful for App/Scenario/Logger later)
    int step_count() const { return step_count_; }
    double dt() const { return dt_; }
    std::uint32_t seed() const { return seed_; }
    std::mt19937& rng() { return rng_; }

    World& world() { return world_; }
    const World& world() const { return world_; }

private:
    void Step();

    int step_count_ = 0;
    double dt_ = 0.1;

    std::uint32_t seed_ = 0;
    std::mt19937 rng_{ 0 };

    World world_;
};
