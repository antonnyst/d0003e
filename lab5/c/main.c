#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct Bridge {
    bool southGreen;
    bool northGreen;
    int southCars;
    int northCars;
    int onBridge;
} Bridge;


void southLightRed(struct Bridge *self){
    self->southGreen = false;
}

void southLightGreen(struct Bridge *self){
    self->southGreen = true;
}

void nothLightRed(struct Bridge *self){
    self->northGreen = false;
}

void northLightGreen(struct Bridge *self){
    self->northGreen = true;
}

void enqueueNorth(struct Bridge *self){
    self->northCars ++;
}

void enqueueSouth(struct Bridge *self){
    self->southCars ++;
}

void dequeueNorth(struct Bridge *self){
    self->northCars --;
}

void dequeueSouth(struct Bridge *self){
    self->southCars --;
}

char *lightToStr(bool light){
    
    if (light == true){
        return "G"; 
    }

    else {
        return "R";
    }
}

void runGui (struct Bridge *bridge){
   // while (true){
        system("clear");
        printf("S: %d: %s /=%d=\\ %s :%d :N\n",
        bridge->southCars, lightToStr(bridge->southGreen), bridge->onBridge, lightToStr(bridge->northGreen), bridge->northCars);
   // }
}

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

void* userInputThread(){
    while(true){
        c = getchar()
    }
}


int main(){
/*
    int fd = openPort("/dev/ttyUSB0");

    int bob;
    write(fd, "k", 1);
    tcdrain(fd);
    char buffer[100];
    bob = read(fd, buffer, 1);
    printf("%d bytes read", bob);
    buffer[bob] = '\0';
    printf("Those bytes are as follows: % s\n", buffer);
*/
    Bridge b = {.northGreen = true, .southGreen = false, .southCars = 0, .northCars = 1, .onBridge = 0};
    runGui(&b);
}
