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
    UBRR0H = (uint8_t)(UBRRVAL << 8);
    UBRR0L = (uint8_t)(UBRRVAL);

    // Receive and transmit.
    UCSR0B = SET(RXEN0)  | SET(TXEN0)  | SET(RXCIE0);
    // 8 bit char.
    UCSR0C = SET(UCSZ01) | SET(UCSZ00);
}

uint8_t recv(void) {
    while (!(UCSR0A & SET(RXC0)));
    return UDR0;
}

void send(uint8_t data) {
    while (!(UCSR0A & SET(UDRE0)));
    UDR0 = data;
}

int main() {
    init();

    while (1)
        send(recv());
}
