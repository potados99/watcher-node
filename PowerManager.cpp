#include "PowerManager.h"

#include "Arduino.h"

float PowerManager::readVoltage(int pin) {
    return analogRead(pin)
    * 2     // Voltage is divided into half. See https://www.wemos.cc/en/latest/_static/files/sch_d32_v1.0.0.pdf
    * 3.3   // GPIO can read up to 3.3v.
    * mVoltageCompensation
    / 4096;
}

float PowerManager::readBatteryVoltage() {
    return readVoltage(mBatteryPin);
}

int PowerManager::readBatteryPercentage() {
    float voltage = readBatteryVoltage();
    
    if (voltage >= 4.17)
        return 100;
    
    if (voltage <= 3.50)
        return 0;
    
    return
    + 2836.9625 * (voltage*voltage*voltage*voltage)
    - 43987.4889 * (voltage*voltage*voltage)
    + 255233.8134 * (voltage*voltage)
    - 656689.7123 * voltage
    + 632041.7303;
}

bool PowerManager::isUsbPowered() {
    return readUsbVoltage() > 4.5;
}

float PowerManager::readUsbVoltage() {
    return readVoltage(mUsbPin);
}
