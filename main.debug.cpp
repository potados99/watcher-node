#ifndef ESP_PLATFORM

#include "routines.h"

int main(int argc, const char * argv[]) {
    setup();
    
    while (true) {
        loop();
    }
}

#endif
