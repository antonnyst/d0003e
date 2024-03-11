#include <pthread.h>
#include <stdbool.h>
#include "Bridge.h"

typedef struct threadObj {
    pthread_t tId;
    bool run;
    int Tfd;
    Bridge *b;
} threadObj;

void threadClose(struct threadObj *self);

