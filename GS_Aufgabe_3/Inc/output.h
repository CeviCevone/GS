#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "LCD_GUI.h"

int resetLCD(void);
int printPixel(uint16_t x, uint16_t y, COLOR color); 
int printLine(uint16_t y, uint16_t width, COLOR* colors);

#endif