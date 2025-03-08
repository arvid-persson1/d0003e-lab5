#include "interruptHandler.h"
#include "common.h"

int recv(__attribute__((unused)) const InterruptHandler *const self, __attribute__((unused)) const int _x) {
    ASYNC(self->bridge, process, (int)UDR);
    return 0;
}

int drempty(__attribute__((unused)) const InterruptHandler *const self, __attribute__((unused)) const int _x) {
    ASYNC(self->writer, ready, 0);
    return 0;
}
