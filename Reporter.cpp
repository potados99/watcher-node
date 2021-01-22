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

    // Invoked when server wants a realtime data.
    void (*updateRequestCallback)(void);

    void handleDisconnect() {
        Serial.printf("Socket.io disconnected!\n");
    }

    void handleConnect() {
        Serial.printf("Socket.io connected!\n");
        socketIO.send(sIOtype_CONNECT, SOCKET_NAMESPACE);
    }

    void launchUpdateRequestCallback() {
        Serial.printf("Server wants an update!\n");

        if (updateRequestCallback) {
            updateRequestCallback();
        }
    }

    void handleEvent(uint8_t *payload) {
        Serial.printf("Socket.io got event\n");

        bool serverWantsUpdate = String((const char *)payload).indexOf("\"request:update\"") > 0;
        
        if (serverWantsUpdate) {
            launchUpdateRequestCallback();
        } 
    }

    void handleWebSocketEvent(socketIOmessageType_t type, uint8_t *payload, size_t length) {
        switch (type) {
            case sIOtype_DISCONNECT: return handleDisconnect();
            case sIOtype_CONNECT: return handleConnect();
            case sIOtype_EVENT: return handleEvent(payload);
            
            default: Serial.printf("Message type: %d\n", sIOtype_ACK); break;
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

    void emitEvent(String event, String payload) {
        socketIO.sendEVENT(SOCKET_NAMESPACE ",[\"" + event + "\", " + payload + "]");
    }

    void onUpdateRequest(void (*action)(void)) {
        updateRequestCallback = action;
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
    impl->emitEvent(event, String(value));
}

void Reporter::emit(const char *event, int value) {
    impl->emitEvent(event, String(value));
}

void Reporter::emit(const char *event, float value) {
    impl->emitEvent(event, String(value));
}

void Reporter::emit(const char *event, const char *fmt, ...) { 
    char buf[128];
    
    va_list args;
    va_start (args, fmt);
    vsnprintf(buf, sizeof(buf)/sizeof(char), fmt, args);
    va_end (args);

    impl->emitEvent(event, buf); 
}

void Reporter::updateProp(const char *propName, bool propValue) {
    emit("prop:update", "{\"name\": \"%s\", \"value\": %s}", propName, propValue ? "true" : "false");
}

void Reporter::updateProp(const char *propName, int propValue) {
    emit("prop:update", "{\"name\": \"%s\", \"value\": %d}", propName, propValue);
}

void Reporter::updateProp(const char *propName, float propValue) {
    emit("prop:update", "{\"name\": \"%s\", \"value\": %f}", propName, propValue);
}

void Reporter::updateProp(const char *propName, const char *propValue) {
    emit("prop:update", "{\"name\": \"%s\", \"value\": \"%s\"}", propName, propValue);
}

void Reporter::onUpdateRequest(void (*action)(void)) {
    impl->onUpdateRequest(action);
}

void Reporter::loop() { 
    impl->loop(); 
}