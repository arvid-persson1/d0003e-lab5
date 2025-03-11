#include "writer.h"
#include "common.h"

int send(__attribute__((unused)) const Writer *const self, const int arg) {
    uint8_t data = (uint8_t)arg;

    while (!(UCSRA & SET(UDRE)));
    UDR = data;

    return 0;
}
