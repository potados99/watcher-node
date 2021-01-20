#include "Reporter.h"

#include <ESP8266WiFi.h>

WiFiManager manager;

void Reporter::setup() {
    WiFi.hostname("Watcher");
    manager.autoConnect((mName + "_setup").c_str(), "potados");
}

void Reporter::emit(const char *event, bool value) {
    
}

void Reporter::emit(const char *event, int value) {
    
}
