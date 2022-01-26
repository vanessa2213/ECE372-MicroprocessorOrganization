#include <Arduino.h>
#include <avr/io.h>

#include <led.h>
#include <pwm.h>
#include <timer.h>
#include <adc.h>
#include <segments.h>
#include <switch.h>

typedef enum state_enum{
  wait_press,
  debounce_press,
  wait_release,
  debounce_release
}state;

// Initialize states.  Remember to use volatile 
volatile state button_state = wait_press; //initial state for button


int main(){

  initTimer0();
  initTimer1();
  //initLEDs();     //use to assure that timers 0 and 1 are working correctly
  initPWMTimer3();  //pin 5
  
  initADC0();
  init7Segments();
  initSwitchPD0();
  sei(); // Enable global interrupts.

  Serial.begin(9600);
  /*
  test ADC*/
  unsigned int result = 0;
  float voltage = 0;
  
  //int count =10;
  turnOff7segments();
  
  //shiftData(0x02);
  //shiftData(0x03);
  
  while(1){
   // turnOn7segments(9);
    /*
    //use to assure timers were working correctly
    toogle_LEDs();
    delayMs(5);
    
    toogle_LEDs();
    delayinSeconds(3);
    */
    //use to assure pwm is working
    /*
    for(unsigned int i = 500; i<10000; i++){
      changeDutyCycle(i);
      

    }
    */

   //to test 7segment 
    /*if(count != 0){
      count--;
      turnOn7segments(count);
      delayinSeconds(1);
    }
    else{
      count = 10;
    }
    */
    
    //to test ADC
    
    // /*
    result = ADCL;
    result += ((unsigned int) ADCH) << 8;
    voltage = result * (5.0/1024.0);
    Serial.println(voltage,2);
    Serial.flush();
    changeDutyCycle(voltage);
    // */
    
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
      
      changeDutyCycle(2.5);
    
      EIMSK &= ~(1<<INT0); //disable switch interrupt

      for(int i = 9; i>=0; i--){
        turnOn7segments(i);
        delayinSeconds(1);
      }
      turnOff7segments();
      EIMSK |= (1<<INT0); //re enables switch interrupt
      
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
ISR(INT0_vect){
  if(button_state == wait_press){ //whenever detects the first press in the button
    button_state = debounce_press;  //change state to button press
    
  }
  else if(button_state == wait_release){  //when the debounce is done 
    button_state = debounce_release;  //change state to finish debouncing
  }
  
}

