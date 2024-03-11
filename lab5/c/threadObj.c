#include <stdbool.h>
#include <pthread.h>
#include "Bridge.h"
typedef struct threadObj {
    pthread_t tId;
    bool run;
    int Tfd;
    Bridge *b;
} threadObj;



void threadClose(struct threadObj *self){
    self->run = false;
}

