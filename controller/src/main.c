#include "TinyTimber.h"
#include "interruptHandler.h"
#include "bridge.h"
#include "init.h"

int main() {
    init();

    Bridge b = initBridge();
    InterruptHandler h = initHandler(&b);

    INSTALL(&h, handle, IRQ_USART0_RX);

    return TINYTIMBER(&b, poll, 0);
}
