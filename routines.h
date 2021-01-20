#ifndef routines_h
#define routines_h

#include "types.h"

void setup();
void loop();

void dumpPowerStatus();
void onDetectUsbDisconnection(callback action);
void notifyUsbDisconnected();
void reportStatus();

#endif /* routines_h */
