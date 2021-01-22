#ifndef Reporter_h
#define Reporter_h

class Reporter {
private:
    class Impl;
    Impl *impl;

public:    
    Reporter(const char *host, int port, const char *path);
    ~Reporter();
    
    void setup();
    
    void emit(const char *event, bool value);
    void emit(const char *event, int value);
    void emit(const char *event, float value);
    void emit(const char *event, const char *fmt, ...);

    void updateProp(const char *propName, bool propValue);
    void updateProp(const char *propName, int propValue);
    void updateProp(const char *propName, float propValue);
    void updateProp(const char *propName, const char *propValue);

    void onUpdateRequest(void (*action)(void));

    void loop();
};

#endif /* Reporter_h */
