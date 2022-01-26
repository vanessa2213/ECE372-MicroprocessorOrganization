/*AJ Sterner
November 29th 2021
Lab5

Description:  This program implements a Theremin using the ATMega microcontroller. It uses a pasive piezo buzzer as the output device, and 
              receives input from 2 infrared sensors. One of these sensors controls the pitch of the theremin, and the other controls the volume.
              The program also implements another output device in the form of an LED matrix controlled with SPI protocol that displays the current
              note on the musical scale that the buzzer is playing.

------------------------------------------------------------------------------------------------------------------------------------------------------
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<Arduino.h>

#include "adc.h"
#include "pwm.h"
#include "spi.h"
#include "switch.h"
#include "timer.h"

//Initialize global variables for ISR and SPI
typedef enum StateVarButtonEnum {wait_for_press, switch_pressed, wait_for_release, switch_released} StateVarButton; //for button
typedef enum StateVarSPIEnum {blank, A, B, C, D, E, F, G}StateVarSPI; //for spi

volatile StateVarButton button_state = wait_for_press;
volatile StateVarSPI spi_state = blank;

int main() {
  sei(); //initialize interrupts
  //Serial.begin(9600); //start serial transmission


  //Initialize all elements
  initSwitchPB7();
  initTimer0();
  initPWM4();
  initADC();

  //Initialize 8x8 LED matrix
  initSPIMaster();
  SPI_write_execute (0x0A, 0x08); //brightness control
  SPI_write_execute(0x0B, 0x07); //scan all rows/columns
  SPI_write_execute(0x0C, 0x01); //set shutdown register to normal operation
  SPI_write_execute(0x0F, 0x00); //set display test register to 0
  spi_state = blank;

  int onOrOff = 1; //on/off variable for buzzer. 0 = off, 1 = on
  
  unsigned int result1 = 0, sample1; //assign variables for ADC reading/converting
  unsigned int result2 = 0;
  float voltage1 = 0;
  float voltage2 = 0;
  unsigned int frequency = 0;
  float volume = 0;
  
  while(1) {
    switch(button_state) { //state machine for the on/off button
      case wait_for_press:
        break;
      case switch_pressed:
        delayMS();
        button_state = wait_for_release;
        break;
      case wait_for_release:
        break;
      case switch_released:
        delayMS();
        if(onOrOff == 0) {
          onOrOff = 1;
        } else {
          onOrOff = 0;
        }
        button_state = wait_for_press;
        break;
    }
    switch(spi_state) { //state machine for the LED matrix
      case blank: //turns all LEDs off
        SPI_write_execute(0x01, 0b00000000);
        SPI_write_execute(0x02, 0b00000000);
        SPI_write_execute(0x03, 0b00000000);
        SPI_write_execute(0x04, 0b00000000);
        SPI_write_execute(0x05, 0b00000000);
        SPI_write_execute(0x06, 0b00000000);
        SPI_write_execute(0x07, 0b00000000);
        SPI_write_execute(0x08, 0b00000000);
        break;
      case A:
        SPI_write_execute(0x03, 0b00111110);
        SPI_write_execute(0x04, 0b01001000);
        SPI_write_execute(0x05, 0b01001000);
        SPI_write_execute(0x06, 0b00111110);
        break;
      case B:
        SPI_write_execute(0x03, 0b00101100);
        SPI_write_execute(0x04, 0b01010010);
        SPI_write_execute(0x05, 0b01010010);
        SPI_write_execute(0x06, 0b01111110);
        break;
      case C:
        SPI_write_execute(0x03, 0b00100100);
        SPI_write_execute(0x04, 0b01000010);
        SPI_write_execute(0x05, 0b01000010);
        SPI_write_execute(0x06, 0b00111100);
        break;
      case D:
        SPI_write_execute(0x03, 0b00111100);
        SPI_write_execute(0x04, 0b01000010);
        SPI_write_execute(0x05, 0b01000010);
        SPI_write_execute(0x06, 0b01111110);
        break;
      case E:
        SPI_write_execute(0x03, 0b01000010);
        SPI_write_execute(0x04, 0b01010010);
        SPI_write_execute(0x05, 0b01010010);
        SPI_write_execute(0x06, 0b01111110);
        break;
      case F:
        SPI_write_execute(0x03, 0b01000000);
        SPI_write_execute(0x04, 0b01010000);
        SPI_write_execute(0x05, 0b01010000);
        SPI_write_execute(0x06, 0b01111110);
        break;
      case G:
        SPI_write_execute(0x03, 0b00100100);
        SPI_write_execute(0x04, 0b01001010);
        SPI_write_execute(0x05, 0b01000010);
        SPI_write_execute(0x06, 0b00111100);
        break;
    }
   
   
    //ADC Conversions
    for(int i = 0; i < 26; i++) { delayMS(); } //delay for ADC

    //Read in the first value and convert to voltage (the sensor outputs a value between 0 and 3 volts)
    int sampleNum = 10; //taking 10 samples and averaging them helps to cut out the noise from the sensors
    for(int i = 0; i < sampleNum; i++) {
      sample1 = ADCL;
      sample1 += ((unsigned int)ADCH) << 8;
      result1 += sample1;
      for(int j = 0; j < 13; j++) { delayMS(); }
    }
    

    voltage1 = (result1 / ((float)sampleNum)) * (5.0/1024.0);

    voltage1 = (((int)(voltage1 * 10)) / 10.0) - 0.1; //slightly modifies the voltage output to produce a more consistent result

    //frequency = 440*(pow(10, (voltage1Floor * 0.02)));

    //unsigned int frequencyFloor = ((unsigned int)(frequency / 200)) * 100;
    
    //if(frequency > 1760) {
    //  frequency = 1760;
    //} else if(frequency < 440) {
    //  frequency = 440;
    //}
    
    swapADC(); //swap ADC to other channel to read volume
    for(int i = 0; i < 26; i++) { delayMS(); }

    //read in volume values and convert to voltage
    result2 = ADCL;
    result2 += ((unsigned int)ADCH) << 8;

    voltage2 = (result2) * (5.0/1024.0);

    volume = ((1.0/3.0) * voltage2); //convert to a value we can use, range from 0 to 1

    float changedVol;
    if(volume <= 0) { //Sort the volumes into categories, since it sounds better on the buzzer
        changedVol = 0;
    } else if((volume > 0) & (volume <= 0.3)) {
        changedVol = 0.2;
    } else if((volume > 0.3) & (volume <= 0.6)) {
        changedVol = 0.5;
    } else {
        changedVol = 0.9;
    }

    swapADC(); //swap back to frequency channel 

    //print frequency/volume to console
    //Serial.print(voltage2);
    //Serial.print(", ");
    //Serial.println(changedVol);
    //Serial.flush();


    //Set the LED matrix output and frequency based on the voltage reading from the sensor
           if((voltage1 <= 0.5)) {
      spi_state = C;
      frequency = 523;
    } else if((voltage1 > 0.5) & (voltage1 <= 0.8)) {
      spi_state = D;
      frequency = 587;
    } else if((voltage1 > 0.8) & (voltage1 <= 1.1)) {
      spi_state = E;
      frequency = 659;
    } else if((voltage1 > 1.1) & (voltage1 <= 1.4)) {
      spi_state = F;
      frequency = 698;
    } else if((voltage1 > 1.4) & (voltage1 <= 1.7)) {
      spi_state = G;
      frequency = 784;
    } else if((voltage1 > 1.7) & (voltage1 <= 2.1)) {
      spi_state = A;
      frequency = 880;
    } else if((voltage1 > 2.1) & (voltage1 <= 2.5)) {
      spi_state = B;
      frequency = 988;
    } else if((voltage1 > 2.5)) {
      spi_state = C;
      frequency = 1047;
    }

    //output frequency/volume to the buzzer
    if(onOrOff == 1) {
      setBoth(frequency, changedVol);
    } else {
      disablePin(); //Setting frequency/duty cycle to 0 on the active buzzer still produced a faint noise, so we just disabled/re-enabled the pin
      spi_state = blank;
    }

    result1 = 0;
    //for(int i = 0; i < 200; i++) { delayMS(); }
  }
}
ISR(PCINT0_vect) { //handles the interrput routine for the switch
  if(button_state == wait_for_press) { //if the state is wait_for_press
    button_state = switch_pressed; //change to the switch_pressed state
  } else if(button_state == wait_for_release) { //if the state is wait_for_release
    button_state = switch_released; //switch to the switch_released state
  }
}