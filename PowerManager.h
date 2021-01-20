#ifndef PowerManager_h
#define PowerManager_h

class PowerManager {
private:
    float mVoltageCompensation = 1.086;
    
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
