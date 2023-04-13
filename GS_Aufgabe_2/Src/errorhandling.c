#include "errorhandling.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "init_reset.h"
#include "LCD_GUI.h"

#define S6 1 << 6
#define D21 1 << 5

/**
  * @brief  Gibt Fehlermeldung aus, setzt das Programm bei Druck von S6 zurück
  * @param  None
  * @retval None
  */
int errorHandler(void)
{
	GUI_clear(0xFFFF);
	lcdGotoXY(1,6); 
	lcdPrintlnS("Fehler: Nicht erfasster Zustand"); 
	lcdGotoXY(1,7); 
	lcdPrintlnS("S6 zum Reset druecken"); 
	
	GPIOE->ODR = D21; //D21 an
	while(S6 & GPIOF->IDR); // s6 = 01000000 & 11111111 = 01000000 =! 0 
													// s6 & 10111100 -> 00000000 == 0 -> endwhile
	GPIOE->ODR = 0; 	//D21 aus
	reset();
	
	return 0;
}
