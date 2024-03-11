#include "TinyTimber.h"
#include "controller.h"

#ifndef INTERRUPT_H
#define INTERRUPT_H

#define initInterruptHandler(controller) { initObject(), controller }

typedef struct {
    Object super; 
    Controller *controller;
} InterruptHandler;

int handle_usart_rx(InterruptHandler *self);

#endif