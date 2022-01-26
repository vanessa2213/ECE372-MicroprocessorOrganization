// Author: Perla Vanessa Jaime Gaytán
// Net ID: vanejaime
// Date: 10/07/2021
// Assignment: Lab 3
//----------------------------------------------------------------------//
#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <Arduino.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){

  DDRA |= ((1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3));//declare as output pins 22-25
  DDRB |=( (1<<DDB4) | (1<<DDB6)); //enable and RS pin 10 and 12
  Serial.begin(9600);

}



/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
    
    //PORTA &=  (PORTA &0xF0);
    PORTA |= (PORTA &0xF0)|(data&0x0F); //bit mask data with PORTA
    PORTB &= ~(1<<PORTB6);    //Set RS pin to low
    PORTB |= (1<<PORTB4);     //Set Enable pin to high
    delayUs(1);               //delay 1us
    PORTB &= ~(1<<PORTB4);    //set enable pin to low
    delayUs(delay);           //delay
    PORTA &=  (PORTA &0xF0);
    

}



/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
   
    PORTB &= ~(1<<PORTB4);    //set enable pin to low
    PORTA &=  (PORTA &0xF0);
    PORTA |=   ((command&0xF0)>>4); //bit mask top 4 bits command with PORTA

    PORTB &= ~(1<<PORTB6);    //Set RS pin to low
    PORTB |= (1<<PORTB4);     //Set Enable pin to high
    
    delayUs(1);               //delay 1us

    PORTB &= ~(1<<PORTB4);    //set enable pin to low

    PORTA &=  (PORTA &0xF0);
    PORTA |= (command&0x0F); //bit mask bottom 4 bits command with PORTA
    
    PORTB |= (1<<PORTB4);     //Set Enable pin to high

    delayUs(1);               //delay 1us
    PORTB &= ~(1<<PORTB4);    //set enable pin to low
    //Serial.println(PORTA);
    delayUs(delay);
    PORTA &=  (PORTA &0xF0);
 
}



/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
     
     
    
    PORTA &=  (PORTA &0xF0);
    PORTA |=  ((character&0xF0)>>4); //bit mask top 4 bits character with PORTA
    //Serial.println(PORTA );
    PORTB |= (1<<PORTB6);    //Set RS pin to high
    PORTB |= (1<<PORTB4);     //Set Enable pin to high
    //Serial.println()
    delayUs(1);               //delay 1us

    PORTB &= ~(1<<PORTB4);    //set enable pin to low
    PORTA &=  (PORTA &0xF0);
    PORTA |= (character&0x0F); //bit mask bottom 4 bits command with PORTA
    //Serial.println(PORTA);
    
    PORTB |= (1<<PORTB4);     //Set Enable pin to high
    delayUs(1);               //delay 1us
    PORTB &= ~(1<<PORTB4);    //set enable pin to low
    delayUs(46);              //delay is always 46 MICROseconds for a character write
    //PORTA &=  (PORTA &0xF0);
}




/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){
  int i = 0;
  while(string[i] != '\0'){
    writeCharacter(string[i]);
    //Serial.println(string[i]);
    i++;
  }


}



/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
  //1x y    
  unsigned char command = 0x80; //this sets DB7 as required
  //Serial.println(x<<6);
  command |= (x<<6);            //this writes x to DB6 setting the line
  command |= (y);                 //sets the column
  Serial.println(command);
	eightBitCommandWithDelay(command,46);
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  
  delayMs(50);// Delay 15 milliseconds

  fourBitCommandWithDelay(0x03,4100);// Write 0b0011 to DB[7:4] delay 4100 microseconds
  
  fourBitCommandWithDelay(0x03,100);// Write 0b0011 to DB[7:4] anddelay 100 microseconds

  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  fourBitCommandWithDelay(0x03,100);// write 0b0011 to DB[7:4] and 100us delay

  fourBitCommandWithDelay(0x02,100);// write 0b0010 to DB[7:4] and 100us delay.

  eightBitCommandWithDelay(0x28,53);// Function set in the command table with 53us delay

  eightBitCommandWithDelay(0x08,53);// Display off in the command table with 53us delay
  eightBitCommandWithDelay(0x01,4000);// Clear display in the command table. Remember the delay is longer!!!

  eightBitCommandWithDelay(0x06,53);// Entry Mode Set in the command table.
  

  eightBitCommandWithDelay(0x0F,53);// Display ON/OFF Control in the command table. (Yes, this is not specified),
  
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
  
}



/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}