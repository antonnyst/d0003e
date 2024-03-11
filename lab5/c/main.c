#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "threadObj.h"
#include "portReader.h"
#include "gui.h"
#include "Bridge.h"


void initStdin(){
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);
    attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}

char readStdin(){
    char buf[1] ;
    ssize_t input = read(STDIN_FILENO, buf, 1);
    return buf[0];
}



int main(){
    int fd = openPort("/dev/ttyUSB0");
    initStdin();
    Bridge br = {.northGreen = false, .southGreen = false, .southCars = 0, .northCars = 0, .onBridge = 0};
    pthread_t tid;
    threadObj portReaderT = {.tId = tid, .run = true, .Tfd = fd, .b = &br};
    pthread_create(&tid, NULL, contReadUpdate, (void *)&portReaderT);
    runGui(&br);
    while (true){
        char c = readStdin();
        
        if (strcmp(&c, "n") == 0){
            enqueueNorth(&br);
            writePort(fd, 0b0001);
        }

        if (strcmp(&c, "s") == 0){
            enqueueSouth(&br);
            writePort(fd, 0b0100);
        }

        if (strcmp(&c, "e") == 0){
            threadClose(&portReaderT);
            break;
        }
    }
}
