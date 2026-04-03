#include <ixwebsocket/IXNetSystem.h>
#include "app/App.h"

int main() {
    // CRITICAL for Windows: Initialize the network subsystem
    ix::initNetSystem();

    App app;
    app.Run();

    // Cleanup before exit
    ix::uninitNetSystem();
    return 0;
}