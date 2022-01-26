#include "adc.h"
#include <avr/io.h>

volatile unsigned int currADC = 1;

void initADC(){
    /*
    using pin A0 (PF0)
    */

    // set reference voltage set to AVCC = 5V output.  (we really want to measure 5V output from the 
    // development board to determine what the reference value is for precise calculations)
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    // determine left or right justified (ADLAR = 0 RIGHT JUSTIFIED)
    ADMUX &= ~(1 << ADLAR);

    // Specify ADC input channel and mode
    //Set ADC0 as single-ended input with MUX[5:0] = 0b000000
    ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    ADMUX &= ~((1 << MUX4) | (1 << MUX3));
    ADCSRB &= ~(1 << MUX5);

    // set Auto Trigger Source Selection
    // Use free-running mode ADTS[2:0] = 0b000
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

    // enable ADC and enable auto-triggering
    ADCSRA |= (1 << ADEN) | (1 << ADATE);


    // set the ADC clock frequency.  Use a pre-scaler of 32
    // ADC clock frequency is 16 Mhz divided by pre-scaler ~=100 kHz
    // Sampling rate is 1/ ((1/125K Hz )*(13 clock cycles)) =  38.461 kHz
    // ADPS2 = 1, ADPS1 = 1, ADPS0 = 1
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
    ADCSRA &= ~(1 << ADPS2);

    // //set up timer 1 
    // TCCR1B &= ~(1<<WGM13);
    // TCCR1B |= (1<<WGM12);
    // TCCR1A &= ~(1<<WGM11);
    // TCCR1A &= ~(1<<WGM10);

    // OCR1B = 65535; //set to max value for greatest delay

    // TCNT1= 0; //clears timer counter value

    // TCCR1B |= (1<<CS12);
    // TCCR1B &= ~(1<<CS11);
    // TCCR1B |= (1<<CS10);

    // disable ADC0 pin digital input - pin A0 on board
    DIDR0 |= (1 << ADC0D);

    // start the first ADC conversion
    ADCSRA |= (1 << ADSC);
}

void swapADC() {
    //unsigned int currADC; //1 = pin A0, 2 = pin A1

    ADCSRA &= ~((1 << ADEN) | (1 << ADATE)); //disable current ADC

    if (currADC == 1) { //switch to 2
        //Specify ADC input channel and mode
        //Set ADC1 as single-ended input with MUX[5:0] = 0b000001
        ADMUX &= ~((1 << MUX2) | (1 << MUX1));
        ADMUX &= ~((1 << MUX4) | (1 << MUX3));
        ADCSRB &= ~(1 << MUX5);
        ADMUX |= (1 << MUX0);

        DIDR0 |= (1 << ADC1D); //disable digital input for ADC0

        currADC = 2;
    }
    else { //switch to 1
        // Specify ADC input channel and mode
        //Set ADC0 as single-ended input with MUX[5:0] = 0b000000
        ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
        ADMUX &= ~((1 << MUX4) | (1 << MUX3));
        ADCSRB &= ~(1 << MUX5);

        DIDR0 |= (1 << ADC0D);

        currADC = 1;
    }

    ADCSRA |= (1 << ADEN) | (1 << ADATE); //enable ADC and auto triggering

    ADCSRA |= (1 << ADSC); //start conversion
}