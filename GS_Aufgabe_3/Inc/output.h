#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "LCD_GUI.h"

/**
* @brief  this function resets the LCD Display 
* @retval 0 
*/
uint32_t resetLCD(void);

/**
* @brief  prints a pixel on the LCD Display 
* @param  x the x coordinate 
* @param  y the y coordinate 
* @param  color the color of the pixel
* @retval 0 
*/
uint32_t printPixel(uint16_t x, uint16_t y, COLOR color); 

/**
* @brief  prints a line of pixels on the LCD Display 
* @param  y the y coordinate 
* @param  width the length of the line
* @param  colors the colors of the pixels
* @retval 0 
*/
uint32_t printLine(uint16_t y, uint16_t width, COLOR* colors);

#endif