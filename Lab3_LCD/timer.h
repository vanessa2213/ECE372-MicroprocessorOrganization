// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 3
//----------------------------------------------------------------------//

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void initTimer1();
void delayUs(unsigned int delay);
void initTimer0();
void delayMs(unsigned int delay); 

#endif
