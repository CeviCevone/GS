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

static int32_t init = true; 
static int32_t lastState = 0; 
static int32_t ticks = 0;  
static int32_t lastTicks = 0; 

/**
  * @brief  Liest die aktuellen Werte aus 
  * @param  time - der Wert des Timers, currentState - der Zustand, buttons - das Bitmuster der Knöpfe 
  * @retval none 
  */
int32_t fetch(uint32_t* time, int32_t* currentState, int32_t* buttons)
{
	*time = TIM2->CNT; 
	*currentState = (GPIOF->IDR & MASK_PIN_0_1); //letzte 2 bits der Eingabe holen 
	*buttons = GPIOF->IDR;		
	return OK; 
}

/**
  * @brief  liefert die aktuelle Phase anhand des aktuellen Zustands 
  * @param  i - die Rückgabe liefert die Phase, currentState - der aktuelle Zustand
  * @retval 1 - falls eine Phase übersprungen wurde
  */
int32_t detectPhase(int32_t* i,int32_t currentState)
{	
	if(init)
	{
		lastState = currentState;
		init = false; 
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
					++ticks;
				}
				else if(D == currentState)
				{
					*i = BACKWARD; 
					--ticks;
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
					++ticks; 
				}
				else if(A == currentState)
				{
					*i = BACKWARD; 
					--ticks;
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
				 	++ticks;
				 }
				 else if(B == currentState)
				 {
				 	*i = BACKWARD; 
				 	--ticks;
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
					++ticks; 
				}
				else if(C == currentState)
				{
					*i = BACKWARD; 
					--ticks;
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
int32_t deltaAngle(int32_t* i)
{
	*i = 3*(ticks - lastTicks); 
	lastTicks = ticks;
	return OK; 
}

/**
  * @brief  liefert den Winkel in Relation zum Ausgangspunkt
  * @param  i - die Rückgabe
  * @retval None
  */
int32_t totalAngle(int32_t* i)
{
	*i = 3*ticks; 
	return OK; 
}

/**
  * @brief  Setzt die Ticks auf null
  * @param  None
  * @retval None
  */
int32_t resetTicks(void)
{
	init = true; 
	ticks = 0; 
	lastTicks = 0; 
	return OK; 
}

/**
  * @brief  liefert die Anzahl der Ticks 
  * @param  i - die Rückgabe
  * @retval None
  */
int32_t getTicks(int32_t* i)
{
	*i = ticks; 
	return OK; 
}
