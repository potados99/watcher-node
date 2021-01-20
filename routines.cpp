#include "routines.h"

#include "IO.h"
#include "Arduino.h"
#include "PowerManager.h"

PowerManager pm(35/*Battery*/, 34/*USB*/);

bool usbConnected = false;

void setup() {
    IO::setup();
}

void loop() {
    dumpPowerStatus();
    
    onDetectUsbDisconnection(notifyUsbDisconnected);
    
    reportStatus();
    
    delay(1000);
}

void dumpPowerStatus() {
    IO::printf("Battery: %fv (%i%%), ", pm.readBatteryVoltage(), pm.readBatteryPercentage());
    
    if (pm.isUsbPowered()) {
        IO::printf("USB: %fv.\n", pm.readUsbVoltage());
    } else {
        IO::printf("USB disconnected.\n");
    }
}

void onDetectUsbDisconnection(callback action) {
    bool wasConnected = usbConnected;
    bool currentlyConnected = pm.isUsbPowered();
    
    if (wasConnected && !currentlyConnected) {
        action();
    }
    
    usbConnected = currentlyConnected;
}

void notifyUsbDisconnected() {
    IO::printf("Usb connection lost!!\n");
}

void reportStatus() {
    // 
}
