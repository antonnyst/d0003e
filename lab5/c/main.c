#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

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



int openPort (char *tty){
    
    int fd = open(tty, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (fd == -1){
        printf("Error: failed to open port");
        return 1;
    }

    if(!isatty(fd)) {
        printf("Error: not a tty");
    }

    struct termios config;

    if(tcgetattr(fd, &config) < 0) {
        printf("Error:");
        return 1;
    }

    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    config.c_oflag = 0;
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= CS8;
    config.c_cc[VMIN]  = 1;
    config.c_cc[VTIME] = 0;

    if(cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0) {
        printf("Error:");
        return 1;
    }

    if(tcsetattr(fd, TCSAFLUSH, &config) < 0) {
        printf("Error: cannot apply configuration");
        return 1;
    }
    return fd;
}

int main(){

    int fd = openPort("/dev/tty10");

    write(fd, "k", 1);

}
