#include "CsvLogger.h"

#include "../sim/Simulation.h"
#include "../sim/World.h"
#include "../sim/Entity.h"
#include "../sim/Drone.h"
#include "../sim/Target.h"
#include "../sim/Pose.h"

#include <cmath>
#include <typeinfo>

CsvLogger::~CsvLogger() {
    Close();
}

bool CsvLogger::OpenStateLog(const std::string& path) {
    state_.open(path, std::ios::out | std::ios::trunc);
    return state_.is_open();
}

bool CsvLogger::OpenEventLog(const std::string& path) {
    events_.open(path, std::ios::out | std::ios::trunc);
    return events_.is_open();
}

void CsvLogger::Close() {
    if (state_.is_open()) state_.close();
    if (events_.is_open()) events_.close();
}

void CsvLogger::WriteStateHeader() {
    if (!state_.is_open()) return;
    state_ << "time_s,step,entity_id,type,x,y,heading\n";
}

void CsvLogger::WriteEventHeader() {
    if (!events_.is_open()) return;
    events_ << "time_s,step,drone_id,target_id,distance_m\n";
}

static const char* EntityTypeName(const Entity& e) {
    if (dynamic_cast<const Drone*>(&e)) return "drone";
    if (dynamic_cast<const Target*>(&e)) return "target";
    return "entity";
}

void CsvLogger::LogState(const Simulation& sim) {
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

void CsvLogger::LogDetectionsProximity(const Simulation& sim, double range_m) {
    if (!events_.is_open()) return;

    // Find the first drone (prototype assumption: 1 drone)
    const Drone* drone = nullptr;
    int drone_id = -1;
    Pose drone_pose;

    for (const auto& sp : sim.world().entities()) {
        if (auto d = dynamic_cast<const Drone*>(sp.get())) {
            drone = d;
            drone_id = d->id();
            drone_pose = d->pose();
            break;
        }
    }
    if (!drone) return;

    const double t = sim.world().time_s();
    const int step = sim.step_count();

    // For each target, log if within range
    for (const auto& sp : sim.world().entities()) {
        if (auto tgt = dynamic_cast<const Target*>(sp.get())) {
            const Pose& tp = tgt->pose();
            const double dx = tp.x - drone_pose.x;
            const double dy = tp.y - drone_pose.y;
            const double dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= range_m) {
                events_
                    << t << ","
                    << step << ","
                    << drone_id << ","
                    << tgt->id() << ","
                    << dist
                    << "\n";
            }
        }
    }
}
