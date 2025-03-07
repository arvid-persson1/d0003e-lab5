#include <avr/io.h>
#include "init.h"

#define FOSC 8000000
#define BAUD 9600
#define UBRRVAL (FOSC / (16UL * BAUD) - 1)

// TODO: read status bits (FE, DOR, RXB8) before UDR reg
// CHR9 -> UCSZ2, OR -> DOR
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
