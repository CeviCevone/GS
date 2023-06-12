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
#include "interrupt.h"

#define S7 1<<7
#define TICKS_PER_SECOND 90000000
#define TOLERANCE 100
#define D20 1 << 4
#define TRUE 1

volatile int32_t currentState = 0;
volatile int32_t buttons = 0;
volatile uint32_t time = 0;
volatile int32_t phase = 0;
volatile int32_t ticks = 0;  
volatile int32_t angVel = 0;
volatile int32_t angle = 0; 
volatile uint32_t oldtime = 0;
volatile int32_t oldticks = 0; 

void EXTI0_IRQHandler(void)
{
		EXTI->PR |= (1<<0);
    fetch(&time,&currentState,&buttons);
		if(detectPhase(&phase, currentState,&ticks)) //276 ns
		{
			//fehlerhandling
			errorHandler();
			resetTicks(&ticks);
		}
		getVelocity(&angVel,time, ticks, &oldtime, &oldticks);
}

void EXTI1_IRQHandler(void)
{
		EXTI->PR |= (1<<1);
    fetch(&time,&currentState,&buttons);
		if(detectPhase(&phase, currentState, &ticks)) //276 ns
		{
			//fehlerhandling
			errorHandler();
			resetTicks(&ticks);
		}
		getVelocity(&angVel,time, ticks, &oldtime, &oldticks);
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){
	
	init(); 
	resetTicks(&ticks);
	initInterrupts();
	

	while(TRUE)
	{	
		fetch(&time,&currentState,&buttons);
		
		if(!(S7 & buttons)) //65ns - 122ns
		{
			resetTicks(&ticks);
		}	

	  totalAngle(&angle,&ticks); //90ns
		
		if(TOLERANCE >= (time % TICKS_PER_SECOND))
		{
			printValues(angle,angVel); //7 micro sec - 7,7ms
			getVelocity(&angVel,time, ticks, &oldtime, &oldticks);
		}
		
		//ausgabe 314ns 
		
		setDirectionalLed(phase);
		printTicks(&ticks);
	}
}

void initInterrupts(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; //schalte clock ein 
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
	
	SYSCFG->EXTICR[0] &= ~(0x0f << (4*0));
	SYSCFG->EXTICR[0] |= 0x06 << (4*0);
	
	SYSCFG->EXTICR[0] &= ~(0x0f << (4*1));
	SYSCFG->EXTICR[0] |= 0x06 << (4*1);
	
	EXTI->RTSR |= (1<<0); 
	EXTI->FTSR |= (1<<0); 
	EXTI->IMR |= (1<<0); 
	
	EXTI->RTSR |= (1<<1); 
	EXTI->FTSR |= (1<<1);
	EXTI->IMR |= (1<<1); 
	
	NVIC_SetPriorityGrouping(2); // Setup interrupt controller: // 4 subpriority for each priority
	NVIC_SetPriority(EXTI0_IRQn, 8); // Setup EXTI2: // priority = 2, subpriority = 0 // Enable EXTI2
	NVIC_EnableIRQ(EXTI0_IRQn);
	
	NVIC_SetPriorityGrouping(2); // Setup interrupt controller: // 4 subpriority for each priority
	NVIC_SetPriority(EXTI1_IRQn, 8); // Setup EXTI2: // priority = 2, subpriority = 0 // Enable EXTI2
	NVIC_EnableIRQ(EXTI1_IRQn);
}
// EOF
