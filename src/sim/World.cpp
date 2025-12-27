#include "World.h"
#include "Entity.h"

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

void World::Add(const std::shared_ptr<Entity>& e) {
    entities_.push_back(e);
}

const std::vector<std::shared_ptr<Entity>>& World::entities() const {
    return entities_;
}
