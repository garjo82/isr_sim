#pragma once
#include "World.h"

class Simulation {
public:
    void RunForSteps(int steps);

private:
    void Step();

    int step_count_ = 0;
    double dt_ = 0.1;
    World world_;
};
