#ifndef arduino_h
#define arduino_h

#ifdef ESP_PLATFORM
#include <Arduino.h>
#else
#include "mock.h"
#endif /* ESP_PLATFORM */

#endif /* arduino_h */
