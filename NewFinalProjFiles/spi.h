#ifndef SPI_H
#define SPI_H

void initSPIMaster(); //initializes SPI in master mode
void SPI_write_execute(unsigned char CMD, unsigned char DATA); //writes a value to the SLA address specified

#endif