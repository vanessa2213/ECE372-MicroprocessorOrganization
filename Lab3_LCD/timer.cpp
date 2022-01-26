// Author: Perla Vanessa Jaime Gaytán 
// Net ID: vanejaime
// Date: 10/07
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
	//set waveform generation bits to CTC mode
    //WGM13 = 0, WGM12 = 1, WGM11 =  0, WGM10 = 0

    //WGM10 and WGM11 are in TCCR1A
    TCCR1A &= ~(1<<WGM10);
    TCCR1A &= ~(1<<WGM11);
    //WGM12 and WGM13 are in TCCR1B
    TCCR1B |= (1<<WGM12);
    TCCR1B &= ~(1<<WGM13);
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
    unsigned int count = 0;
    
    //count = (Td*f)/ps -1 = (1us*16M)/8 = 2
    OCR1A = 2;

    TIFR1 |= (1<<OCF1A); //set the flag down with logic one
    TCNT1= 0; //clears timer
    //ps = 8 
    //CS12 = 0  CS11 =1 CS10 = 0
    TCCR1B &= ~(1<<CS12);
    TCCR1B |= (1<<CS11);
    TCCR1B &= ~(1<<CS10);
    while(count < delay) {
        if((TIFR1 & (1<<OCF1A))){
            count++;
            TIFR1 |= (1<<OCF1A); //set timer to start counting again
        }   
    }
    TCCR1B &= ~((1<<CS12)| (1<<CS11)|(1<<CS10)); //turn off clock
   
}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
    //set waveform generation bits to CTC mode
    //WGM02 = 1 WGM01 = 0  WGM00 = 0

    //WGM00 and WGM01 are in TCCR0A 
    TCCR0A &= ~(1<<WGM00);
    TCCR0A |= (1<<WGM01); 
    TCCR0B &= ~(1<<WGM02);  //WGM02 is in TCCR0B

}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
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