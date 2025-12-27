#pragma once

#include "Entity.h"

class Target : public Entity {
public:
    Target(int id, Pose pose) : Entity(id, pose) {}

    void Update(double /*dt*/) override {
        // static target
    }
};
