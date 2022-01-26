#include<avr/io.h>
#include "switch.h"

void initSwitchPB7() {
    DDRB &= ~(1 << DDB7); //Sets B7 as an input (pin 13)
    PORTB |= (1 << PORTB7); //Turns on the pullup resistor for B7

    PCICR |= (1 << PCIE0); //Turn on pin interrupts for portB pins
    PCMSK0 |= (1 << PCINT7); //Turn on the PCINT that corresponds to B7
}