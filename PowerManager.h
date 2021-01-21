#ifndef PowerManager_h
#define PowerManager_h

class PowerManager {
private:
    class Impl;
    Impl *impl;
    
public:
    PowerManager(int batteryPin, int usbPin, float voltageCompensation);
    ~PowerManager();
    
    float readBatteryVoltage();
    int readBatteryPercentage();
    
    bool isUsbPowered();
    float readUsbVoltage();
};

#endif /* PowerManager_h */
