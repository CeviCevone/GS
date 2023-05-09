#include "output.h"
#include "LCD_GUI.h"

#define STARTOFLINE 0
#define OK 0

uint32_t resetLCD(void)
{
	GUI_clear(WHITE); 
	return OK; 
}

uint32_t printPixel(uint16_t x, uint16_t y, COLOR color)
{	
	Coordinate crd;
	crd.x = x; 
	crd.y = y; 
	if(VALID_COORDINATE(crd))
	{
		GUI_drawPoint(crd ,color, DOT_PIXEL_1X1, DOT_FILL_AROUND);
	}
	return OK; 
}

uint32_t printLine(uint16_t y, uint16_t width, COLOR* colors)
{	
	Coordinate crd;
	crd.x = STARTOFLINE; 
	crd.y = y; 
	if(VALID_COORDINATE(crd))
	{
		GUI_WriteLine(crd, width, colors); 
	}
	return OK; 
}

