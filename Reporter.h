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
    void emit(const char *event, const char *value);

    void loop();
};

#endif /* Reporter_h */
