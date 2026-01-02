#include "Simulation.h"

#include <algorithm>

void Simulation::Reset(double dt, std::uint32_t seed) {
    dt_ = dt;
    seed_ = seed;
    rng_.seed(seed_);

    step_count_ = 0;
    world_.Reset();
}

void Simulation::RunForSteps(int steps) {
    // Defensive: ignore negative inputs
    if (steps <= 0) return;

    for (int i = 0; i < steps; ++i) {
        Step();
    }
}

void Simulation::Step() {
    ++step_count_;
    world_.Update(dt_);

    // No logging/printing here. Step should be "pure simulation".
    // Logging will be added later via Logger + Sensor/Event hooks.
}
