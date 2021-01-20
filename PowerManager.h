//
//  PowerManager.hpp
//  watcher-node
//
//  Created by Potados on 2021/01/20.
//

#ifndef PowerManager_hpp
#define PowerManager_hpp

class PowerManager {
private:
    float mVoltageCompensation = 1.086;
    
    int mBatteryPin;
    int mUsbPin;
    
    float readVoltage(int pin);
    int gpioAnalogRead(int pin);
    
public:
    
    PowerManager(int batteryPin, int usbPin): mBatteryPin(batteryPin), mUsbPin(usbPin) {}
    
    float readBatteryVoltage();
    int readBatteryPercentage();
    
    bool isUsbPowered();
    float readUsbVoltage();
};

#endif /* PowerManager_hpp */
