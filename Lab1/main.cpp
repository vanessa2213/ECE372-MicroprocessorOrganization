/*
Vanessa Jaime
Septemeber 09th, 2021.

Turns on 8 LEDs. 
I like to divided into SERIE A and SERIE B.
SERIE A is from pin 4 to pin 7 and the first LED to turn on is LED in pin 4.
SERIE B is from pin 8 to pin 11 and the first LED to turn on us LED in pin 11.

The sequence of turning on the LEDs is going in pairs. 
(4,11), (5,10), (6,9), (7,8), (6,9), (5,10), (4,11), (5,10) and so on

Whenever the push button is pressed, the delay change from short delay to long delay.


*/

#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"

#define LONG_DELAY 1000
#define SHORT_DELAY 100

int main()
{
  
  initLEDs();                 //initialization of leds as outputs
  initswitch();               //initilization of push button as input
  unsigned int led_num_a = 4; //variable to start turn in LED series A
  unsigned int led_num_b = 11; //variable to start turn in LED series B
  int led_num_a_up = 1;         //Series A of LEDs starts going up
  int led_num_b_up = 0;         //Series B of LEDs starts going down 
  

  while (1)
  {
    //
    if(!(PINA & (1<<PINA0))){ //check if switch is pressed pin 22
      runLED(led_num_a);      //turn on led_num_a and turn off the others in that serie
      runLED(led_num_b);      //turn on led_num_b and turn off the others in that serie
      _delay_ms(LONG_DELAY);  //long delay
      //turnOnLED(4);
      
    }
    else{
      runLED(led_num_a);    //turn on led_num_a and turn off the others in that serie
      runLED(led_num_b);    //turn on led_num_b and turn off the others in that serie
      _delay_ms(SHORT_DELAY); //short delay
    }
    //SERIES A is from LEDs in pin 4 to pin 7
    if(led_num_a_up )       //whenever the series a is going up
      if(led_num_a != 7)    //if it is not led in pin 7
        led_num_a++;        //going to turn on the next one up
      else{
        led_num_a--;        //if it is 7 is going to turn on the next one down
        led_num_a_up = 0;   //indicates that now it's going down
      }
        
    else                    //whenever is going down
      if(led_num_a != 4 )   //if the led is not 4 
        led_num_a--;        //turn on the next one down
      else{
        led_num_a++;        //if it is 4 going to turn on the next one up
        led_num_a_up = 1;   //indicate that now it's going up
      }

    //SERIES B is from LEDs in pin 8 to pin 11
    if(!led_num_b_up)       //whenever the series b is going down
      if(led_num_b != 8)    //if it is not led 8
        led_num_b--;        //turn on the next one down
      else{
        led_num_b++;        //if it is led 8 turn on the next one up
        led_num_b_up = 1;   //indicate that now it's going up
      }
    else                    //whenever the series b is going up
      if(led_num_b != 11)   //if it is not 11
        led_num_b++;        //turn on the next one up
      else{
        led_num_b--;        //if it is 11 turn on the next one down 
        led_num_b_up = 0;   //indicate that now it's going down
      }
  }

  return 0;
  
  

}