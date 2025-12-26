#include "App.h"
#include "../sim/Simulation.h"

void App::Run() {
    Simulation sim;
    sim.RunForSteps(3);
}
