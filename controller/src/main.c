#include "TinyTimber.h"
#include "interruptHandler.h"
#include "bridge.h"
#include "common.h"

int main() {
    init();

    Writer w = initWriter();
    Bridge b = initBridge(&w);
    InterruptHandler h = initHandler(&w, &b);

    INSTALL(&h, recv, IRQ_USART0_RX);
    INSTALL(&h, drempty, IRQ_USART0_UDRE);

    return TINYTIMBER(&b, poll, 0);
}
