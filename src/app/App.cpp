#include "App.h"

#include "../sim/Simulation.h"
#include "Scenario.h"
#include "../io/CsvLogger.h"

void App::Run() {
    // Config
    const double dt = 0.1;
    const std::uint32_t seed = 123;
    const int steps = 300;
    const double detect_range_m = 10.0;

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

    // Setup sim + scenario
    Simulation sim;
    sim.Reset(dt, seed);
    BuildScenario(sim, cfg);

    // Logging
    CsvLogger logger;
    logger.OpenStateLog("state.csv");
    logger.OpenEventLog("events.csv");
    logger.WriteStateHeader();
    logger.WriteEventHeader();

    // Log initial state (step 0)
    logger.LogState(sim);
    logger.LogDetectionsProximity(sim, detect_range_m);

    // Run step-by-step so we can log each tick
    for (int i = 0; i < steps; ++i) {
        sim.RunForSteps(1);
        logger.LogState(sim);
        logger.LogDetectionsProximity(sim, detect_range_m);
    }
}
