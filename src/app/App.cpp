#include "App.h"
#include "../sim/Simulation.h"

#include <memory>

#include "../sim/Drone.h"
#include "../sim/Target.h"
#include "../sim/Pose.h"

void App::Run() {
    Simulation sim;
    sim.Reset(0.1, 123); // dt, seed

    sim.world().Add(std::make_shared<Drone>(1, Pose{ 0.0, 0.0, 0.0 }, 5.0));
    sim.world().Add(std::make_shared<Target>(2, Pose{ 25.0, 10.0, 0.0 }));
    sim.world().Add(std::make_shared<Target>(3, Pose{ 40.0, -5.0, 0.0 }));

    sim.RunForSteps(3);
}
