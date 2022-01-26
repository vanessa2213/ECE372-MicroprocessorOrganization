// Author:    Perla Vanessa Jaime Gaytan     
// Net ID:    vanejaime       
// Date:      09/28/2021      
// Assignment:     Lab 2
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"
#include "timer.h"

/*
 * Define a set of states that can be used in the state machine using an enum.
 */
#define SHORT_DELAY 100
#define LONG_DELAY  200

//Create states
typedef enum state_enum{
  wait_press,
  debounce_press,
  wait_release,
  debounce_release
}state;


volatile state button_state = wait_press; //initial state for button

volatile int flip = 0;  // use to handle the flipping of the delay between the lighting of the LEDs

volatile unsigned char bit = 0;

int main(){
  
  /*
  * Implement a state machine in the while loop which achieves the assignment
  * requirements.
  */
  //Serial.begin(9600);
  sei();              //enable interrupts
  initLED();          //initilize LEDS from pin 22-25 as outputs
  initSwitchPB3();    //initilize pin 50 as input for the push button
  initTimer0();       //initilize the timer 0 to use

	while (1) {
    //use to count from 0 to 15
    if(bit == 15){
      bit = 0;
    }
    else{
      bit++;
    }
    

    if(!flip) //if the button is not pressed
    {
      delayMs(LONG_DELAY);      //use long delay
      turnOnLEDWithChar(bit);   //turn the LEDs according to the bit number

    }
    else{ //if it was pressed
      delayMs(SHORT_DELAY);     //use short delay
      turnOnLEDWithChar(bit);   //turn the LEDs according to the bit number 

    }
    //state machine to debounce the button 
    switch (button_state)
    {
    case wait_press:
      //wating for any press of the button
      break;
    case debounce_press:
      delayMs(1);
      button_state = wait_release;
      break;
    case wait_release:
      //waiting the user realese the button
      break;
    case debounce_release:
      delayMs(1);
      button_state = wait_press;
      break;
    
    default:
      button_state = wait_press;
      break;
    }
    //Serial.println(button_state);
  

	}
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
ISR(PCINT0_vect){
  if(button_state == wait_press){ //whenever detects the first press in the button
    button_state = debounce_press;  //change state to button press
  }
  else if(button_state == wait_release){  //when the debounce is done 
    if(!flip){  //if it has not been press before 
      flip = 1; //change to 1
    }
    else{ //if it was press before
      flip = 0; //come back to original value
    }
    button_state = debounce_release;  //change state to finish debouncing
  }
  
}

