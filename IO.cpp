#include "IO.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>

void IO::setup() {
    Serial.begin(115200);
}

void IO::printf(const char *format, ...) {
    char buf[128];
    
    va_list args;
    va_start (args, format);
    vsnprintf(buf, sizeof(buf)/sizeof(char), format, args);
    va_end (args);
    
    Serial.print(buf);
}
