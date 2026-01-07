#include "App.h"

#include "../sim/Simulation.h"
#include "Scenario.h"

#include "../io/GroundTruthLogger.h"
#include "../io/PerceptionLogger.h"

#include "../sensors/RadarSensor.h"
#include "../sensors/RadarConfig.h"

#include "../render/AsciiRenderer.h"
#include "../sim/World.h"

#include <memory>
#include <vector>
#include <thread>
#include <chrono>

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

    // Feature toggles
    const bool enable_rendering = true; // false to turn off rendering
    const bool enable_truth_logging = true; // false to turn off truth logging
    const bool enable_perception_logging = true; // false to turn off perception logging

    // Setup sim + scenario
    Simulation sim;
    sim.Reset(dt, seed);
    BuildScenario(sim, cfg);

    // ASCII debug renderer (draw every N steps)
    std::unique_ptr<AsciiRenderer> renderer;
    if (enable_rendering) {
        renderer = std::make_unique<AsciiRenderer>(100, 30, ClearMode::Auto); // adjust size if you want
    }
    RenderBounds rb;
    rb.min_x = cfg.world_min_x;
    rb.max_x = cfg.world_max_x;
    rb.min_y = cfg.world_min_y;
    rb.max_y = cfg.world_max_y;

    const int render_every_n_steps = 5;

    // Logging
    std::unique_ptr<GroundTruthLogger> truth_logger;
    if (enable_truth_logging) {
        truth_logger = std::make_unique<GroundTruthLogger>();
        truth_logger->OpenStateLog("state.csv");
        truth_logger->WriteStateHeader();
    }

    std::unique_ptr<PerceptionLogger> perception_logger;
    if (enable_perception_logging) {
        perception_logger = std::make_unique<PerceptionLogger>();
        perception_logger->OpenDetectionsLog("detections.csv");
        perception_logger->WriteDetectionsHeader();
    }

    // Sensors (single sensor now, multi-sensor ready)
    std::vector<std::unique_ptr<RadarSensor>> sensors;
    RadarConfig radar_cfg;
    radar_cfg.name = "radar_front";
    sensors.push_back(std::make_unique<RadarSensor>(radar_cfg));

    // Log initial state (step 0)
    if (truth_logger) truth_logger->LogState(sim);
    {
        const double t = sim.world().time_s();
        const int step = sim.step_count();
        std::vector<Detection> all_dets;
        for (auto& s : sensors) {
            auto dets = s->Update(sim);
            if (perception_logger) perception_logger->LogDetections(t, step, dets);
            all_dets.insert(all_dets.end(), dets.begin(), dets.end());
        }

        if (step % render_every_n_steps == 0) {
            if (renderer) {
                const bool cleared = renderer->Render(sim.world(), all_dets, rb, step, t);
                if (cleared) {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(renderer->FrameDelayMs()));
                }
            }
        }
    }

    // Run step-by-step so we can log each tick
    for (int i = 0; i < steps; ++i) {
        sim.RunForSteps(1);

        if (truth_logger) truth_logger->LogState(sim);

        const double t = sim.world().time_s();
        const int step = sim.step_count();
        std::vector<Detection> all_dets;
        for (auto& s : sensors) {
            auto dets = s->Update(sim);
            if (perception_logger) perception_logger->LogDetections(t, step, dets);
            all_dets.insert(all_dets.end(), dets.begin(), dets.end());
        }

        if (step % render_every_n_steps == 0) {
            if (renderer) {
                const bool cleared = renderer->Render(sim.world(), all_dets, rb, step, t);
                if (cleared) {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(renderer->FrameDelayMs()));
                }
            }
        }
    }
}
