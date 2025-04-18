#ifndef _BRIDGE_H
#define _BRIDGE_H

#include <stdint.h>
#include "TinyTimber.h"
#include "writer.h"
#include "display.h"

typedef enum {
    NORTH,
    SOUTH
} Direction;

typedef struct {
    Object super;
    Writer *writer;
    Display *display;
    unsigned int northQueue,
                 southQueue,
                 onBridge,
                 passed;
    bool open;
    Direction direction;
} Bridge;

#define initBridge(w, d) { .super = initObject(), .writer = w, .display = d, .northQueue = 0, .southQueue = 0, .onBridge = 0 }

int process(Bridge *const, __attribute__((unused)) const int);
int poll(Bridge *const, __attribute__((unused)) const int);

typedef struct {
    bool northGreen : 1,
         northRed   : 1,
         southGreen : 1,
         southRed   : 1;
    // TODO: remove.
    unsigned int _pad : 4;
} LightStatus;

union _WritePun {
    LightStatus ls;
    uint8_t arg;
};

#define NORTH_GREEN ((union _WritePun){ { .northGreen = true, .southRed = true } }).arg
#define SOUTH_GREEN ((union _WritePun){ { .southGreen = true, .northRed = true } }).arg
#define BOTH_RED    ((union _WritePun){ { .northRed   = true, .southRed = true } }).arg

int send(Writer *const, const int);
int ready(Writer *const, __attribute__((unused)) const int);

#endif
