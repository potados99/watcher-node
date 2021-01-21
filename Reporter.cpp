#include "Reporter.h"

#include <WiFi.h>
#include <WiFiManager.h>

#include <WebSocketsClient.h> // This must be before SocketIOclient.h!
#include <SocketIOclient.h>

#include "IO.h"
#include "config.h"

class Reporter::Impl {
private:
    WiFiManager wifiManager;
    SocketIOclient socketIO;

    // WebSocket configutations
    const char *host;
    int port;
    const char *path;

    // Status
    unsigned long lastHeartBeat = 0;

    void handleWebSocketEvent(socketIOmessageType_t type, uint8_t *payload, size_t length) {
        switch (type) {
            case sIOtype_DISCONNECT:
                IO::printf("Socket.io disconnected!\n");
                break;
            case sIOtype_CONNECT:
                IO::printf("Socket.io connected!\n");
                socketIO.send(sIOtype_CONNECT, "/"); // join default namespace (no auto join in Socket.IO V3)
                break;
            case sIOtype_EVENT:
                IO::printf("Socket.io got event\n");
                break;
            case sIOtype_ACK:
                IO::printf("Socket.io got ack\n");
                break;
            case sIOtype_ERROR:
                IO::printf("Socket.io got error\n");
                break;
            case sIOtype_BINARY_EVENT:
                IO::printf("Socket.io got binary\n");
                break;
            case sIOtype_BINARY_ACK:
                IO::printf("Socket.io got binary ack\n");
                break;
        }
    }

public:
    Impl(const char *host, int port, const char *path) {
        this->host = host;
        this->port = port;
        this->path = path;
    }

    void setupWiFi() {
        wifiManager.setHostname(NODE_NAME);
        wifiManager.autoConnect(NODE_NAME "Setup", SETUP_PASSWORD);
    }

    void connectSocket() {
        IO::printf("Connecting websocket...\n");

        socketIO.begin(host, port, path);
        socketIO.onEvent([&](socketIOmessageType_t type, uint8_t *payload, size_t lenght) {
            handleWebSocketEvent(type, payload, lenght);
        });
    }

    void emit(String event, String payload) {
        socketIO.sendEVENT("[\"" + event + "\", " + payload + "]");
    }

    void loop() {
        socketIO.loop();
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