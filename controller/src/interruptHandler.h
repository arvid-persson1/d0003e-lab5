#ifndef _HANDLER_H
#define _HANDLER_H

#include "TinyTimber.h"
#include "bridge.h"
#include "writer.h"

typedef struct {
    Object super;
    Bridge *bridge;
    Writer *writer;
} InterruptHandler;

#define initHandler(b, w) { initObject(), b, w }

int recv(const InterruptHandler *const, __attribute__((unused)) const int);
int drempty(const InterruptHandler *const, __attribute__((unused)) const int);

#endif
