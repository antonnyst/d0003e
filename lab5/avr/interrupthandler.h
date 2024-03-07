#include "TinyTimber.h"

#ifndef INTERRUPT_H
#define INTERRUPT_H

#define initInterruptHandler(controller) { initObject(), controller }

typedef struct {
    Object super; 
    Object controller;
} InterruptHandler;

int handle_usart_rx(InterruptHandler *self);

#endif