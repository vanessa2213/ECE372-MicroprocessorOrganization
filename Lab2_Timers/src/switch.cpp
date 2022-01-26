// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){

    DDRB &= ~(1<<DDB3);     //initialized as input
    PORTB |= (1<<PORTB3);   //enable pull-up resistor pin 50
    PCICR |= (1<<PCIE0);    //enable pin change interrupt for 0-7
    PCMSK0 |= (1<<PCINT3);  //enable the PIN CHANGE INTERRUPT SPECIFIC MASK REGISTER 
    
}
