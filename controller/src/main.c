#include "TinyTimber.h"
#include "interruptHandler.h"
#include "bridge.h"
#include "common.h"

int main() {
    init();

    Bridge b = initBridge();
    Writer w = initWriter();
    InterruptHandler h = initHandler(&b, &w);

    INSTALL(&h, recv, IRQ_USART0_RX);
    INSTALL(&h, drempty, IRQ_USART0_UDRE);

    return TINYTIMBER(&b, poll, 0);
}
