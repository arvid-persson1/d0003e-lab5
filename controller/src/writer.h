#ifndef _WRITER_H
#define _WRITER_H

#include "TinyTimber.h"

typedef struct {
    Object super;
} Writer;

#define initWriter() { initObject() }

#endif
