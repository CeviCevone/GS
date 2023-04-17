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

#define S7 1<<7
#define TICKS_PER_SECOND 90000000
#define TOLERANCE 100
#define D20 1 << 4

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){
	
	init();  
	
	int phase = 0; 
	int currentState = 0;
	int angVel = 0;
	unsigned int time = 0;
	int angle = 0; 
	int buttons = 0; 

	while(1)
	{	
		//daten Auslesen
		//GPIOE->BSRR = D20; 
		fetch(&time,&currentState, &buttons); // 750 ns 
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
