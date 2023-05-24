/**
 ******************************************************************************
 * @file    main.c
 * @author  TI Team
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    17. Juli 2020
 * @brief   Rahmen fuer C Programme, die auf dem ITSboard ausgefuehrt werden.
 *
 *					25.04.2022 Tobias De Gasperis: printf hinzugefuegt
 ******************************************************************************
 */

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"
#include "one_wire.h"
#include "time.h"
#include "timer.h"
#include "gpio_basefunct.h"

void demo(void); 

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){
	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}
	
	demo(); 
}


void demo(void)
{
	initTimer(); 
	init_GPIO(); 
	
	uint64_t rom = 0; 
	readRom(&rom); 
	
	if(!checkCRC(rom,7))
	{
		Error_Handler(); 
	}
	
	for(uint32_t i; i < 64; ++i)
	{
		lcdGotoXY(i,2); 
		lcdPrintC(rom & (1 << i)); 
	}
}
	
// EOF
