#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void initTimer0();
void delayMs(unsigned int delay);
void initTimer1();
void delayinSeconds(unsigned int delay);

#endif
