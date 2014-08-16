#include	"delays.h"

void udelay(void){
    unsigned long j;
    for (j = 0; j < 0x00001000; j++);
}

void delay(void){
    unsigned long j;
//    for (j = 0; j < 0x00000FF0; j++);
    for (j = 0; j < 0x000fffff; j++);	// for SLOW CLOCK
}

void delayShort(void){
    unsigned int j;
    for (j = 0; j < 0x00FFFF; j++);
}

void tick(unsigned int N){
    unsigned int j;
    for (j = 0; j < (N); j++);
}
