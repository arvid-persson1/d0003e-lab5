#include <stdint.h>
#include "bridge.h"
#include "common.h"

#define POLL_TIME      SEC(1)
#define DRIVE_TIME     SEC(5)
#define PASS_THRESHOLD 10

static inline bool isSouth(const State s) {
    return (int)s & 1;
}

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

    return 0;
}

int leave(Bridge *const self, __attribute__((unused)) const int _x) {
    assert(self->onBridge);
    self->onBridge--;

    return 0;
}

static void enter(Bridge *const self) {
    self->onBridge++;
    self->passed++;
    AFTER(DRIVE_TIME, self, leave, 0);

    if (isSouth(self->state)) {
        self->southQueue--;
        ASYNC(self->writer, send, SOUTH_GREEN);
    } else {
        self->northQueue--;
        ASYNC(self->writer, send, NORTH_GREEN);
    }
}

static void checkSwitch(Bridge *const self) {
    if (self->passed < PASS_THRESHOLD)
        return;

    if (self->state == OPEN_NORTH && self->southQueue) {
        self->passed = 0;
        self->state = CLOSED_NORTH;
        ASYNC(self->writer, send, BOTH_RED);
    } else if (self->state == OPEN_SOUTH && self->northQueue) {
        self->passed = 0;
        self->state = CLOSED_SOUTH;
        ASYNC(self->writer, send, BOTH_RED);
    }
}

static void emptyBridge(Bridge *const self) {
    // TODO: fairness
    if (self->northQueue) {
        if (isSouth(self->state))
            self->passed = 0;

        self->state = OPEN_NORTH;
    } else if (self->southQueue) {
        if (!isSouth(self->state))
            self->passed = 0;

        self->state = OPEN_SOUTH;
    } else {
        return;
    }

    enter(self);
}

static void goingNorth(Bridge *const self) {
    if (self->northQueue)
        enter(self);
    else if (self->southQueue)
        self->state = CLOSED_NORTH;
}

static void goingSouth(Bridge *const self) {
    if (self->southQueue)
        enter(self);
    else if (self->northQueue)
        self->state = CLOSED_SOUTH;
}

int poll(Bridge *const self, __attribute__((unused)) const int _x) {
    checkSwitch(self);

    if (!self->onBridge) 
        emptyBridge(self);
    else if (self->state == OPEN_NORTH)
        goingNorth(self);
    else if (self->state == OPEN_SOUTH)
        goingSouth(self);
    else
        assert(false);

    ASYNC(self->display, print, self);
    AFTER(POLL_TIME, self, poll, 0);

    return 0;
}
