#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPD0(){

    DDRD &= ~(1<<DDD0);     //initialized as input
    PORTD |= (1<<PORTD0);   //enable pull-up resistor pin 21
    //external interrupt:
    //the falling edge of INT0 generates asynchronous interrup
    //ISC01 = 1 ISC00 = 0
    EICRA |= (1<<ISC01);
    EICRA &= ~(1<<ISC00);

    EIMSK |= (1<<INT0);  //enable the external interrupt mask to int0

}