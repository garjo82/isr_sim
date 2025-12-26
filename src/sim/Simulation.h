#pragma once

class Simulation {
public:
    void RunForSteps(int steps);

private:
    void Step();
    int step_count_ = 0;
};
