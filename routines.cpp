#include "config.h"
#include "Reporter.h"
#include "PowerManager.h"

#include "utils/Task.h"
#include "utils/Detector.h"

#include <Arduino.h>

PowerManager powerManager(PIN_BAT, PIN_USB, VOLTAGE_COMPENSATION);
Reporter reporter(SOCKET_SERVER_HOST, SOCKET_SERVER_PORT, SOCKET_SERVER_PATH);

Task dumpPowerStatusTask;
Task reportBatteryTask;
Detector<bool> usbConnectionDetector;

void setup() {
    Serial.begin(115200);
    reporter.setup();

    dumpPowerStatusTask.runOnEverySecond(1, []() {
        Serial.printf("Battery: %fv (%i%%), ", powerManager.readBatteryVoltage(), powerManager.readBatteryPercentage());
        
        if (powerManager.isUsbPowered()) {
            Serial.printf("USB: %fv.\n", powerManager.readUsbVoltage());
        } else {
            Serial.printf("USB disconnected.\n");
        }
    });

    reportBatteryTask.runOnEverySecond(5, []() {
        reporter.emit("battery", "{\"voltage\": %f, \"percentage\": %d}", powerManager.readBatteryVoltage(), powerManager.readBatteryPercentage());
    });

    usbConnectionDetector.watch([]() {
        return powerManager.isUsbPowered();
    });

    usbConnectionDetector.onChange([](bool connected) {
        Serial.printf("USB connected: %s\n", connected ? "true" : "false");

        reporter.emit("usb:connected", connected ? "true" : "false");
    });
}

void loop() {            
    reporter.loop();

    dumpPowerStatusTask.loop();
    reportBatteryTask.loop();
    usbConnectionDetector.loop();
}