#pragma once

#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXConnectionState.h>
#include <string>
#include <mutex>
#include <set>
#include <memory>

class WebStreamer {
public:
    WebStreamer(int port = 8080);
    ~WebStreamer();

    void Start();
    void Stop();
    void Broadcast(const std::string& json_payload);

private:
    int port_;
    ix::WebSocketServer server_;

    std::mutex clients_mtx_;
    // We store shared_ptrs here so the streamer "owns" the connection 
    // while it is active for broadcasting.
    std::set<std::shared_ptr<ix::WebSocket>> clients_;
};