#include <stdbool.h>
#include "TinyTimber.h"

// TODO: read status bits (FE, DOR, RXB8) before UDR reg
// CHR9 -> UCSZ2, OR -> DOR

#define assert(x) { if (!x) while (1); }

void init(void) {
    // LCD setup; see previous labs.
    LCDCRA = SET(LCDEN)   | SET(LCDAB);
    LCDCRB = SET(LCDCS)   | SET(LCDMUX1) | SET(LCDMUX0) | SET(LCDPM2)  | SET(LCDPM1)  | SET(LCDPM0);
    LCDFRR = SET(LCDCD2)  | SET(LCDCD1)  | SET(LCDCD0);
    LCDCCR = SET(LCDCC3)  | SET(LCDCC2)  | SET(LCDCC1)  | SET(LCDCC0);

    // UBRR calculation from p.154 in ATmega169 manual.
    // XXX: writing high byte is unnecessary with current config.
    UBRR0H = (uint8_t)(UBRRVAL << 8);
    UBRR0L = (uint8_t)(UBRRVAL);

    // Receive and transmit.
    UCSR0B = SET(RXEN0)  | SET(TXEN0)  | SET(RXCIE0);
    // 8 bit char.
    UCSR0C = SET(UCSZ01) | SET(UCSZ00);
}

typedef struct {
    Object super;
    State *state;
} InterruptHandler;

int handle(const InterruptHandler *const self, __attribute__((unused)) const int _x) {
    ASYNC(self->state, process, UDR0);
    return 0;
}

typedef enum {
    OPEN_NORTH = 0,
    OPEN_SOUTH = 1,
    CLOSED_NORTH = 2,
    CLOSED_SOUTH = 3
} Direction;

bool isSouth(Direction direction) {
    return direction & 1;
}

typedef struct {
    Object super;
    unsigned int northQueue;
    unsigned int southQueue;
    unsigned int onBridge;
    Direction direction;
    Turn turn;
} State;

#define ENTRY_DELAY
#define DRIVE_TIME
#define SWAP_TIME

int process(State *const self, int arg) {
    uint8_t input = (uint8_t)arg;

    // North arrival
    if (input & (1 << 7))
        self->northQueue++;

    // South arrival
    if (input & (1 << 5))
        self->southQueue++;
}

int poll(State *const self, int arg) {
    if (self->onBridge == 0) {
        if (isSouth(self->turn)) {
            if (self->southQueue) {
                send(SOUTH_GREEN);
                self->southQueue--;
                self->direction = OPEN_SOUTH;
                self->onBridge++;
                AFTER(DRIVE_TIME, self, leave, self->direction);
            } else if (self->northQueue) {
                send(NORTH_GREEN);
                self->northQueue--;
                self->direction = OPEN_NORTH;
                self->onBridge++;
                AFTER(DRIVE_TIME, self, leave, self->direction);
            }
        } else {
            if (self->northQueue) {
                send(NORTH_GREEN);
                self->northQueue--;
                self->direction = OPEN_NORTH;
                self->onBridge++;
                AFTER(DRIVE_TIME, self, leave, self->direction);
            } else if (self->southQueue) {
                send(SOUTH_GREEN);
                self->southQueue--;
                self->direction = OPEN_SOUTH;
                self->onBridge++;
                AFTER(DRIVE_TIME, self, leave, self->direction);
            }
        }
    } else if (self->direction == OPEN_NORTH) {
        if (self->northQueue) {
            send(NORTH_GREEN);
            self->northQueue--;
            self->onBridge++;
            AFTER(DRIVE_TIME, self, leave, self->direction);
        } else if (self->southQueue) {
            self->direction = CLOSED_NORTH;
        }
    } else if (self->direction == OPEN_SOUTH) {
        if (self->southQueue) {
            send(SOUTH_GREEN);
            self->southQueue--;
            self->onBridge++;
            AFTER(DRIVE_TIME, self, leave, self->direction);
        } else if (self->northQueue) {
            self->direction = CLOSED_SOUTH;
        }
    }

    AFTER(ENTRY_DELAY, self, poll, 0);
}

int leave(State *const self, int arg) {
    assert(self->onBridge);

    self->onBridge -= 1;
    if (onBridge == 0)
        self->turn ^= 1;
}

int main() {
    init();

    while (1);
    return 0;
}
