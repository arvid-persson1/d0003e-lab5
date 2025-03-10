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

// TODO: is the writer initially ready?
#define initWriter() { .super = initObject(), .stored = false, .ready = true }

#endif
