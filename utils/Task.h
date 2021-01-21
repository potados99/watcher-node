#ifndef task_h
#define task_h

#include <Arduino.h>

class Task {
private:
    void (*task)(void);
    unsigned long lastExecution = 0;
    int intervalMillis;

public:
    void runOnEvery(int seconds, void (*action)(void)) {
        intervalMillis = seconds * 1000;
        task = action;
    }

    void loop() {
        auto now = millis();

        if ((now - lastExecution) > intervalMillis) {
            task();
            lastExecution = now;
        }
    }
};

#endif /* task_h */