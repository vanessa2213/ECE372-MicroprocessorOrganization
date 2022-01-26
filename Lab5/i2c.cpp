#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"

#define wait_for_completion while(!(TWCR&(1<<TWINT)));

void initI2C(){

    //Wake up I2C module on mega 2560
    PRR0 &= ~(1<<PRTWI);
    TWSR |= (1<<TWPS0);

    //Set prescaler TWPS to 1
    TWSR &= ~(1<<TWPS1);

    //Set two wire interface bit rate register TWBR
    //TWBR = (CPU_clk_fq/SCL_fq - 16)/(2 * 4^TWPS)
    TWBR = 0xC6; //198

    //Enable two wire interface
    TWCR |= (1<<TWINT)|(1<<TWEN); 
    
}

void StartI2C_Trans(unsigned char SLA){
    /*
        This function initiates a star condition and 
        calls slave device with SLA
    */
    //Clear TWINT, initiate start condition, initiate enable
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //put flag down, initiate a start condition and enable

    //Wait for completion
    wait_for_completion;

    //Set two wire data register to the SLA + write bit
    TWDR = (SLA<<1);

    //Trigger action: Clear TWINT and initiate enable
    TWCR = (1<<TWINT)|(1<<TWEN);

    //Wait for completion
    wait_for_completion;

}

void StopI2C_Trans(){

    //- Trigger action + stop condition
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    
}

void write(unsigned char data){
    /*
        This function loads the data passed into the I2C data register 
        and transmits
    */

   //Set two wire data register equal to incoming data
   TWDR = data ;

   //Trigger action
   TWCR = (1<<TWINT)|(1<<TWEN);

   //Wait for completion
   wait_for_completion;

}
void Read_from(unsigned char SLA, unsigned char MEMADDRESS){
    /*
        This function read from SLA the data at the SLA MEMADDRESS

    */

    //- Start a transmission to the SLA
    StartI2C_Trans(SLA);

    //- Write to the MEMADDRESS
    write(MEMADDRESS);

    //- Clear TWINT, initiate start condition, initiate enable
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

    //- Wait for completion
    wait_for_completion;

    //- Set two wire data register to the SLA + read bit
    TWDR = (SLA<<1)|0x01;

    //- Trigger action + master acknowledge bit
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);

    //- Wait for completion
    wait_for_completion;

    //- Trigger action
    TWCR = (1<<TWINT)|(1<<TWEN); 

    //- Wait for completion
    wait_for_completion;

    //Trigger action + stop condition
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);

}

unsigned char Read_data(){
    /*
        Returns the last byte

    */
    return TWDR;
}
