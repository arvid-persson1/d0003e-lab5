#include <stdint.h>
#include "bridge.h"
#include "init.h"

#define NORTH_ARRIVAL SET(7)
#define SOUTH_ARRIVAL SET(5)

#define POLL_TIME      SEC(1)
#define DRIVE_TIME     SEC(5)
#define PASS_THRESHOLD 10

#if defined(__OPTIMIZE__) && __OPTIMIZE__ > 2
    #define assert(x) { if (!x) __builtin_unreachable(); }
#else
    #define assert(x) { if (!x) while (1); }
#endif

static bool goingSouth(State s) {
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

void enter(Bridge *const self, State direction) {
    self->onBridge++;
    self->passed++;
    AFTER(DRIVE_TIME, self, leave, 0);

    if (goingSouth(direction)) {
        self->southQueue--;
        // TODO: usart send
    } else {
        self->northQueue--;
        // TODO: usart send
    }
}

int poll(Bridge *const self, __attribute__((unused)) const int _x) {
    if (self->passed >= PASS_THRESHOLD) {
        if (self->state == OPEN_NORTH && self->southQueue) {
            self->passed = 0;
            self->state = CLOSED_NORTH;
        } else if (self->state == OPEN_SOUTH && self->northQueue) {
            self->passed = 0;
            self->state = CLOSED_SOUTH;
        }
    }

    if (self->onBridge == 0) {
        // TODO: fairness
        if (self->northQueue) {
            if (goingSouth(self->state))
                self->passed = 0;

            self->northQueue--;
            self->state = OPEN_NORTH;
        } else if (self->southQueue) {
            if (!goingSouth(self->state))
                self->passed = 0;

            self->southQueue--;
            self->state = OPEN_SOUTH;
        }
        self->onBridge++;
        self->passed++;
        AFTER(DRIVE_TIME, self, leave, 0);
    } else if (self->state == OPEN_NORTH) {
        if (self->northQueue) {
            self->northQueue--;
            self->onBridge++;
            self->passed++;
            AFTER(DRIVE_TIME, self, leave, 0);
        } else if (self->southQueue) {
            self->state = CLOSED_NORTH;
        }
    } else if (self->state == OPEN_SOUTH) {
        if (self->southQueue) {
            self->southQueue--;
            self->onBridge++;
            self->passed++;
            AFTER(DRIVE_TIME, self, leave, 0);
        } else if (self->northQueue) {
            self->state = CLOSED_SOUTH;
        }
    } else {
        assert(false);
    }

    AFTER(POLL_TIME, self, poll, 0);

    return 0;
}

int leave(Bridge *const self, __attribute__((unused)) const int _x) {
    assert(self->onBridge);
    self->onBridge--;

    return 0;
}
