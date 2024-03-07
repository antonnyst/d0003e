#include "controller.h"
#include "serial.h"

#define IDLE_DELAY MSEC(500)
#define WAIT_DELAY MSEC(50)

#define STACK_CAR_AMT 10

#define CAR_DELAY SEC(1)
#define CAR_TRAVERSAL_TIME SEC(5)

int start_controller(Controller *self) {

}

int handle_recieve(Controller *self) {

    // Read data
    int data = SYNC(&(self->serial), serial_read, NULL);

    if (input & 0b0001) { // Bit 0: Northbound car arrival sensor activated
        self->north_amt++;
    }

    if (input & 0b0100) { // Bit 2: Southbound car arrival sensor activated
        self->south_amt++;
    }

    if (input & 0b0010) { // Bit 1: Northbound bridge entry sensor activated
        self->bridge_amt++;
        self->sent++;
        AFTER(CURRENT_OFFSET() + CAR_TRAVERSAL_TIME, self, car_passed, NULL);
    } 
    
    if (input & 0b1000) { // Bit 3: Southbound bridge entry sensor activated
        self->bridge_amt++;
        self->sent++;
        AFTER(CURRENT_OFFSET() + CAR_TRAVERSAL_TIME, self, car_passed, NULL);
    }

    return 0;
}

int car_passed(Controller *self) {
    self->bridge_amt--;
    return 0;
}

enum Direction invert_direction(enum Direction direction) {
    switch (direction) {
        case 0:
            return NONE;
        case 1:
            return SOUTH;
        case 2:
            return NORTH;
    }}

int get_cars(enum Direction direction) {
    switch (direction) {
        case 0:
            return 0;
        case 1:
            return self->north_amt;
        case 2:
            return self->south_amt;
    }
}


// State machine using events?
int idle(Controller *self);
int wait_empty(Controller *self);
int sending(Controller *self);
int send_car(Controller *self);


int idle(Controller *self) {
    // Switch based on which direction we sent cars last time
    switch (self->direction) {
        case 0:
        case 1:
            // NONE or NORTH
            if (self->south_amt > 0) {
                self->direction = SOUTH;
                ASYNC(self, sending, NULL);
                return 0;
            }
            if (self->north_amt > 0) {
                self->direction = NORTH;
                ASYNC(self, sending, NULL);
                return 0;
            }
            break;
        case 2:
            // SOUTH
            if (self->north_amt > 0) {
                self->direction = NORTH;
                ASYNC(self, sending, NULL);
                return 0;
            }
            if (self->south_amt > 0) {
                self->direction = SOUTH;
                ASYNC(self, sending, NULL);
                return 0;
            }
            break;
    }

    // If we reach here no cars were found so we keep chilling
    AFTER(CURRENT_OFFSET() + IDLE_DELAY, self, idle, NULL);
    return 0;
}

int wait_empty(Controller *self) {
    if (self->bridge_amt == 0) {
        ASYNC(self, idle, NULL);
    } else {
        AFTER(CURRENT_OFFSET() + WAIT_DELAY, self, wait_empty, NULL);
    }
    return 0;
}

// State where we send cars
int sending(Controller *self) {
    if (get_cars(self->direction) == 0) {   
        // No more cars to send
        ASYNC(self, wait_empty, NULL);
        return 0;
    }

    if (self->sent > STACK_CAR_AMT) { 
        // Check if there are cars waiting in the other direciton
        if (get_cars(invert_direction(self->direciton)) > 0) {
            ASYNC(self, wait_empty, NULL);
            return 0;
        }
    }
    
    // We should send an car
    ASYNC(self, send_car, NULL);
    return 0;
}

////////// Send car routine

int send_car(Controller *self) {

    // Send green light
    int data = 0;
    switch(self->direciton) {
        case 0:
            break;
        case 1:
            data = 0b0001;
            break;
        case 2:
            data = 0b0100;
            break;
    }

    SYNC(&(self->serial), serial_write, data);
    
    self->wait = self.sent;
    SYNC(self, wait_car_entry, NULL);
    return 0;
}

int wait_car_entry(Controller *self) {
    if (self->sent > self->wait) {
        // We have sent an car, red light and return to sending after waiting the car delay

        int data = 0;
        switch(self->direciton) {
            case 0:
                break;
            case 1:
                data = 0b0010;
                break;
            case 2:
                data = 0b1000;
                break;
        }

        ASYNC(&(self->serial), serial_write, data);
        
        AFTER(CURRENT_OFFSET() + CAR_DELAY, self, sending, NULL);
    } else {
        // Keep waiting...
        AFTER(CURRENT_OFFSET() + WAIT_DELAY, self, wait_car_entry, NULL);
    }
    return 0;
}

int get_north_amt(Controller *self) {
    return self->north_amt;
}

int get_bridge_amt(Controller *self) {
    return self->bridge_amt;
}

int get_south_amt(Controller *self) {
    return self->south_amt;
}
