#include "App.h"

#include "../sim/Simulation.h"
#include "Scenario.h"

#include "../io/GroundTruthLogger.h"
#include "../io/PerceptionLogger.h"

#include "../sensors/RadarSensor.h"
#include "../sensors/RadarConfig.h"

#include <memory>
#include <vector>

void App::Run() {
    // Config
    const double dt = 0.1;
    const std::uint32_t seed = 123;
    const int steps = 300;

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
    GroundTruthLogger truth_logger;
    truth_logger.OpenStateLog("state.csv");
    truth_logger.WriteStateHeader();

    PerceptionLogger perception_logger;
    perception_logger.OpenDetectionsLog("detections.csv");
    perception_logger.WriteDetectionsHeader();

    // Sensors (single sensor now, multi-sensor ready)
    std::vector<std::unique_ptr<RadarSensor>> sensors;
    RadarConfig radar_cfg;
    radar_cfg.name = "radar_front";
    sensors.push_back(std::make_unique<RadarSensor>(radar_cfg));

    // Log initial state (step 0)
    truth_logger.LogState(sim);
    {
        const double t = sim.world().time_s();
        const int step = sim.step_count();
        for (auto& s : sensors) {
            auto dets = s->Update(sim);
            perception_logger.LogDetections(t, step, dets);
        }
    }

    // Run step-by-step so we can log each tick
    for (int i = 0; i < steps; ++i) {
        sim.RunForSteps(1);

        truth_logger.LogState(sim);

        const double t = sim.world().time_s();
        const int step = sim.step_count();

        for (auto& s : sensors) {
            auto dets = s->Update(sim);
            perception_logger.LogDetections(t, step, dets);
        }
    }
}
