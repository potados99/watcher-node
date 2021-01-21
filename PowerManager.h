#ifndef PowerManager_h
#define PowerManager_h

#include "config.h"

class PowerManager {
private:
    float mVoltageCompensation = VOLTAGE_COMPENSATION;
    
    int mBatteryPin;
    int mUsbPin;
    
    float readVoltage(int pin);
    
public:
    
    PowerManager(int batteryPin, int usbPin): mBatteryPin(batteryPin), mUsbPin(usbPin) {}
    
    float readBatteryVoltage();
    int readBatteryPercentage();
    
    bool isUsbPowered();
    float readUsbVoltage();
};

#endif /* PowerManager_h */
