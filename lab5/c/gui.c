#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "Bridge.h"
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
       // system("clear");
        printf("S: %d: %s /=%d=\\ %s :%d :N\n",
        bridge->southCars, lightToStr(bridge->southGreen), bridge->onBridge, lightToStr(bridge->northGreen), bridge->northCars);
   // }
}


