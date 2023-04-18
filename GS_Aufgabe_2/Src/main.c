/**
 ******************************************************************************
 * @file    main.c
 * @author  Cevi, Ruben
 * @version 0.1
 *
 * @date    05.04.2023
 * @brief   Programm zum auslesen eines Drehgebers
 *
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
#include "timer.h"
#include "init_reset.h"
#include "fetch_update.h"
#include "errorhandling.h"
#include "output.h"
#include <stdint.h> 

#define S7 1<<7
#define TICKS_PER_SECOND 90000000
#define TOLERANCE 100
#define D20 1 << 4
#define TRUE 1

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int32_t main(void){
	
	init();  
	
	int32_t phase = 0; 
	int32_t currentState = 0;
	int32_t angVel = 0;
	uint32_t time = 0;
	int32_t angle = 0; 
	int32_t buttons = 0; 

	while(TRUE)
	{	
		//daten Auslesen
		//GPIOE->BSRR = D20; 
		fetch(&time,&currentState,&buttons); // 750 ns 
		//GPIOE->BSRR = D20 << 16;
		//zustände ändern
		
		
		
		if(detectPhase(&phase, currentState)) //650 ns
		{
			//fehlerhandling
			errorHandler();
		}
		
		
		
		if(!(S7 & buttons)) //sehr schnell
		{
			resetTicks();
		}	
		
	  totalAngle(&angle); 
		
		
		if(TOLERANCE >= (time % TICKS_PER_SECOND)) //1 mikrosec
		{
			deltaAngle(&angVel);
			printValues(angle,angVel);
		}
		
		//ausgabe
		setDirectionalLed(phase); //600 ns
		printTicks();
	}
}

// EOF
