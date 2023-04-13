#include "fetch_update.h"
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "errorhandling.h"

#define A 0 
#define B 2 
#define C 3 
#define D 1
#define true 1
#define false 0

static int init = true; 
static int lastState = 0; 
static int ticks = 0;  
static int lastTicks = 0; 

/**
  * @brief  Liest die aktuellen Werte aus 
  * @param  time - der Wert des Timers, currentState - der Zustand, buttons - das Bitmuster der Knöpfe 
  * @retval none 
  */
int fetch(unsigned int* time, int* currentState, int* buttons)
{
	*time = TIM2->CNT; 
	*currentState = (GPIOF->IDR & 3); //letzte 2 bits der Eingabe holen
	*buttons = GPIOF->IDR;
	return OK; 
}

/**
  * @brief  liefert die aktuelle Phase anhand des aktuellen Zustands 
  * @param  i - die Rückgabe liefert die Phase, currentState - der aktuelle Zustand
  * @retval 1 - falls eine Phase übersprungen wurde
  */
int detectPhase(int* i,int currentState)
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
int deltaAngle(int* i)
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
int totalAngle(int* i)
{
	*i = 3*ticks; 
	return OK; 
}

/**
  * @brief  Setzt die Ticks auf null
  * @param  None
  * @retval None
  */
int resetTicks(void)
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
int getTicks(int* i)
{
	*i = ticks; 
	return OK; 
}
