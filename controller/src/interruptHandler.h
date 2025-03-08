#ifndef _HANDLER_H
#define _HANDLER_H

#include "TinyTimber.h"
#include "bridge.h"
#include "writer.h"

typedef struct {
    Object super;
    Writer *writer;
    Bridge *bridge;
} InterruptHandler;

#define initHandler(w, b) { initObject(), w, b }

int drempty(const InterruptHandler *const, __attribute__((unused)) const int);
int recv(const InterruptHandler *const, __attribute__((unused)) const int);

#endif
