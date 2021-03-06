#include "PowerManager.h"

#include <Arduino.h>

#include "utils/RunningAverageFilter.h"

class PowerManager::Impl {
private:
    int batteryPin;
    int usbPin;
    float voltageCompensation;
    RunningAverageFilter<float> batteryVoltageFilter;
    RunningAverageFilter<float> usbVoltageFilter;

    float readVoltage(int pin) {
        return analogRead(pin) 
               * 2  // Voltage is divided into half. See https://www.wemos.cc/en/latest/_static/files/sch_d32_v1.0.0.pdf
               * 3.3  // GPIO can read up to 3.3v.
               * voltageCompensation / 4096;
    }

public:
    Impl(int batteryPin, int usbPin, float voltageCompensation) {
        this->batteryPin = batteryPin;
        this->usbPin = usbPin;
        this->voltageCompensation = voltageCompensation;
    }

    float readBatteryVoltage() { 
        return batteryVoltageFilter.filter(readVoltage(batteryPin)); 
    }

    float readUsbVoltage() { 
        return usbVoltageFilter.filter(readVoltage(usbPin)); 
    }

    int readBatteryPercentage() {
        float voltage = readBatteryVoltage();

        if (voltage >= 4.20) return 100;

        if (voltage <= 3.50) return 0;

        // Voltage drop is not linear.
        // Copied from https://github.com/G6EJD/ESP32-e-Paper-Weather-Display/blob/5c3df15a3fe2b0005fba99e48a493ae4fc1b3a94/examples/Waveshare_2_13_T5/Waveshare_2_13_T5.ino#L490
        return + 2836.9625 * (voltage * voltage * voltage * voltage) 
               - 43987.4889 * (voltage * voltage * voltage) 
               + 255233.8134 * (voltage * voltage) 
               - 656689.7123 * voltage 
               + 632041.7303;
    }
};

PowerManager::PowerManager(int batteryPin, int usbPin, float voltageCompensation): impl(new Impl(batteryPin, usbPin, voltageCompensation)) {}
PowerManager::~PowerManager() {}

float PowerManager::readBatteryVoltage() { 
    return impl->readBatteryVoltage(); 
}

int PowerManager::readBatteryPercentage() {
    return impl->readBatteryPercentage();
}

bool PowerManager::isUsbPowered() { 
    return readUsbVoltage() > 4.5; 
}

float PowerManager::readUsbVoltage() { 
    return impl->readUsbVoltage(); 
}

void PowerManager::loop() {
    // This increases accuracy in measure value.
    readBatteryVoltage();
    readUsbVoltage();
}
