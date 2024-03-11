#include <termios.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "threadObj.h"
#include "simulator.h"



int openPort (char *tty){
     
    int fd = open("/dev/ttyUSB0", O_RDWR);
    
    if (fd == -1){
        printf("Error: failed to open port");
        return 1;
    }

    if(!isatty(fd)) {
        printf("Error: not a tty");
    }

    struct termios config;

    if(tcgetattr(fd, &config) < 0) {
        printf("Error: 1");
         //return 1;
    }

    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    config.c_oflag = 0;
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= CS8;
    config.c_cc[VMIN]  = 1;
    config.c_cc[VTIME] = 0;

    if(cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0) {
        printf("Error: 2");
        return 1;
    }

    if(tcsetattr(fd, TCSAFLUSH, &config) < 0) {
        printf("Error: cannot apply configuration");
        return 1;
    }
    return fd;
}

char readPort(int fd){
    char buff;
    
    int byte = read(fd, &buff, 1);
    
    return buff;
}

void writePort(int fd, int data){
    write(fd, &data, 1);
}

void *contReadUpdate(void *vself){
    struct threadObj *self = (struct threadObj *)vself;
    struct Bridge *bridge = self->b;
    int fd = self->Tfd;
    char r;


    while (self->run == true){
        r = readPort(fd);
        if (r & 0b0001){
            if (bridge->northCars > 0){
                writePort(fd, 0b0010);
            }
            northLightGreen(bridge);
            
        }

        if (r & 0b0010){
            northLightRed(bridge);
        }

        if (r & 0b0100){

            if (bridge->southCars > 0){
                writePort(fd, 0b1000);
            }
            southLightGreen(bridge);
            
        }

        if (r & 0b1000){
            southLightRed(bridge);
        }
    }
    return NULL;
}
