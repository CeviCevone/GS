#include "output.h"
#include "stm32f4xx_hal.h"
#include "fetch_update.h"
#include "lcd.h"
#include "LCD_GUI.h" 
#include <stdio.h>
#include "errorhandling.h"
#include <stdint.h>

#define D23 1 << 7 
#define D22 1 << 6
#define END '\0'
#define SIZE 16
#define TICK_BIT_MASK 0xFF

static char ang[SIZE] = {' '};
static char angV[SIZE] = {' '}; 

static int32_t printNum(char* oldnum, int32_t newNumber, int32_t y); 

/**
  * @brief  Setzt die Leds anhand der Phase 
  * @param  phase - die aktuelle Phase
  * @retval None
  */
int32_t setDirectionalLed(int32_t phase)
{
	if(FORWARD == phase)
	{
		GPIOE->BSRR = D23; 
		GPIOE->BSRR = (D22 << 16); 
		//D23 an
		//D22 aus
	}
	else if(BACKWARD == phase)
	{
		GPIOE->BSRR = D22; 
		GPIOE->BSRR = (D23 << 16); 
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
int32_t printTicks(void)
{
	int32_t i = 0; 
	getTicks(&i);
	GPIOD->BSRR = (TICK_BIT_MASK << 16);
	GPIOD->BSRR = (i & TICK_BIT_MASK); //letzte 8 bits von i 
	return OK; 
}
/**
  * @brief  initialisiert das Display 
  * @param  None
  * @retval None
  */
int32_t initDisplay(void)
{
	GUI_clear(0xFFFF);
	
	for(int32_t i = 0; i < SIZE; ++i)
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
int32_t printValues(int32_t angle, int32_t angVel)
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
static int32_t printNum(char* oldnum, int32_t newNumber, int32_t y) 
{
	char newnum[SIZE] = {0}; 
	
	for(int32_t i = 0; i < SIZE; ++i)
	{
		newnum[i] = ' '; 
	}

	int32_t len = sprintf(newnum, "%d", newNumber); // "     ...", 123 -> "123\0   ..."; 
	
	//letzte int stelle hat den wert *10^-1 fügt komma ein 
	char temp = newnum[len-1];
	newnum[len-1] = '.'; 
	newnum[len] = temp;  
	newnum[len+1] = END; 
  
	
	for(int32_t i = 0; i < SIZE; ++i)           
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