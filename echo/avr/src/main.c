#include <stdint.h>
#include <avr/io.h>

#define assert(x) { if (!x) while (1); }

#define FOSC 8000000
#define BAUD 9600
#define UBRRVAL (FOSC / (16UL * BAUD) - 1)
#define SET(x) (1 << x)

void init(void) {
    // UBRR calculation from p.154 in ATmega169 manual.
    // XXX: writing high byte is unnecessary with current config.
    UBRRH = (uint8_t)(UBRRVAL << 8);
    UBRRL = (uint8_t)(UBRRVAL);

    // Receive and transmit.
    UCSRB = SET(RXEN)  | SET(TXEN);
    // 8 bit char.
    UCSRC = SET(UCSZ1) | SET(UCSZ0);
}

uint8_t recv(void) {
    while (!(UCSRA & SET(RXC)));
    return UDR;
}

void send(uint8_t data) {
    while (!(UCSRA & SET(UDRE)));
    UDR = data;
}

int main() {
    init();

    while (1)
        send(recv());
}
