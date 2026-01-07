#include "GroundTruthLogger.h"

#include "../sim/Simulation.h"
#include "../sim/World.h"
#include "../sim/Entity.h"
#include "../sim/Drone.h"
#include "../sim/Target.h"
#include "../sim/Pose.h"

GroundTruthLogger::~GroundTruthLogger() {
    Close();
}

bool GroundTruthLogger::OpenStateLog(const std::string& path) {
    state_.open(path, std::ios::out | std::ios::trunc);
    return state_.is_open();
}

void GroundTruthLogger::Close() {
    if (state_.is_open()) state_.close();
}

static const char* EntityTypeName(const Entity& e) {
    if (dynamic_cast<const Drone*>(&e)) return "drone";
    if (dynamic_cast<const Target*>(&e)) return "target";
    return "entity";
}

void GroundTruthLogger::WriteStateHeader() {
    if (!state_.is_open()) return;
    state_ << "time_s,step,entity_id,type,x,y,heading\n";
}

void GroundTruthLogger::LogState(const Simulation& sim) {
    if (!state_.is_open()) return;

    const double t = sim.world().time_s();
    const int step = sim.step_count();

    for (const auto& sp : sim.world().entities()) {
        const Entity& e = *sp;
        const Pose& p = e.pose();

        state_
            << t << ","
            << step << ","
            << e.id() << ","
            << EntityTypeName(e) << ","
            << p.x << ","
            << p.y << ","
            << p.heading
            << "\n";
    }
}
