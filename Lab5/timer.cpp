#include "timer.h"

//Timer 0 will be used as debounce in ms
void initTimer1(){

    //set waveform generation bits to CTC mode
    //WGM13 = 0 WGM12 = 1 WGM11 = 0  WGM10 = 0

    //WGM13 and WGM12 is in TCCR1B
    TCCR1B &= ~(1<<WGM13);
    TCCR1B |= (1<<WGM12);
    //WGM10 and WGM11 are in TCCR0A 
    TCCR1A &= ~(1<<WGM11); 
    TCCR1A &= ~(1<<WGM10);
    
    
}

void delayMs(unsigned int delay){
    int unsigned count = 0; //used for the loop 
    
    //ps to 1024
    // count  = (Td * f)/ps -1= (1ms*16MHz)/1024 -1 = 15.625
    OCR1A = 16;

    TIFR1 |= (1<<OCF1A); //set the flag down with logic one

    TCNT1 = 0; //clears timer
    
    // turning on timer by settin ps to 1024 and start timer
    //CS02 = 1 CS01 = 0 CS00 = 1

    TCCR1B |= (1<<CS02)|(1<<CS00);
    
    TCCR1B &= ~(1<<CS01);
    
    
    while(count < delay) {
        if((TIFR1 & (1<<OCF1A))){
            count++;
            TIFR1 |= (1<<OCF1A); //set timer to start counting again
        }   
    }
    TCCR1B &= ~((1<<CS12)| (1<<CS11)|(1<<CS10)); //turn off clock
   
}