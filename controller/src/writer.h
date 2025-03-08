#ifndef _WRITER_H
#define _WRITER_H

#include <stdint.h>
#include <stdbool.h>
#include "TinyTimber.h"

typedef struct {
    Object super;
    // TODO: add buffer slots?
    uint8_t buf;
    unsigned int stored : 1;
    unsigned int ready  : 1;
} Writer;

#define initWriter() { initObject(), 0, false, false }

int write(Writer *const, const int);
int ready(Writer *const, __attribute__((unused)) const int);

#endif
