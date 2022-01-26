#include "segments.h"
#include "timer.h"

void init7Segments(){

    //SETS AS OUTPUT C0-C6 PIN 31-37
    //DDRC |= (1<<DDC6)|(1<<DDC5)|(1<<DDC4)|(1<<DDC3)|(1<<DDC2)|(1<<DDC1)|(1<<DDC0);     
    //PORTC &= ~((1<<PORTC6)|(1<<PORTC5)|(1<<PORTC4)|(1<<PORTC3)|(1<<PORTC2)|(1<<PORTC1)|(1<<PORTC0));     

    // for sending the data C6 = PIN 31
    //for clock and enable C7  PIN30
    DDRC |= (1<<DDC7)|(1<<DDC6);
    PORTC &= ~((1<<PORTC7)|(1<<PORTC6));

}
void turnOff7segments(){
    //PORTC &= ~((1<<PORTC6)|(1<<PORTC5)|(1<<PORTC4)|(1<<PORTC3)|(1<<PORTC2)|(1<<PORTC1)|(1<<PORTC0)); 
    shiftData(0); 
}

void shiftData(unsigned short data){
    //lets use PORTC6 pin 31 to send the data and PORTC7 pin 30 to enable
    Serial.begin(9600);
    for(unsigned short i = 0x01; i< 0x80; i<<=1){
        
        //Serial.println(i);
        if((data & i)){ //checks if ones
            PORTC |= (1<<PORTC6); //send a one
           // Serial.println("True"); 
        }
        else{
            PORTC &= ~(1<<PORTC6); //send a 0
           // Serial.println("False"); 
        }
        PORTC |= (1<<PORTC7); //turn on enable
        PORTC &= ~(1<<PORTC7); //turn off enable
        
        
    }
    PORTC |= (1<<PORTC7); //turn on enable
    PORTC &= ~(1<<PORTC7); //turn off enable

    
}

void turnOn7segments( int num){//receives the seconds
    switch (num)
    {
    
    case 9:
        //0b0110 1111
        //PORTC  = (PORTC & 0x80) | (0x6F& 0x7F); //withouth shift register
        shiftData(0x6F);
        break;
    case 8:
        //0b0111 1111
        //PORTC  = (PORTC & 0x80) | (0x7F& 0x7F); //withouth shift register
        shiftData(0x7F);
        break;
    case 7:
        //0b0000 0111
        //PORTC  = (PORTC & 0x80) | (0x07& 0x7F); //withouth shift register
        shiftData(0x07);
        break;
    case 6:
        //0b0111 1101
        //PORTC  = (PORTC & 0x80) | (0x7D& 0x7F); //withouth shift register
        shiftData(0x7D);
        break;
    case 5:
        //0b0110 1101
        //PORTC  = (PORTC & 0x80) | (0x6D& 0x7F); //withouth shift register
        shiftData(0x6D);
        break;
    case 4:
        //0b 0110 0110
        //PORTC  = (PORTC & 0x80) | (0x66& 0x7F); //withouth shift register
        shiftData(0x66);
        break;
    case 3:
        //0b0100 1111
        //PORTC  = (PORTC & 0x80) | (0x4F& 0x7F); //withouth shift register
        shiftData(0x4F);
        break;
    case 2:
        //0b0101 1011
        //PORTC  = (PORTC & 0x80) | (0x5B& 0x7F); //withouth shift register
        shiftData(0x5B);
        break;
    case 1:
        //0b0000 0110
        //PORTC  = (PORTC & 0x80) | (0x06& 0x7F); //withouth shift register
        shiftData(0x06);
        break;
    case 0:
        //0b0011 1111
        //PORTC  = (PORTC & 0x80) | (0x3F& 0x7F); //withouth shift register
        shiftData(0x3F);
        break;
    
    default:
        turnOff7segments();
        break;
    }
    
}

