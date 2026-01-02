#include "App.h"
#include "../sim/Simulation.h"
#include "../sim/Entity.h"
#include "Scenario.h"
#include <cstdint>

void App::Run() {
    ScenarioConfig cfg;
    cfg.world_min_x = 0.0;
    cfg.world_max_x = 200.0;
    cfg.world_min_y = -50.0;
    cfg.world_max_y = 50.0;
    cfg.target_count = 25;

    cfg.drone_x = 0.0;
    cfg.drone_y = 0.0;
    cfg.drone_heading = 0.0;
    cfg.drone_speed_mps = 5.0;

    Simulation sim1;
    sim1.Reset(0.1, 123);
    BuildScenario(sim1, cfg);

    Simulation sim2;
    sim2.Reset(0.1, 123);
    BuildScenario(sim2, cfg);

    Simulation sim3;
    sim3.Reset(0.1, 124);
    BuildScenario(sim3, cfg);

    // now do the real run
    sim1.RunForSteps(300);
}

