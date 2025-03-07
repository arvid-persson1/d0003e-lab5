#ifndef _HANDLER_H
#define _HANDLER_H

#include "TinyTimber.h"
#include "bridge.h"

typedef struct {
    Object super;
    Bridge *bridge;
} InterruptHandler;

#define initHandler(b) { initObject(), b }

int handle(const InterruptHandler *const, __attribute__((unused)) const int);

#endif
