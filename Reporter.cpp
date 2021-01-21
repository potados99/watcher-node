#include "Reporter.h"

#include <WebSocketsClient.h>
#include <WiFi.h>
#include <WiFiManager.h>

#include "config.h"

class Reporter::Impl {
private:
    WiFiManager wifiManager;
    WebSocketsClient webSocket;

    // WebSocket configutations
    const char *host;
    int port;
    const char *path;

    // Status
    bool isConnected = false;
    unsigned long lastHeartBeat = 0;

    void handleWebSocketEvent(WStype_t type, uint8_t *payload, size_t lenght) {
        switch (type) {
            case WStype_CONNECTED:
                isConnected = true;
                webSocket.sendTXT("5");  // socket.io upgrade confirmation message (required)
                break;

            case WStype_DISCONNECTED:
                isConnected = false;
                break;

            default: break;
        }
    }

public:
    Impl(const char *host, int port, const char *path) {
        this->host = host;
        this->port = port;
        this->path = path;
    }

    void setupWiFi() {
        WiFi.setHostname(NODE_NAME);
        wifiManager.autoConnect(NODE_NAME "Setup", SETUP_PASSWORD);
    }

    void connectSocket() {
        webSocket.beginSocketIO(host, port, path);
        webSocket.onEvent([&](WStype_t type, uint8_t *payload, size_t lenght) {
            handleWebSocketEvent(type, payload, lenght);
        });
    }

    void emit(String event, String payload) {
        if (!isConnected) {
            return;
        }
        
        webSocket.sendTXT("42[\"" + event + "\", " + payload + "]");
    }

    void loop() {
        webSocket.loop();

        if (!isConnected) {
            return;
        }

        auto now = millis();

        if ((now - lastHeartBeat) > HEARTBEAT_INTERVAL) {
            webSocket.sendTXT("2");  // socket.io heartbeat message
            lastHeartBeat = now;
        }
    }
};

Reporter::Reporter(const char *host, int port, const char *path) : impl(new Impl(host, port, path)) {}
Reporter::~Reporter() { delete impl; }

void Reporter::setup() {
    impl->setupWiFi();
    impl->connectSocket();
}

void Reporter::emit(const char *event, bool value) {
    impl->emit(event, String(value));
}

void Reporter::emit(const char *event, int value) {
    impl->emit(event, String(value));
}

void Reporter::emit(const char *event, const char *value) { 
    impl->emit(event, value); 
}

void Reporter::loop() { 
    impl->loop(); 
}
