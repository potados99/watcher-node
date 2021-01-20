#ifndef mock_h
#define mock_h

#include <unistd.h>

static inline void delay(int millis) {
    usleep(millis * 1000);
}

static inline int analogRead(int pin) {
    return 2000;
}

#endif /* mock_h */
