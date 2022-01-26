/*
This file contains all the function requiered in main regarding the LEDs

*/

#include <avr/io.h>
#include "led.h"
#include <Arduino.h>

void initLEDs(){
    //initialized leds as outputs 

    DDRG |= (1<<DDG5);                                      //D04
    DDRE |= (1<<DDE3);                                      //D05
    DDRH |= (1<<DDH3) |(1<<DDH4) | (1<<DDH5) | (1<<DDH6);   //D06 D07 D08 D09
    DDRB |= (1<<DDB4)|(1<<DDB5);                            //D10 D11
    
    //TURN OFF LEDs
    PORTG &= (~(1<<PORTG5)); 
    PORTE &= (~(1<<PORTE3)); 
    PORTH &= (~(1<<PORTH3))|(~(1<<PORTH4))|(~(1<<PORTH5))|(~(1<<PORTH6)); 
    PORTB &= (~(1<<PORTB4))|(~(1<<PORTB5)); 

}

void turnOnLED(unsigned int led){
    //turn on the led received 
    switch (led)
    {
    case 4:
        PORTG |= 1<<PORTG5; //D04
        break;
    case 5:
        PORTE |= 1<<PORTE3; //D05
        break;
    case 6:
        PORTH |= 1<<PORTH3; //D06
        break;

    case 7:
        PORTH |= 1<<PORTH4; //D07
        break;

    case 8:
        PORTH |= 1<<PORTH5; //D08
        break;

    case 9:
        PORTH |= 1<<PORTH6; //D09
        break;

    case 10:
        PORTB |= 1<<PORTB4; //D10
        break;

    case 11:
        PORTB |= 1<<PORTB5; //D11
        break;
    
    default:
        //turn off al LEDs
        PORTG &= (~(1<<PORTG5)); 
        PORTE &= (~(1<<PORTE3)); 
        PORTH &= (~(1<<PORTH3))|(~(1<<PORTH4))|(~(1<<PORTH5))|(~(1<<PORTH6)); 
        PORTB &= (~(1<<PORTB4))|(~(1<<PORTB5));         
        break;
    }
}

void turnOffLED(unsigned int led){
    //turns off all the LEDs in its series except the one received
    switch (led)
    {

    case 4: 
        PORTE &= (~(1<<PORTE3));    //D05
        PORTH &= (~(1<<PORTH3));    //D06
        PORTH &= (~(1<<PORTH4));    //D07
        break;
    case 5:
        PORTG &= (~(1<<PORTG5));    //D04
        PORTH &= (~(1<<PORTH3));    //D06
        PORTH &= (~(1<<PORTH4));    //D07   
        break;
    case 6:
        PORTG &= (~(1<<PORTG5));    //D04
        PORTE &= (~(1<<PORTE3));    //D05
        PORTH &= (~(1<<PORTH4));    //D07    
        break;

    case 7:
        PORTG &= (~(1<<PORTG5));    //D04
        PORTE &= (~(1<<PORTE3));    //D05
        PORTH &= (~(1<<PORTH3));    //D06   
        break;

    case 8:
        PORTH &= (~(1<<PORTH6));    //D09
        PORTB &= (~(1<<PORTB4));    //D10
        PORTB &= (~(1<<PORTB5));    //D11      
        break;

    case 9:
        
        PORTH &= (~(1<<PORTH5));    //D08
        PORTB &= (~(1<<PORTB4));    //D10
        PORTB &= (~(1<<PORTB5));    //D11
        break;

    case 10:
       
        PORTH &= (~(1<<PORTH5));    //D08
        PORTH &= (~(1<<PORTH6));    //D09
        PORTB &= (~(1<<PORTB5));    //D11    
        break;

    case 11:
        
        PORTH &= (~(1<<PORTH5));    //D08
        PORTH &= (~(1<<PORTH6));    //D09
        PORTB &= (~(1<<PORTB4));    //D10    
        break;
    
    default:
        //turn off al LEDs
        PORTG &= (~(1<<PORTG5)); 
        PORTE &= (~(1<<PORTE3)); 
        PORTH &= (~(1<<PORTH3))|(~(1<<PORTH4))|(~(1<<PORTH5))|(~(1<<PORTH6)); 
        PORTB &= (~(1<<PORTB4))|(~(1<<PORTB5));         
        break;
    }
}

void runLED(unsigned int led_num){
    turnOffLED(led_num);        //turn off all LEDs except led_num
    turnOnLED(led_num);         //turn on led_num
    
}