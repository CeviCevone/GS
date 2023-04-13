#include "output.h"
#include "stm32f4xx_hal.h"
#include "fetch_update.h"
#include "lcd.h"
#include "LCD_GUI.h" 
#include <stdio.h>
#include "errorhandling.h"


#define D23 1 << 7 
#define D22 1 << 6
#define END '\0'

static char ang[16] = {' '};
static char angV[16] = {' '}; 

static int printNum(char* oldnum, int newNumber, int y); 

/**
  * @brief  Setzt die Leds anhand der Phase 
  * @param  phase - die aktuelle Phase
  * @retval None
  */
int setDirectionalLed(int phase)
{
	if(FORWARD == phase)
	{
		GPIOE->ODR = D23; 
		//D23 an
		//D22 aus
	}
	else if(BACKWARD == phase)
	{
		GPIOE->ODR = D22; 
		//D23 aus
		//D22 an 
	}	
	return OK; 
}

/**
  * @brief  Gibt die letzten acht Bits der Tickanzahl aud den Leds D8-D15 aus
  * @param  None
  * @retval None
  */
int printTicks(void)
{
	int i = 0; 
	getTicks(&i);
	GPIOD->ODR = (i & 0xFF); //letzte 8 bits von i 
	return OK; 
}
/**
  * @brief  initialisiert das Display 
  * @param  None
  * @retval None
  */
int initDisplay(void)
{
	GUI_clear(0xFFFF);
	
	for(int i = 0; i < 16; ++i)
	{
		ang[i] = ' '; 
		angV[i] = ' '; 
	}
	
	lcdGotoXY(1,1); 
	lcdPrintlnS("Drehgeschwindigkeit:");
	lcdGotoXY(1,4); 
	lcdPrintlnS("Drehwinkel:");
	return OK; 
}

/**
  * @brief  Druckt die Werte für den Winkel und die Geschwindigkeit auf dem Display 
  * @param  None
  * @retval None
  */
int printValues(int angle, int angVel)
{
	printNum(angV,angVel,2);
	printNum(ang,angle,5);
	
	return 0; 
}
/**
  * @brief  Druckt die Änderung zur letzten Zahl an der angegebenen Stelle 
  * @param  oldnum - die Ziffern der letzten Zahl, newNumber - die Zahl, y - die y Position 
  * @retval None
  */
static int printNum(char* oldnum, int newNumber, int y) 
{
	char newnum[16] = {0}; 
	
	for(int i = 0; i < 16; ++i)
	{
		newnum[i] = ' '; 
	}

	int len = sprintf(newnum, "%d", newNumber); // "     ...", 123 -> "123\   ..."; 
	
	//letzte int stelle hat den wert *10^-1 fügt komma ein 
	char temp = newnum[len-1];
	newnum[len-1] = '.'; 
	newnum[len] = temp; 
	newnum[len+1] = END; 
  
	
	for(int i = 0; i < 16; ++i)
	{
		//wenn sich eine ziffer ändert -> an die entsprechende stelle gehen 
		if(oldnum[i] != newnum[i])
		{	
			lcdGotoXY(i+1,y);
			
			// '\0' -> ende 
			if(END == newnum[i])
			{
				lcdPrintC(' ');
				oldnum[i] = END;
				break;
			}
			//Druckt ziffer
			lcdPrintC(newnum[i]); 
			oldnum[i] = newnum[i]; 
		}
	}
	return OK;
}