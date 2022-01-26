#include <Arduino.h>
#include "PWM.h"

void initPWM4(){ //uses timer 4

    //Set to fast PWM mode 15
    TCCR4A |= (1 << WGM40) | (1 << WGM41);
    TCCR4B |= (1 << WGM42) | (1 << WGM43); 

    // Set non-inverting mode
    TCCR4A &= ~(1 << COM4C0);
    TCCR4A |= (1 << COM4C1);

    //Set prescaler to 8
    TCCR4B &= ~((1 << CS42) | (1 << CS40));
    TCCR4B |= (1 << CS41);

    DDRH |= (1 << DDH5); //Set pin 8, H5, to output mode (OC4C)
}

void setFrequency(unsigned int frequency){
    OCR4A = 16000000 / (8 * frequency);
    OCR4AH = OCR4A >> 8;
    OCR4AL = OCR4A;

    OCR4C = OCR4A * 0.1;
    OCR4CH = OCR4C >> 8;
    OCR4CL = OCR4C;
}

void setBoth(unsigned int frequency, float volume) {
    DDRH |= (1 << DDH5);

    OCR4A = 16000000 / (8 * frequency);
    OCR4AH = OCR4A >> 8;
    OCR4AL = OCR4A;

    OCR4C = OCR4A * volume;
    OCR4CH = OCR4C >> 8;
    OCR4CL = OCR4C;
}
void disablePin() { //disables the output pin
    DDRH &= ~(1 << DDH5);
}
