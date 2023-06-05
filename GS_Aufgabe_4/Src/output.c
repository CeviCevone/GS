#include "output.h"
#include "lcd.h"
#define X_POS 7
#define NO_DECI_VALUE 1000

void lcdPrintTemp(int16_t vk, uint16_t nk, uint8_t pos)
{
	lcdGotoXY(X_POS, pos); 
		lcdPrintInt(vk);
		lcdPrintC('.'); 
		if(nk < NO_DECI_VALUE) //wenn nk > 1000 also keine direkte nachkommastelle z.B. 0.0625
		{
			lcdPrintC('0');
		}
		lcdPrintInt(nk);
}

void lcdInitTemp(uint8_t pos)
{
	for(uint32_t i = 0; i < pos; ++i)
	{
		lcdGotoXY(0,i); 
		lcdPrintS("Temp"); 
		lcdPrintInt(i); 
		lcdPrintS(": ");
	}
}