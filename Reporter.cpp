#include "Reporter.h"

#include <stdio.h>
#include <stdarg.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WebSocketsClient.h> // This must be before SocketIOclient.h!
#include <SocketIOclient.h>

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
                Serial.printf("Socket.io disconnected!\n");
                break;
            case sIOtype_CONNECT:
                Serial.printf("Socket.io connected!\n");

                // Join /node namespace.
                socketIO.send(sIOtype_CONNECT, SOCKET_NAMESPACE);
                break;
            case sIOtype_EVENT:
                Serial.printf("Socket.io got event\n");
                break;
            case sIOtype_ACK:
                Serial.printf("Socket.io got ack\n");
                break;
            case sIOtype_ERROR:
                Serial.printf("Socket.io got error\n");
                break;
            case sIOtype_BINARY_EVENT:
                Serial.printf("Socket.io got binary\n");
                break;
            case sIOtype_BINARY_ACK:
                Serial.printf("Socket.io got binary ack\n");
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
        Serial.printf("Connecting websocket...\n");

        socketIO.begin(host, port, path);
        socketIO.onEvent([&](socketIOmessageType_t type, uint8_t *payload, size_t lenght) {
            handleWebSocketEvent(type, payload, lenght);
        });
    }

    void emit(String event, String payload) {
        socketIO.sendEVENT(SOCKET_NAMESPACE ",[\"" + event + "\", " + payload + "]");
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

void Reporter::emit(const char *event, float value) {
    impl->emit(event, String(value));
}

void Reporter::emit(const char *event, const char *fmt, ...) { 
    char buf[128];
    
    va_list args;
    va_start (args, fmt);
    vsnprintf(buf, sizeof(buf)/sizeof(char), fmt, args);
    va_end (args);

    impl->emit(event, buf); 
}

void Reporter::loop() { 
    impl->loop(); 
}