#ifndef SEGMENTS_H
#define SEGMENTS_H
#include <avr/io.h>
#include <Arduino.h>

void init7Segments();
void turnOn7segments(int num);
void turnOff7segments();
void shiftData(unsigned short data);


#endif