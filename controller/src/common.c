#include "common.h"

#define FOSC 8000000UL
#define BAUD 9600UL
#define UBRRVAL (FOSC / (16UL * BAUD) - 1UL)

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
    UBRRH = (uint8_t)(UBRRVAL << 8);
    UBRRL = (uint8_t)(UBRRVAL);

    // Receive and transmit.
    UCSRB = SET(RXEN)  | SET(TXEN)  | SET(RXCIE);
    // 8 bit char.
    UCSRC = SET(UCSZ1) | SET(UCSZ0);
}
