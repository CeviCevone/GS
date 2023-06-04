#include "output.h"
#include "lcd.h"
#define X_POS 7

void lcdPrintTemp(int16_t vk, uint16_t nk, uint8_t pos)
{
	lcdGotoXY(X_POS, pos); 
		lcdPrintInt(vk);
		lcdPrintC('.'); 
		if(nk < 1000)
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