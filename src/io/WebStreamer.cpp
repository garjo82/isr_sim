#include "WebStreamer.h"
#include <iostream>

// Include necessary IX headers for type resolution
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXConnectionState.h>

WebStreamer::WebStreamer(int port) : port_(port), server_(port, "0.0.0.0") {

    // Explicitly matching the signature: weak_ptr then shared_ptr<ConnectionState>
    // This resolves the C2664 conversion error by using the exact library types.
    server_.setOnConnectionCallback([this](std::weak_ptr<ix::WebSocket> webSocketWeak,
        std::shared_ptr<ix::ConnectionState> state) {

            // Upgrade the weak_ptr to a shared_ptr to manage its lifecycle safely
            auto webSocket = webSocketWeak.lock();
            if (!webSocket) return;

            {
                // Thread-safe insertion into the active client set
                std::lock_guard<std::mutex> lock(clients_mtx_);
                clients_.insert(webSocket);
            }

            // Set up the message callback for this specific client to handle disconnects
            webSocket->setOnMessageCallback([this, webSocket](const ix::WebSocketMessagePtr& msg) {
                if (msg->type == ix::WebSocketMessageType::Close || msg->type == ix::WebSocketMessageType::Error) {
                    std::lock_guard<std::mutex> lock(clients_mtx_);
                    clients_.erase(webSocket);
                }
                });
        });
}

void WebStreamer::Start() {
    // listenAndStart() is the atomic "Open Port + Start Thread" call.
    // It returns a pair (success_bool, error_message) in some versions, 
    // or just a bool in others. We check success to ensure the port is active.
    if (!server_.listenAndStart()) {
        std::cerr << "CRITICAL: WebStreamer failed to bind to port " << port_
            << ". Is another app using it?" << std::endl;
        return;
    }
    std::cout << "SUCCESS: WebStreamer listening on ws://localhost:" << port_ << std::endl;
}

void WebStreamer::Stop() {
    server_.stop();
}

void WebStreamer::Broadcast(const std::string& json_payload) {
    // Protect the client set while iterating and broadcasting
    std::lock_guard<std::mutex> lock(clients_mtx_);
    for (auto it = clients_.begin(); it != clients_.end(); ) {
        // Only send to sockets that have completed the handshake and are open
        if ((*it)->getReadyState() == ix::ReadyState::Open) {
            (*it)->send(json_payload);
            ++it;
        }
        else {
            // Clean up stale or closed connections
            it = clients_.erase(it);
        }
    }
}

WebStreamer::~WebStreamer() {
    Stop(); // RAII: Ensure the server background thread stops when the object is destroyed
}