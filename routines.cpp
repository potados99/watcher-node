#include "IO.h"
#include "config.h"
#include "Arduino.h"
#include "Reporter.h"
#include "PowerManager.h"

#include "utils/Detector.h"


PowerManager pm(PIN_BAT, PIN_USB, VOLTAGE_COMPENSATION);
Detector<bool> usbConnectionDetector;
Reporter reporter(SOCKET_SERVER_HOST, SOCKET_SERVER_PORT, SOCKET_SERVER_PATH);

void setup() {
    IO::setup();

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
    usbConnectionDetector.loop();
}

void dumpPowerStatus() {
    IO::printf("Battery: %fv (%i%%), ", pm.readBatteryVoltage(), pm.readBatteryPercentage());
    
    if (pm.isUsbPowered()) {
        IO::printf("USB: %fv.\n", pm.readUsbVoltage());
    } else {
        IO::printf("USB disconnected.\n");
    }
}