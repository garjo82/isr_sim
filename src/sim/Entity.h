#pragma once

#include "Pose.h"

class Entity {
public:
    Entity(int id, Pose pose) : id_(id), pose_(pose) {}
    virtual ~Entity() = default;

    int id() const { return id_; }
    const Pose& pose() const { return pose_; }

    virtual void Update(double dt) = 0;

protected:
    int id_ = 0;
    Pose pose_;
};
