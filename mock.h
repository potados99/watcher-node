#ifndef mock_h
#define mock_h

#include <unistd.h>
#include <stdio.h>

static inline void delay(int millis) {
    usleep(millis * 1000);
}

static inline int analogRead(int pin) {
    return 2100;
}

class HardwareSerial {
public:
    void begin(int baud) {}
    void print(const char *str) {
        printf("%s", str);
    }
};

extern HardwareSerial Serial;

#endif /* mock_h */
