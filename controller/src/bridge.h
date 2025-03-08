#ifndef _BRIDGE_H
#define _BRIDGE_H

#include "TinyTimber.h"
#include "writer.h"

// NOTE: Variant order must be preserved for `goingSouth`.
typedef enum {
    OPEN_NORTH,
    CLOSED_NORTH,
    OPEN_SOUTH,
    CLOSED_SOUTH
} State;

typedef struct {
    Object super;
    Writer *writer;
    unsigned int northQueue,
                 southQueue,
                 onBridge,
                 passed;
    State state;
} Bridge;

#define initBridge(w) { .super = initObject(), .writer = w }

int process(Bridge *const, __attribute__((unused)) const int);
int poll(Bridge *const, __attribute__((unused)) const int);

#endif
