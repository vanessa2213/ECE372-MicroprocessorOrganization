#include "switch.h"
#include <avr/io.h>


void initSwitchPB7(){

    DDRB &= ~(1<<DDB7);     //initialized as input
    PORTB |= (1<<PORTB7);   //enable pull-up resistor pin 13
    PCICR |= (1<<PCIE0);    //enable pin change interrupt for 0-7
    PCMSK0 |= (1<<PCINT7);  //enable the PIN CHANGE INTERRUPT SPECIFIC MASK REGISTER 

}