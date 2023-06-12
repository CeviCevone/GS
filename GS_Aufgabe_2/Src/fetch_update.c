#include "fetch_update.h"
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "errorhandling.h"

#define A 0b00 
#define B 0b10
#define C 0b11 
#define D 0b01
#define true 1
#define false 0
#define MASK_PIN_0_1 0b11

volatile int32_t isinit = true; 
volatile int32_t lastState = 0; 
volatile int32_t lastTicks = 0; 
volatile uint32_t timeold = 0; 

/**
  * @brief  Liest die aktuellen Werte aus 
  * @param  time - der Wert des Timers, currentState - der Zustand, buttons - das Bitmuster der Knöpfe 
  * @retval none 
  */
int32_t fetch(volatile uint32_t* time,volatile int32_t* currentState,volatile int32_t* buttons)
{
	*time = TIM2->CNT; 
	*currentState = (GPIOG->IDR & MASK_PIN_0_1); //letzte 2 bits der Eingabe holen 
	*buttons = GPIOF->IDR;		
	return OK; 
}

/**
  * @brief  liefert die aktuelle Phase anhand des aktuellen Zustands 
  * @param  i - die Rückgabe liefert die Phase, currentState - der aktuelle Zustand
  * @retval 1 - falls eine Phase übersprungen wurde
  */
int32_t detectPhase(volatile int32_t* i,volatile int32_t currentState, volatile int32_t* ticks)
{	
	if(isinit)
	{
		lastState = currentState;
		isinit = false; 
	}
	
	switch(lastState) //anhand des Letzten und des aktuellen Zustands die Phase ermitteln
	{
		case A:
			{
				if(A == currentState)
				{
					*i = STANDING;  
				}
				else if(B == currentState)
				{
					*i = FORWARD; 
					*ticks += 1;
				}
				else if(D == currentState)
				{
					*i = BACKWARD; 
					*ticks -= 1;
				}
				else 
				{
					return ERR; 
				}
				break;
			}
		case B: 
			{
				if(B == currentState)
				{
					*i = STANDING;  
				}
				else if(C == currentState)
				{
					*i = FORWARD; 
					*ticks += 1; 
				}
				else if(A == currentState)
				{
					*i = BACKWARD; 
					*ticks -= 1;
				}
				else 
				{
					return ERR; 
				}
				break;
			}
		case C: 
			{
				 if(C == currentState)
				 {
				 	*i = STANDING;  
				 }
				 else if(D == currentState)
				 {
				 	*i = FORWARD; 
				 	*ticks += 1;
				 }
				 else if(B == currentState)
				 {
				 	*i = BACKWARD; 
				 	*ticks -= 1;
				 }
				 else 
				 {
				 	return ERR; 
				 }
				 break;
			}
		case D: 
			{
				if(D == currentState)
				{
					*i = STANDING;  
				}
				else if(A == currentState)
				{
					*i = FORWARD; 
					*ticks += 1; 
				}
				else if(C == currentState)
				{
					*i = BACKWARD; 
					*ticks -= 1;
				}
				else 
				{
					return ERR; 
				}
				break;
			}
	}
	
	lastState = currentState; 
	return OK; 
}

/**
  * @brief  liefert die Winkeländerung zum letzten Aufruf 
  * @param  i - die Rückgabe
  * @retval None
  */
int32_t deltaAngle(volatile int32_t* i,volatile uint32_t time, volatile int32_t* ticks)
{
	//*i = ((10*(ticks -lastTicks)) /(3*(time - timeold)))*90000000 ; 
	//timeold = time;
	*i = 3*(*ticks-lastTicks);
	lastTicks = *ticks;
	return OK; 
}

/**
  * @brief  liefert den Winkel in Relation zum Ausgangspunkt
  * @param  i - die Rückgabe
  * @retval None
  */
int32_t totalAngle(volatile int32_t* i, volatile int32_t* ticks)
{
	*i = 3*(*ticks); 
	return OK; 
}

/**
  * @brief  Setzt die Ticks auf null
  * @param  None
  * @retval None
  */
int32_t resetTicks(volatile int32_t* ticks)
{
	isinit = true; 
	*ticks = 0; 
	lastTicks = 0; 
	return OK; 
}
