#ifndef running_average_filter_h
#define running_average_filter_h

template <typename T/*Must be a number*/>
class RunningAverageFilter {
private:
    static const int bufferSize = 32;
    
    T runningAverageBuffer[bufferSize];
    int nextRunningAverage = 0;

    bool bufferNotFilledYet = true;

public:
    T filter(T raw) {
        runningAverageBuffer[nextRunningAverage++] = raw;
        if (nextRunningAverage >= bufferSize) {
            bufferNotFilledYet = false;
            nextRunningAverage = 0;
        }

        if (bufferNotFilledYet) {
            return raw;
        }

        T average = 0;
        for (int i = 0; i < bufferSize; ++i) {
            average += runningAverageBuffer[i];
        }
        average /= bufferSize;

        return average;
    }
};

#endif /* running_average_filter_h */