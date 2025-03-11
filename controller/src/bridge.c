#include <stdint.h>
#include "bridge.h"
#include "common.h"

#define POLL_TIME      SEC(1)
#define DRIVE_TIME     SEC(5)
#define PASS_THRESHOLD 10

int process(Bridge *const self, const int arg) {
    typedef struct {
        bool northArrival : 1,
             northEntry   : 1,
             southArrival : 1,
             southEntry   : 1;
        // TODO: remove.
        unsigned int _pad : 4;
    } SensorRead;

    union {
        int raw;
        SensorRead read;
    } pun = { arg };
    SensorRead read = pun.read;
    assert(read._pad == 0);

    if (read.northArrival)
        self->northQueue++;
    if (read.southArrival)
        self->southQueue++;
    // Other cases ignored.

    int err = SYNC(self->display, print, self);
    assert(err == 0);

    return 0;
}

int leave(Bridge *const self, __attribute__((unused)) const int _x) {
    assert(self->onBridge);
    self->onBridge--;

    int err = SYNC(self->display, print, self);
    assert(err == 0);

    return 0;
}

static void enter(Bridge *const self) {
    self->onBridge++;
    AFTER(DRIVE_TIME, self, leave, 0);

    int err;
    if (self->direction == NORTH) {
        self->northQueue--;
        err = SYNC(self->writer, send, NORTH_GREEN);
    } else {
        self->southQueue--;
        err = SYNC(self->writer, send, SOUTH_GREEN);
    }
    assert(err == 0);

    err = SYNC(self->display, print, self);
    assert(err == 0);
}

int poll(Bridge *const self, __attribute__((unused)) const int _x) {
    bool switchReady = self->passed < PASS_THRESHOLD;
    unsigned int *activeQueue, *waitQueue;
    if (self->direction == SOUTH) {
        activeQueue = &self->southQueue;
        waitQueue   = &self->northQueue;
    } else {
        activeQueue = &self->northQueue;
        waitQueue   = &self->southQueue;
    }

    if (!self->onBridge) {
        if (self->southQueue && (!self->northQueue || self->direction == NORTH)) {
            self->direction = SOUTH;
            self->passed = 1;
            self->open = true;
            enter(self);
        } else if (self->northQueue && (!self->southQueue || self->direction == SOUTH)) {
            self->direction = NORTH;
            self->passed = 1;
            self->open = true;
            enter(self);
        }
        // both sides empty, pass
    } else if (switchReady && *waitQueue) {
        self->open = false;
        int err = SYNC(self->writer, send, BOTH_RED);
        assert(err == 0);
    } else if (self->open && *activeQueue && (!switchReady || !*waitQueue)) { // TODO: remove unnecessary check at the end
        self->passed++;
        enter(self);
    } else {
        // TODO: are there other cases?
        assert(!self->onBridge && (!self->open || !*activeQueue));
    }

    AFTER(POLL_TIME, self, poll, 0);

    return 0;
}
