#ifndef _BRIDGE_H
#define _BRIDGE_H

#include "TinyTimber.h"

typedef enum {
    OPEN_NORTH,
    CLOSED_NORTH,
    OPEN_SOUTH,
    CLOSED_SOUTH
} State;

typedef struct {
    Object super;
    unsigned int northQueue;
    unsigned int southQueue;
    unsigned int onBridge;
    unsigned int passed;
    State state;
} Bridge;

#define initBridge() { .super = initObject() }

int process(Bridge *const, __attribute__((unused)) const int);
int poll(Bridge *const, __attribute__((unused)) const int);
int leave(Bridge *const, __attribute__((unused)) const int);

#endif
