#ifndef _INIT_H
#define _INIT_H

#include <avr/io.h>

#if defined(__OPTIMIZE__) && __OPTIMIZE__ > 2
    #define assert(x) { if (!(x)) __builtin_unreachable(); }
#else
    #define assert(x) { if (!(x)) while (1); }
#endif

#ifndef UDR
#define UDR   UDR0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRB UCSR0B
#define RXEN  RXEN0
#define TXEN  TXEN0
#define RXCIE RXCIE0
#define UCSRC UCSR0C
#define UCXZ1 UCXZ01
#define UCSZ0 UCSZ00
#endif

#define SET(x) (1 << x)

void init(void);

#endif
