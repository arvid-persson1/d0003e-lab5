#include "writer.h"
#include "common.h"

static void send(Writer *const self, const uint8_t data) {
    self->ready = false;
    UDR = data;
}

int write(Writer *const self, const int arg) {
    uint8_t data = (uint8_t)arg;
    if (self->ready) {
        // By definition.
        assert(UCSRA & SET(UDRE));
        send(self, data);
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
    // By definition.
    assert(UCSRA & SET(UDRE));
    if (self->stored) {
        self->stored = false;
        send(self, self->buf);
    } else {
        self->ready = true;
    }

    return 0;
}
