#include "output.h"
#include "LCD_GUI.h"

int resetLCD(void)
{
	GUI_clear(WHITE); 
	return 0; 
}

int printPixel(uint16_t x, uint16_t y, COLOR color)
{	
	Coordinate crd;
	crd.x = x; 
	crd.y = y; 
	if(VALID_COORDINATE(crd))
	{
		GUI_drawPoint(crd ,color, DOT_PIXEL_1X1, DOT_FILL_AROUND);
	}
	return 0; 
}

int printLine(uint16_t y, uint16_t width, COLOR* colors)
{	
	Coordinate crd;
	crd.x = 0; 
	crd.y = y; 
	if(VALID_COORDINATE(crd))
	{
		GUI_WriteLine(crd, width, colors); 
	}
	return 0; 
}

