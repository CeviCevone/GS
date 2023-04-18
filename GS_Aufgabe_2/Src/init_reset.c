#include "init_reset.h"
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
#include "timer.h"
#include "fetch_update.h"
#include "output.h"
#include "errorhandling.h"

/**
  * @brief  initialisiert das Board, startet den Timer 
  * @param  None
  * @retval None
  */
int32_t init(void)
{
	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}
	
	initDisplay();
	initTimer();
	resetTicks();
	
	return OK; 
}

/**
  * @brief  Setzt das Display, den Timer und die Ticks zurück
  * @param  None
  * @retval None
  */
int32_t reset(void)
{
	initDisplay();
	initTimer();
	resetTicks();
	return OK; 
}