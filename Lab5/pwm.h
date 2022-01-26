#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

void initPWMTimer3();
void change_frequency(unsigned int freq_val);

#endif