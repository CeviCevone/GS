#include "time.h" 
#include <stm32f4xx_hal.h>
#include <stdint.h> 

#define TIM_FREQ 180
#define MICRO_SEC_FREQ 1

static uint32_t deltaTime = 0; 
static uint32_t time = 0; 

void initTime(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* Takt fuer Timer 2 einschalten */
    TIM2->CR1 = 0;                      /* Timer disabled                */
    TIM2->CR2 = 0;                      /*                               */
    TIM2->PSC = (TIM_FREQ / MICRO_SEC_FREQ)-1;                      /* Prescaler   (180MHz)          */
    TIM2->ARR = 0xffffffff;             /* Auto reload register          */
    TIM2->DIER  = 0;                    /* Interrupt ausschalten         */
    TIM2->CR1 = TIM_CR1_CEN;            /* Enable Timer                  */
}

uint32_t getTime(uint32_t* time)
{
	*time = TIM2->CNT; 
	return 0; 
}

uint32_t resetTime(void)
{
	TIM1->CNT = 0; 
	return 0; 
}


uint32_t wait(uint32_t time)
{
	resetTime();
	getTime(&time); 
	do
	{
		deltaTime = TIM1->CNT - time; 
	}
	while(time > deltaTime); // warte x micro sec
	
	return 0; 
}