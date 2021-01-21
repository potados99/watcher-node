#ifndef detector_h
#define detector_h

template <typename T>
class Detector {
private:
    T lastValue;

    T (*producer)(void);
    void (*callback)(T);

public:
    void watch(T (*action)(void)) {
        producer = action;
    }

    void onChange(void (*action)(T)) {
        callback = action;
    }

    void loop() {
        if (!producer) {
            return;
        }

        if (!callback) {
            return;
        }

        T newValue = producer();
        if (lastValue != newValue) {
            callback(newValue);
        }

        lastValue = newValue;
    }
};


#endif /* detector_h */