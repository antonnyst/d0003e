#include <stdbool.h>
#include <pthread.h>
#include "simulator.h"
#include "gui.h"
#include "portReader.h"

pthread_mutex_t bLock;
pthread_t simId;

void exitBridge(Bridge *self){
    pthread_mutex_lock(&bLock);
    self->onBridge --;
    runGui(self);
    pthread_mutex_unlock(&bLock);
}

void dequeueNorth(struct Bridge *self){
    self->northCars --;
    self->onBridge ++;
    runGui(self);
    pthread_create(&simId, NULL, runSimulator, self);
}

void dequeueSouth(struct Bridge *self){
    self->southCars --;
    self->onBridge ++;
    runGui(self);
    pthread_create(&simId, NULL, runSimulator, self);
}

void southLightRed(struct Bridge *self){
    pthread_mutex_lock(&bLock);
    self->southGreen = false;
    runGui(self);
    pthread_mutex_unlock(&bLock);
}

void southLightGreen(struct Bridge *self){
    pthread_mutex_lock(&bLock);
    self->southGreen = true;
    if (self->southCars > 0){
    dequeueSouth(self);
    }
    runGui(self);
    pthread_mutex_unlock(&bLock);
}

void northLightRed(struct Bridge *self){
    pthread_mutex_lock(&bLock);
    self->northGreen = false;
    runGui(self);
    pthread_mutex_unlock(&bLock);
}

void northLightGreen(struct Bridge *self){
    pthread_mutex_lock(&bLock);
    self->northGreen = true;
    if (self->northCars > 0){
        dequeueNorth(self);
    }
    runGui(self);
    pthread_mutex_unlock(&bLock);
}

void enqueueNorth(struct Bridge *self){
    pthread_mutex_lock(&bLock);
    self->northCars ++;
    runGui(self);
    pthread_mutex_unlock(&bLock);
}

void enqueueSouth(struct Bridge *self){
    pthread_mutex_lock(&bLock);
    self->southCars ++;
    runGui(self);
    pthread_mutex_unlock(&bLock);
}

