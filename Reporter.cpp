#include "Reporter.h"

// Some symbols are hidden without this macro.
// We use arduino compiler targeting ESP32 board, therefore it has no problem.
#define ESP32

#include <WebSocketsClient.h>
#include <WiFi.h>
#include <WiFiManager.h>

#include "config.h"

Reporter::Reporter() : impl(new Impl()) {}
Reporter::~Reporter() { delete impl; }

void Reporter::setup() {
    impl->setupWiFi();
    impl->connectSocket();
}

void Reporter::emit(String event, bool value) {
    impl->emit(event, String(value));
}

void Reporter::emit(String event, int value) {
    impl->emit(event, String(value));
}

void Reporter::emit(String event, String value) { 
    impl->emit(event, value); 
}

void Reporter::loop() { 
    impl->loop(); 
}

class Reporter::Impl {
   private:
    WiFiManager wifiManager;
    WebSocketsClient webSocket;

    bool isConnected = false;
    unsigned long lastHeartBeat = 0;

    void handleWebSocketEvent(WStype_t type, uint8_t *payload, size_t lenght) {
        switch (type) {
            case WStype_CONNECTED:
                isConnected = true;
                webSocket.sendTXT(
                    "5");  // socket.io upgrade confirmation message (required)
                break;

            case WStype_DISCONNECTED:
                isConnected = false;
                break;
        }
    }

   public:
    void setupWiFi() {
        WiFi.setHostname(NODE_NAME);
        wifiManager.autoConnect(NODE_NAME "Setup", SETUP_PASSWORD);
    }

    void connectSocket() {
        webSocket.beginSocketIO(SOCKET_SERVER_HOST, SOCKET_SERVER_PORT,
                                SOCKET_SERVER_PATH);
        webSocket.onEvent([&](WStype_t type, uint8_t *payload, size_t lenght) {
            handleWebSocketEvent(type, payload, lenght);
        });
    }

    void emit(String event, String payload) {
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
