#include "output.h"
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "LCD_GUI.h" 
#include <stdio.h>
#include <stdint.h>

#define OK 0
#define END '\0'
#define SIZE 16

static char oldNum[SIZE] = {' '};

static int32_t printNum(char* oldnum, int32_t newNumber, int32_t y); 

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
		oldNum[i] = ' '; 
	}
	
	lcdGotoXY(1,1); 
	lcdPrintlnS("Registration ROM");
	
	return OK;
}

/**
  * @brief  Druckt die Werte für den Winkel und die Geschwindigkeit auf dem Display 
  * @param  None
  * @retval None
  */
int32_t printValues(int32_t num)
{
	printNum(oldNum,num,2);
	
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