#ifndef Reporter_h
#define Reporter_h

class Reporter {
private:
    class Impl;
    Impl *impl;

public:    
    Reporter();
    ~Reporter();
    
    void setup();
    
    void emit(String event, bool value);
    void emit(String event, int value);
    void emit(String event, String value);

    void loop();
};

#endif /* Reporter_h */
