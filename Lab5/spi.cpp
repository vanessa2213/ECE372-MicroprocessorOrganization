#include "spi.h"


//SPI uses pins 51-53 on the microcontroller. See pin diagram sheet for exact details

#define DDR_SPI         DDRB            //Data direction Register for SPI
#define SPI_PORT        PORTB           //Port pin 
#define DD_SS           DDB0            //SS Chip select pin 53
#define DD_SCK          DDB1            //SCK clock pin 52
#define DD_MOSI         DDB2            //MOSI pin 51
#define SPI_SS_BIT      PORTB0


#define wait_for_completion while(!(SPSR & (1 << SPIF)))


void initSPIMaster() {
    //set MOSI, SCK, SS outputs
    DDR_SPI = ((1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS));

    //since this is output-only to an LED matrix, we don't use the MISO line

    //set SS initially high as default
    SPI_PORT |= (1 << SPI_SS_BIT);

    //enable SPI in master mode, CPOL, CPHA, default clock and fosc/128
    //assumption: CPOL = 1, CHPA = 1 for sample on rising edge
    SPCR |= ((1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA) | (1 << SPR1) | (1 << SPR0));


}

void SPI_write_execute(unsigned char CMD, unsigned char data) {
    SPI_PORT &= ~(1 << SPI_SS_BIT); //set SS to 0 to begin communication

    SPDR = CMD;

    wait_for_completion;

    SPDR = data;

    wait_for_completion;

    SPI_PORT |= (1 << SPI_SS_BIT);

}