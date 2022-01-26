#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

#define clkFreq   16000000  //clk fq 16MHz
#define desirefqz 16000

void initPWMTimer3();


void changeDutyCycle(float voltage);

#endif