#include <stdbool.h>
#include <pthread.h>
#ifndef Bridge_h

#define Bridge_h

typedef struct Bridge {
    bool southGreen;
    bool northGreen;
    int southCars;
    int northCars;
    int onBridge;
} Bridge;

struct argStruct{
    Bridge *self;
    pthread_mutex_t *mutex;
};


void exitBridge(Bridge *self);

void southLightRed(Bridge *self);

void southLightGreen(Bridge *self);

void northLightRed(Bridge *self);

void northLightGreen(Bridge *self);

void enqueueNorth(Bridge *self);

void enqueueSouth(Bridge *self);

#endif
