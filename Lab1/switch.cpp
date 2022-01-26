#include <avr/io.h>
#include "switch.h"

void initswitch(){
     
    DDRA &= ~(1<<DDA0);     //initialized as output pin 22
    PORTA |= (1<<PORTA0);   //enable pull-up resistor

}