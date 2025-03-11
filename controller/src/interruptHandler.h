#ifndef _HANDLER_H
#define _HANDLER_H

#include "TinyTimber.h"
#include "bridge.h"
#include "writer.h"

typedef struct {
    Object super;
    Bridge *bridge;
} InterruptHandler;

#define initHandler(b) { initObject(), b }

int recv(const InterruptHandler *const, __attribute__((unused)) const int);

#endif
