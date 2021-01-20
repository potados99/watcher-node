//
//  main.release.c
//  watcher-node
//
//  Created by Potados on 2021/01/20.
//

#ifdef ESP_PLATFORM

#include <Arduino.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
    
    dumpPowerStatusToSerial();
    
    onDetectUsbDisconnection(notifyUsbDisconnected);
    
    update();
    
    delay(1000);
}

#endif
