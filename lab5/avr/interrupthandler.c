#include "TinyTimber.h"
#include "interrupthandler.h"
#include "controller.h"

int handle_usart_rx(InterruptHandler *self) {
    ASYNC(&(self->controller), handle_recieve, NULL);
    return 0;
}
