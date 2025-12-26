#include "Simulation.h"
#include <iostream>

void Simulation::RunForSteps(int steps) {
    for (int i = 0; i < steps; ++i) {
        Step();
    }
}

void Simulation::Step() {
    ++step_count_;
    std::cout << "Simulation step: " << step_count_ << "\n";
}
