#pragma once

#include "Entity.h"

class Drone : public Entity {
public:
    Drone(int id, Pose pose, double speed_mps)
        : Entity(id, pose), speed_mps_(speed_mps) {}

    void Update(double dt) override {
        pose_.x += speed_mps_ * dt;
    }

private:
    double speed_mps_ = 0.0;
};
