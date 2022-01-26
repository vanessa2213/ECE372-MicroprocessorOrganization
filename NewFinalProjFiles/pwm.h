#ifndef PWM_H
#define PWM_H

void initPWM4(); //initializes PWM using timer 4
void setFrequency(unsigned int frequency); //sets the frequency to an input value and duty cycle to 0.5
void setBoth(unsigned int frequency, float volume); //sets both frequency and duty cycle to input values
void disablePin();

#endif