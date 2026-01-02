#pragma once

#include <memory>
#include <vector>

class Entity;

class World {
public:
    void Reset();
    void Update(double dt);

    double time_s() const;

    void Add(std::shared_ptr<Entity> e);
    const std::vector<std::shared_ptr<Entity>>& entities() const;

private:
    double time_s_ = 0.0;
    std::vector<std::shared_ptr<Entity>> entities_;
};
