#pragma once
#include "../io/WebStreamer.h"

class App {
public:
    void Run();
private:
    WebStreamer streamer_; // This MUST be here for App.cpp to see it
};