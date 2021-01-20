#include "Reporter.h"

#include <WiFi.h>

void Reporter::setup() {
    WiFi.setHostname("Watcher");
    mWifiManager.autoConnect("WatcherSetup", "potados90809");
}

void Reporter::emit(const char *event, bool value) {
    
}

void Reporter::emit(const char *event, int value) {
    
}
