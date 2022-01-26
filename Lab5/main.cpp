#include <avr/io.h>
#include <Arduino.h>

#include "i2c.h"
#include "timer.h"
#include "pwm.h"
#include "switch.h"
#include "spi.h"


//addresses for I2C
#define SLA       0x68 //MPU_6050 address with PIN AD0 grounded
#define PWR_MGMT  0x6B
#define WAKEUP    0x00

#define SL_MEMA_XAX_HIGH 0x3B
#define SL_MEMA_XAX_LOW  0x3C

#define SL_MEMA_YAX_HIGH 0x3D
#define SL_MEMA_YAX_LOW  0x3E

#define SL_MEMA_ZAX_HIGH 0x3F
#define SL_MEMA_ZAX_LOW  0x40

#define SL_TEMP_HIGH     0x41
#define SL_TEMP_LOW      0x42

//state for buttons
typedef enum state_enum{
  wait_press,
  debounce_press,
  wait_release,
  debounce_release
}state_button;

//state for SPI
typedef enum state_enum_spi{
  blank,
  smiley,
  frowny
}state_spi;

// Initialize states.  Remember to use volatile 
volatile state_button button_state = wait_press; //initial state for button
volatile state_spi spi_state = blank;

void change_face();

int main(){

  //signed int T_val = 0;         //Temperature read from chip
  //float      T_C   = 0;         //Temperature in Celcius
  signed int X_val = 0;
  signed int Y_val = 0;
  signed int Z_val = 0;

  signed int Y_val_init = 0;
  signed int Z_val_init = 0;

  signed int threshhold_val_y_min = 0;    
  signed int threshhold_val_y_max = 0;    
  signed int threshhold_val_z_min = 0;  
  signed int threshhold_val_z_max = 0;     


  
  initTimer1();
  initPWMTimer3();
  initSwitchPB7();
  


  Serial.begin(9600);
  sei();
  
  //SPI initilization
  initSPIMaster();
  SPI_write_execute (0x0A, 0x08); //brightness control
  SPI_write_execute(0x0B, 0x07); //scan all rows/columns
  SPI_write_execute(0x0C, 0x01); //set shutdown register to normal operation
  SPI_write_execute(0x0F, 0x00); //set display test register to 0
  spi_state = smiley;

  //I2C initilization
  initI2C();
  StartI2C_Trans(SLA);
  write(PWR_MGMT);  //address on SLA for Power Management
  write(WAKEUP);    //send data to wakeup from sleep mode
  StopI2C_Trans();

  //reading initial values
  //delayMs(1000);
  
    delayMs(1);
    Read_from(SLA,SL_MEMA_YAX_HIGH);
    Y_val_init = Read_data();              //read upper value
    Read_from(SLA,SL_MEMA_YAX_LOW);
    Y_val_init = (Y_val_init<<8)|Read_data();    //read lower value
    
    Read_from(SLA,SL_MEMA_ZAX_HIGH);
    Z_val_init = Read_data();              //read upper value
    Read_from(SLA,SL_MEMA_ZAX_LOW);
    Z_val_init = (Z_val_init<<8)|Read_data();    //read lower value    
    
    StopI2C_Trans();
    

  
  threshhold_val_y_max = Y_val_init+6000 ;
  threshhold_val_y_min = Y_val_init-6000;
  threshhold_val_z_max = Z_val_init+6000;
  threshhold_val_z_min = Z_val_init-6000;
  
  
  //spi_state = frowny;
  while (1)
  {
    // ******I2C******

    //Reading temperature
    
    delayMs(1000);

    /*
    //READING TEMPERATURE

    Read_from(SLA,SL_TEMP_HIGH);

    T_val = Read_data();              //read upper value

    Read_from(SLA,SL_TEMP_LOW);
    T_val = (T_val<<8)|Read_data();    //read lower value

    //Temperature in degrees C = (TEMP_OUT signed register)/340 + 36.53
    T_C = (float(T_val)/340)+36.53;

    Serial.print("Temperature °C = ");
    Serial.println(T_C);
    */
    
    
    //Reading x data
    
    Read_from(SLA,SL_MEMA_XAX_HIGH);

    X_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_XAX_LOW);
    X_val = (X_val<<8)|Read_data();    //read lower value

    Serial.print("X = ");
    Serial.println(X_val);

    //Reading y data
    
    Read_from(SLA,SL_MEMA_YAX_HIGH);

    Y_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_YAX_LOW);
    Y_val = (Y_val<<8)|Read_data();    //read lower value
    Serial.print("Yinit = ");
    Serial.println(Y_val_init);
    Serial.print("Y threshold max = ");
    Serial.println(threshhold_val_y_max);
    Serial.print("Y threshold min = ");
    Serial.println(threshhold_val_y_min);
    Serial.print("Y = ");
    Serial.println(Y_val);

    //Reading z data
    
    Read_from(SLA,SL_MEMA_ZAX_HIGH);

    Z_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_ZAX_LOW);
    Z_val = (Z_val<<8)|Read_data();    //read lower value

    Serial.print("Zinit = ");
    Serial.println(Z_val_init);
    
    Serial.print("Z max threshold = ");
    Serial.println(threshhold_val_z_max);
    Serial.print("Z min threshold = ");
    Serial.println(threshhold_val_z_min);
    Serial.print("Z = ");
    Serial.println(Z_val);
    
    
    StopI2C_Trans();
    
    
    //

    //testing pwm buzzer
    /*
    for(unsigned int i = 500; i<10000; i++){
      change_frequency(i);
    }
    */
   
    /*if(Y_val > threshhold_val_y) { //Replace argument of if statement once actual values are determined
      //e.g y_accel_val + z_accel_val > threshhold_val
      spi_state = frowny;
    }
    change_face();
    
*/
   switch (button_state){
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
        if(spi_state == frowny) {
          spi_state = smiley;
        }
        change_frequency(0);
        button_state = wait_press;
        break;
      
      default:
        button_state = wait_press;
        break;
    }
    //Serial.println(button_state);
    
    change_face();
    if(spi_state == frowny){
      
      for(unsigned int i = 500; i<10000; i++){
        change_frequency(i);
      }
      if(Y_val < threshhold_val_y_max && Y_val>threshhold_val_y_min && Z_val<threshhold_val_z_max && Z_val>threshhold_val_z_min ){
        spi_state = smiley;
      }
    
    }
    else{
      
      if(Y_val>threshhold_val_y_max || Y_val<threshhold_val_y_min|| (Z_val>threshhold_val_z_max) || (Z_val<threshhold_val_z_min)){
        spi_state = frowny;
      }
    }

  }
  

  Serial.flush();
  return 0;

}
void change_face(){
  switch(spi_state) { //state machine to control the SPI protocol
      case blank: //turns all LEDs off
        SPI_write_execute(0x01, 0x00);
        SPI_write_execute(0x02, 0x00);
        SPI_write_execute(0x03, 0x00);
        SPI_write_execute(0x04, 0x00);
        SPI_write_execute(0x05, 0x00);
        SPI_write_execute(0x06, 0x00);
        SPI_write_execute(0x07, 0x00);
        SPI_write_execute(0x08, 0x00);
       // change_frequency(0); //sets the buzzer to off
        break;
      case smiley: //draws a smiley face
        SPI_write_execute(0x01, 0x00);
        SPI_write_execute(0x02, 0b01000010);
        SPI_write_execute(0x03, 0x00);
        SPI_write_execute(0x04, 0x00);
        SPI_write_execute(0x05, 0b10000001);
        SPI_write_execute(0x06, 0b01000010);
        SPI_write_execute(0x07, 0b00111100);
        SPI_write_execute(0x08, 0x00);
        //change_frequency(0); //sets the buzzer to off
        break;
      case frowny: //draws a frowny face
        SPI_write_execute(0x01, 0x00);
        SPI_write_execute(0x02, 0b01000010);
        SPI_write_execute(0x03, 0x00);
        SPI_write_execute(0x04, 0x00);
        SPI_write_execute(0x05, 0b00111100);
        SPI_write_execute(0x06, 0b01000010);
        SPI_write_execute(0x07, 0b10000001);
        SPI_write_execute(0x08, 0x00);
        
        break;
      default:
        spi_state = blank;
        break;
      
    }
    
}

ISR(PCINT0_vect){
  if(button_state == wait_press){ //whenever detects the first press in the button
    button_state = debounce_press;  //change state to button press
    
  }
  else if(button_state == wait_release){  //when the debounce is done 
    button_state = debounce_release;  //change state to finish debouncing
  }
  
}