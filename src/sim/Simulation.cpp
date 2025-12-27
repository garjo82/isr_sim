#include "Simulation.h"

#include <iostream>
#include <memory>

#include "Drone.h"
#include "Target.h"
#include "Entity.h"
#include "Pose.h"

void Simulation::RunForSteps(int steps) {
    world_.Reset();
    world_.Add(std::make_shared<Drone>(1, Pose{ 0.0, 0.0, 0.0 }, 5.0));
    world_.Add(std::make_shared<Target>(2, Pose{ 25.0, 10.0, 0.0 }));
    world_.Add(std::make_shared<Target>(3, Pose{ 40.0, -5.0, 0.0 }));

    for (int i = 0; i < steps; ++i) {
        Step();
    }
}

void Simulation::Step() {
    ++step_count_;
    world_.Update(dt_);

    std::cout << "step=" << step_count_
        << " | t=" << world_.time_s() << "s";

    for (const auto& e : world_.entities()) {
        const auto& p = e->pose();
        std::cout << " | id=" << e->id()
            << " (" << p.x << ", " << p.y << ")";
    }

    std::cout << "\n";
}
