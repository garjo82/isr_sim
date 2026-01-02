#include "World.h"
#include "Entity.h"

#include <utility>

void World::Reset() {
    time_s_ = 0.0;
    entities_.clear();
}

void World::Update(double dt) {
    time_s_ += dt;
    for (auto& e : entities_) {
        e->Update(dt);
    }
}

double World::time_s() const {
    return time_s_;
}

void World::Add(std::shared_ptr<Entity> e) {
    entities_.push_back(std::move(e));
}

const std::vector<std::shared_ptr<Entity>>& World::entities() const {
    return entities_;
}
