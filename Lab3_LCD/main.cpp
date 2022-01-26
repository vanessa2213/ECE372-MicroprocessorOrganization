// Author: Perla Vanessa Jaime Gaytán
// Net ID: vanejaime
// Date: 10/06/2021
// Assignment:     Lab 3
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//




#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"
#include "timer.h"
#include "lcd.h"

// defines
#define SHORT_DELAY 100
#define LONG_DELAY  200


/*
 * Define a set of states that can be used in the state machine using an enum.
 */
// typedef enum .......
typedef enum state_enum{
  wait_press,
  debounce_press,
  wait_release,
  debounce_release
}state;

// Initialize states.  Remember to use volatile 
volatile state button_state = wait_press; //initial state for button

volatile int flip = 0;  // use to handle the flipping of the delay between the lighting of the LEDs

volatile unsigned char bit = 0; //to count and move the LEDs



int main(){

  initTimer1();
  initTimer0();
  initSwitchPB3();
  initLED();
  initLCD();
  
  
  sei(); // Enable global interrupts.
  moveCursor(0, 0); // moves the cursor to 0,0 position
  
  writeString("Current count:");
  //writeCharacter('A');
  
  moveCursor(1, 0);  // moves the cursor to 1,0 position
  writeString("Slow");
  
  

// while loop
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
      
      turnOnLEDWithChar(bit);   //turn the LEDs according to the bit number
      
      delayMs(LONG_DELAY);      //use long delay

    }
    else{ //if it was pressed
      
      turnOnLEDWithChar(bit);   //turn the LEDs according to the bit number 
      
      delayMs(SHORT_DELAY);     //use short delay

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
      moveCursor(1, 0);  // moves the cursor to 1,0 position
      writeString("Fast");
      
    }
    else{ //if it was press before
      flip = 0; //come back to original value
      moveCursor(1, 0);  // moves the cursor to 1,0 position
      writeString("Slow"); 
      
    }
    button_state = debounce_release;  //change state to finish debouncing
  }
  
}