
#include <avr/io.h>
#include "led.h"

void initLEDs(){

    DDRB |= (1<<DDB7)|(1<<DDB6);     //SETS AS OUTPUT B7 AND B6 PIN 13 and 12
    PORTB |= (1<<PORTB6);           //TURN ON PORTB6 ONLY
}

void toogle_LEDs(){

    PORTB ^= (1<<PORTB6)|(1<<PORTB7);     //toogle LEDS in B7 and B6
}