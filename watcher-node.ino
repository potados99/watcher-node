/**
 * Reads battery and USB voltage on WEMOS LOLIN D32.
 *
 * Connect USB pin to 34 through voltage divider: use two 100kOhm resistors to cut the voltage in half.
 * For Serial.printf(), see https://playground.arduino.cc/Main/Printf/
 */

typedef void (*callback)(void);

const int PIN_BAT = 35;
const int PIN_USB = 34;
const float VOLTAGE_COMPENSATION = 1.086;

bool usbConnected = false;

void setup() {
    Serial.begin(115200);
}

void loop() {
    
    dumpPowerStatusToSerial();
    
    onDetectUsbDisconnection(
                             notifyUsbDisconnected
                             );
    
    update();
    
    delay(1000);
}

void dumpPowerStatusToSerial() {
    Serial.printf("Battery: %fv (%i%%), USB: %s.\n", getBatteryVoltage(), getBatteryPercentage(), isUsbPowered() ? (String(getUsbVoltage()) + "v").c_str() : "not connected");
}

void onDetectUsbDisconnection(callback action) {
    bool wasConnected = usbConnected;
    bool notConnectedNow = !isUsbPowered();
    
    if (wasConnected && notConnectedNow) {
        action();
    }
}

void notifyUsbDisconnected() {
    Serial.printf("Usb connection lost!!\n");
}

void update() {
    usbConnected = isUsbPowered();
}




float getBatteryVoltage() {
    return getVoltage(PIN_BAT);
}

int getBatteryPercentage() {
    float voltage = getBatteryVoltage();
    
    if (voltage >= 4.17)
        return 100;
    
    if (voltage <= 3.50)
        return 0;
    
    return 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
}

bool isUsbPowered() {
    return getUsbVoltage() > 4.5;
}

float getUsbVoltage() {
    return getVoltage(PIN_USB);
}

float getVoltage(int sourcePinNumber) {
    return analogRead(sourcePinNumber)
    * 2     // Voltage is divided into half. See https://www.wemos.cc/en/latest/_static/files/sch_d32_v1.0.0.pdf
    * 3.3   // GPIO can read up to 3.3v.
    * VOLTAGE_COMPENSATION
    / 4096;
}
