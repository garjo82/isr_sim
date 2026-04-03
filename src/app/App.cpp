#include "App.h"

// Standard Library Includes
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdint> // Required for std::uint32_t
#include <string>

// Internal Project Includes
#include "../sim/Simulation.h"
#include "Scenario.h"
#include "../io/GroundTruthLogger.h"
#include "../io/PerceptionLogger.h"
#include "../io/JsonSerializer.h"
#include "../io/WebStreamer.h"
#include "../sensors/RadarSensor.h"
#include "../render/AsciiRenderer.h"

void App::Run() {
    // --- 1. EXECUTION MODE TOGGLES ---
    bool enable_stream = true;
    bool enable_ascii = true;
    const bool headless_mode = false;
    const double target_hz = 10.0;

    if (headless_mode) {
        enable_stream = false;
        enable_ascii = false;
    }

    if (enable_stream) {
        streamer_.Start();
    }

    // --- 2. SIMULATION SETTINGS ---
    const double dt = 0.1;
    const std::uint32_t seed = 123;
    const int steps = 300;

    ScenarioConfig cfg;
    cfg.world_min_x = 0.0; cfg.world_max_x = 200.0;
    cfg.world_min_y = -50.0; cfg.world_max_y = 50.0;
    cfg.target_count = 25;
    cfg.drone_x = 0.0; cfg.drone_y = 0.0;
    cfg.drone_heading = 0.0; cfg.drone_speed_mps = 5.0;

    Simulation sim;
    sim.Reset(dt, seed);
    BuildScenario(sim, cfg);

    std::unique_ptr<AsciiRenderer> renderer;
    if (enable_ascii) {
        renderer = std::make_unique<AsciiRenderer>(100, 30, ClearMode::Auto);
    }

    RenderBounds rb;
    rb.min_x = cfg.world_min_x; rb.max_x = cfg.world_max_x;
    rb.min_y = cfg.world_min_y; rb.max_y = cfg.world_max_y;

    const int render_every_n_steps = 5;

    // --- 3. LOGGING & SENSOR SETUP ---
    std::unique_ptr<GroundTruthLogger> truth_logger = std::make_unique<GroundTruthLogger>();
    truth_logger->OpenStateLog("state.csv");
    truth_logger->WriteStateHeader();

    std::unique_ptr<PerceptionLogger> perception_logger = std::make_unique<PerceptionLogger>();
    perception_logger->OpenDetectionsLog("detections.csv");
    perception_logger->WriteDetectionsHeader();

    std::vector<std::unique_ptr<RadarSensor>> sensors;
    RadarConfig radar_cfg;
    radar_cfg.name = "radar_front";
    sensors.push_back(std::make_unique<RadarSensor>(radar_cfg));

    // --- 4. MAIN SIMULATION LOOP ---
    for (int i = 0; i < steps; ++i) {
        sim.RunForSteps(1);

        const double t = sim.world().time_s();
        const int step = sim.step_count();

        // Log state
        truth_logger->LogState(sim);

        std::vector<Detection> all_dets;
        for (auto& s : sensors) {
            auto dets = s->Update(sim);
            perception_logger->LogDetections(t, step, dets);
            all_dets.insert(all_dets.end(), dets.begin(), dets.end());
        }

        if (enable_stream) {
            std::string frame_json = JsonSerializer::SerializeFrame(sim.world(), all_dets, t, step);
            streamer_.Broadcast(frame_json);
        }

        if (enable_ascii && (step % render_every_n_steps == 0)) {
            if (renderer) {
                renderer->Render(sim.world(), all_dets, rb, step, t);
            }
        }

        if ((enable_stream || enable_ascii) && target_hz > 0.0) {
            int delay_ms = static_cast<int>(1000.0 / target_hz);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
    }

    std::cout << "\nSimulation complete. Mode: " << (headless_mode ? "HEADLESS" : "INTERACTIVE") << std::endl;
}