#include "time.h" 
#include "timer.h"
#include <stdint.h>
#include <stm32f4xx_hal.h>


uint32_t wait(uint32_t time)
{
	 TIM2->CNT = 0; 
	 uint32_t waitTime = time * TICKS_PER_US; 
	 uint32_t startTime = TIM2->CNT; 
	 uint32_t deltaTime = 0; 
	
	 do
	 {
		 deltaTime = TIM2->CNT; 
	 }
	 while((deltaTime - startTime) < waitTime); 
	 
	 return 0; 
}