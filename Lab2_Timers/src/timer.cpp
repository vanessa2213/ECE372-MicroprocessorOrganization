// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//

#include "timer.h"

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){

    //set waveform generation bits to CTC mode
    //WGM02 = 1 WGM01 = 0  WGM00 = 0

    //WGM00 and WGM02 are in TCCR0A and TCCR0B
    TCCR0A &= ~(1<<WGM00);
    TCCR0A |= (1<<WGM01); //WGM01 is in TCCR0A
    TCCR0B &= ~(1<<WGM02);
    
    

}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-200 milliseconds
*/
void delayMs(unsigned int delay){

    //deppending on delay is M
    int unsigned count = 0; //used for the loop 
    
    //ps to 1024
    // count  = (Td * f)/ps -1= (1ms*16MHz)/1024 -1 = 15.625
    OCR0A = 16;

    TIFR0 |= (1<<OCF0A); //set the flag down with logic one

    TCNT0 = 0; //clears timer
    
    // turning on timer by settin ps to 1024 and start timer
    //CS02 = 1 CS01 = 0 CS00 = 1

    TCCR0B |= (1<<CS02)|(1<<CS00);
    
    TCCR0B &= ~(1<<CS01);
    
    
    while(count < delay) {
        if((TIFR0 & (1<<OCF0A))){
            count++;
            TIFR0 |= (1<<OCF0A); //set timer to start counting again
        }   
    }
    TCCR0B &= ~((1<<CS02)| (1<<CS01)|(1<<CS00)); //turn off clock
    

}
