#include "writer.h"
#include "common.h"

static inline void writePort(Writer *const self, const uint8_t data) {
    self->ready = false;
    UDR = data;
}

int send(Writer *const self, const int arg) {
    uint8_t data = (uint8_t)arg;
    if (self->ready) {
        assert(UCSRA & SET(UDRE));
        writePort(self, data);
    } else {
        // If this is violated, the buffer is too small or the USART is
        // blocked / otherwise not working.
        assert (!self->stored);
        self->buf = data;
        self->stored = true;
    }

    return 0;
}

int ready(Writer *const self, __attribute__((unused)) const int _x) {
    assert(UCSRA & SET(UDRE));
    if (self->stored) {
        self->stored = false;
        writePort(self, self->buf);
    } else {
        self->ready = true;
    }

    return 0;
}
