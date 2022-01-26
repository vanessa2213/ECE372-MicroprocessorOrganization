#include "pwm.h"
#define arduino_clk_fq 16000000

void initPWMTimer3(){
    //use PWM
    //set datadirection for pins
    DDRE |= (1<<PORTE5); //output when using OCR3C for duty cycle mode 15 variable fq port 3

    // set non-inverting mode- output starts high and then is low
    TCCR3A |= (1 << COM3C1);
    TCCR3A &= ~(1 << COM3C0);
   
   //set WGM for PWM mode 15 variable top
    TCCR3B &= ~((1 << WGM32) | (1 << WGM33));
    TCCR3A |= ((1 << WGM31) | (1 << WGM30));
    

    //set prescaler to value of 1
    TCCR3B &= ~((1 << CS02) | (1 << CS01));
    TCCR3B |= (1 << CS00);

}
void change_frequency(unsigned int freq_val){
    if(freq_val == 0){
        OCR3C = 0;
    }
    else{
        OCR3C = ((arduino_clk_fq / freq_val) - 1)  * 0.25; //Sets output compare to a duty cycle of 0.25
    
    }
   
}