#include "interruptHandler.h"

int handle(__attribute__((unused)) const InterruptHandler *const self, __attribute__((unused)) const int _x) {
    ASYNC(self->bridge, process, (int)UDR0);
    return 0;
}
