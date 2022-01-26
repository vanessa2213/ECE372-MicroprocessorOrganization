#include "timer.h"
#include<avr/io.h>

void initTimer0() {
    //Set timer 1 to CTC mode by setting WGM13 - WGm10 to 010
    TCCR0B &= ~(1 << WGM02);
    TCCR0A |= (1 << WGM01);
    TCCR0A &= ~(1 << WGM00);
}

void delayMS() { //Sets the timer to count to 1ms, using a prescaler of 1024 and delay value 16

    OCR0A = 16; //Actually 15.6ish, but closest whole number ((1ms)/(1024 * 62.5MHz) - 1)

    TIFR0 |= (1 << OCF0A); //Set flag down

    TCNT0 = 0; //Set timer count to 0

    TCCR0B |= ((1 << CS02) | (1 << CS00)); //Set CS00-CS02 to 101 to set the prescaler to 1024
    TCCR0B &= ~(1 << CS01);

    while(!(TIFR0 & (1 << OCF0A))); //wait until flag is raised

    //TIFR0 |= (1 << OCF0A); //Set flag down again, just for good measure
    
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); //Turn clock off by setting CS00-CS02 to 000
}

