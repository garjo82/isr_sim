#include "Scenario.h"

#include "../sim/Simulation.h"
#include "../sim/Drone.h"
#include "../sim/Target.h"
#include "../sim/Pose.h"

#include <memory>
#include <random>

void BuildScenario(Simulation& sim, const ScenarioConfig& cfg) {
    // Spawn drone
    sim.world().Add(std::make_shared<Drone>(
        1,
        Pose{ cfg.drone_x, cfg.drone_y, cfg.drone_heading },
        cfg.drone_speed_mps
        ));

    // Random target placement (deterministic via sim.rng())
    std::uniform_real_distribution<double> dist_x(cfg.world_min_x, cfg.world_max_x);
    std::uniform_real_distribution<double> dist_y(cfg.world_min_y, cfg.world_max_y);

    // Start IDs after drone
    int next_id = 2;

    for (int i = 0; i < cfg.target_count; ++i) {
        const double x = dist_x(sim.rng());
        const double y = dist_y(sim.rng());

        sim.world().Add(std::make_shared<Target>(
            next_id++,
            Pose{ x, y, 0.0 }
        ));
    }
}
