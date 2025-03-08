#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "TinyTimber.h"

// To "lock" the LCD, `Display` has to be a singleton.
// As such, instance parameters cannot be used.
typedef struct {
    Object super;
} Display;

#define initDisplay() { initObject() }

int print(const Display *const, const int);

#endif
