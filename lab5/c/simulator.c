#include <unistd.h>

#include "Bridge.h"
#include "threadObj.h"

void *runSimulator(void *vbridge){
    
    Bridge* bridge = (Bridge *)vbridge;
    usleep(5000000);
    exitBridge(bridge);
    pthread_exit(NULL);
}
