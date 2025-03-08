#include <stdint.h>
#include "bridge.h"
#include "common.h"

#define NORTH_ARRIVAL SET(7)
#define SOUTH_ARRIVAL SET(5)

#define POLL_TIME      SEC(1)
#define DRIVE_TIME     SEC(5)
#define PASS_THRESHOLD 10

static bool isSouth(const State s) {
    return (int)s & 1;
}

int process(Bridge *const self, const int arg) {
    uint8_t udr = (uint8_t)arg;

    if (udr & NORTH_ARRIVAL)
        self->northQueue++;
    if (udr & SOUTH_ARRIVAL)
        self->southQueue++;

    return 0;
}

int leave(Bridge *const self, __attribute__((unused)) const int _x) {
    assert(self->onBridge);
    self->onBridge--;

    return 0;
}

static void enter(Bridge *const self, const State direction) {
    self->onBridge++;
    self->passed++;
    AFTER(DRIVE_TIME, self, leave, 0);

    if (isSouth(direction)) {
        self->southQueue--;
        // TODO: usart send
    } else {
        self->northQueue--;
        // TODO: usart send
    }
}

static void checkSwitch(Bridge *const self) {
    if (self->passed < PASS_THRESHOLD)
        return;

    if (self->state == OPEN_NORTH && self->southQueue) {
        self->passed = 0;
        self->state = CLOSED_NORTH;
    } else if (self->state == OPEN_SOUTH && self->northQueue) {
        self->passed = 0;
        self->state = CLOSED_SOUTH;
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
    }

    enter(self, self->state);
}

static void goingNorth(Bridge *const self) {
    if (self->northQueue)
        enter(self, self->state);
    else if (self->southQueue)
        self->state = CLOSED_NORTH;
}

static void goingSouth(Bridge *const self) {
    if (self->southQueue)
        enter(self, self->state);
    else if (self->northQueue)
        self->state = CLOSED_SOUTH;
}

int poll(Bridge *const self, __attribute__((unused)) const int _x) {
    checkSwitch(self);

    if (self->onBridge == 0) 
        emptyBridge(self);
    else if (self->state == OPEN_NORTH)
        goingNorth(self);
    else if (self->state == OPEN_SOUTH)
        goingSouth(self);
    else
        assert(false);

    AFTER(POLL_TIME, self, poll, 0);

    return 0;
}
