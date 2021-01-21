#include "IO.h"
#include "config.h"
#include "Arduino.h"
#include "Reporter.h"
#include "PowerManager.h"

#include "utils/Task.h"
#include "utils/Detector.h"

PowerManager pm(PIN_BAT, PIN_USB, VOLTAGE_COMPENSATION);
Reporter reporter(SOCKET_SERVER_HOST, SOCKET_SERVER_PORT, SOCKET_SERVER_PATH);

Detector<bool> usbConnectionDetector;
Task dumpPowerStatusTask;

void setup() {
    IO::setup();

    dumpPowerStatusTask.runOnEvery(1, []() {
        IO::printf("Battery: %fv (%i%%), ", pm.readBatteryVoltage(), pm.readBatteryPercentage());
        
        if (pm.isUsbPowered()) {
            IO::printf("USB: %fv.\n", pm.readUsbVoltage());
        } else {
            IO::printf("USB disconnected.\n");
        }
    });

    usbConnectionDetector.watch([]() {
        return pm.isUsbPowered();
    });

    usbConnectionDetector.onChange([](bool connected) {
        IO::printf("Usb connection lost!!\n");
        reporter.emit("usb-connection", false);
    });

    reporter.setup();
}

void loop() {            
    reporter.loop();
    dumpPowerStatusTask.loop();
    usbConnectionDetector.loop();
}