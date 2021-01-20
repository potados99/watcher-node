#ifndef Reporter_h
#define Reporter_h

#include <WiFiManager.h>

class Reporter {
private:
    WiFiManager mWifiManager;
public:
    void setup();
    
    void emit(const char *event, bool value);
    void emit(const char *event, int value);
};

#endif /* Reporter_h */
