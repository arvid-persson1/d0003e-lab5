#ifndef _WRITER_H
#define _WRITER_H

#include <stdint.h>
#include <stdbool.h>
#include "TinyTimber.h"

typedef struct {
    Object super;
    // TODO: add buffer slots?
    uint8_t buf;
    bool stored : 1,
         ready  : 1;
} Writer;

#define initWriter() { .super = initObject() }

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
