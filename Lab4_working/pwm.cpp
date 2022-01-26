#include <avr/io.h>
#include "pwm.h"



void initPWMTimer3(){

    DDRE |= (1<<PORTE3); //set PIN 5 to output  this is for PWM 
    DDRH |= (1<<PORTH3); //set PIN 6 to output = 2A to control motor 
    DDRH |= (1<<PORTH4); //set PIN 7 to output = 1A tocontrol motor

    //Timer 3
    //set non-inverting mode: output starts high and then is low
    TCCR3A |= (1<< COM3A1); //set output for OCR to be non-inverting
    TCCR3A &= ~(1<<COM3A0);

    //set WGM bits for PWM mode 7 fast 10 bit

    //Fast PWM for 10 bit top 
    //WGM33 = 0  WGM32 = 1 WGM31= 1 WGM30= 1
    //WGM33 and WGM32 are in TCCR3B
    //WGM31 and WGM30 are in TCCR3A

    TCCR3B &= ~(1<<WGM33);
    TCCR3B |= (1<<WGM32);
    TCCR3A |= (1<<WGM31)|(1<<WGM30);

    /// PWM frequency calculation for FAST PWM mode on page 148 of datasheet
    //TOP = 1023

    //set ps to 1
    //CS32 = 0  CS31 = 0 CS30=1
    //CS are in TCCR3B
    TCCR3B &= ~(1<<CS32);
    TCCR3B &= ~(1<<CS31);
    TCCR3B |=  (1<<CS30);

}



void changeDutyCycle(float voltage ){ 

    
     //top = 0x03FF =1023
     //OCR = duty cycle  * TOP
    
    if(voltage>=0 && voltage < 2.5){
        //0     clockwise and max rpm
        //clockwise:  1A = 0  2A =  1
        // 1A = H4 2A = H3
        PORTH &= ~(1<<PORTH4);
        PORTH |= (1<<PORTH3);
        
        OCR3A = 1023.0 *((100.0-voltage*100.0/2.5)/100.0);
        
    }
    
    //2.5   stop 
    else if(voltage == 2.5){
        //fast motor stop 
        //1A = 0 and 2A = 0
        PORTH &= ~(1<<PORTH4);
        PORTH &= ~(1<<PORTH3);
        
    }

    //counter clockwise 1A = H  2A = L
    //2.5-5 counter clockwise increasing rpm
    else if(voltage>2.5 && voltage <= 5){
        PORTH |= (1<<PORTH4);
        PORTH &= ~(1<<PORTH3);

        voltage = voltage - 2.5;
        
        OCR3A = 1023.0*(voltage*100.0/2.5)/100.0;
        
    
    } 
     
}