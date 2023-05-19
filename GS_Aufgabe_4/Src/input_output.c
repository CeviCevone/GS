#include "input_output.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "time.h"



uint32_t pinOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0,0,0,0,0};
	
	/* Configure input lines             */
  /* even beeing input configure OType */
  GPIO_InitStruct.Pin = GPIO_PIN_0; 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull= GPIO_PULLUP ;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	return 0; 
}

uint32_t pinInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0,0,0,0,0};
	
	/* Configure input lines             */
  /* even beeing input configure OType */
  GPIO_InitStruct.Pin = GPIO_PIN_0; 
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull= GPIO_PULLUP ;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	return 0; 
}

uint32_t pinLow(void)
{
	pinOutput(); //setze GPIOD_0 auf output 
	GPIOD->BSRR = GPIO_PIN_0; //sende 0

	return 0; 
}

uint32_t writeOne(void)
{	
	pinLow();
	wait(6);
	pinInput(); //bus freigeben ? 
	wait(64);
	
	return 0; 
}
	
uint32_t writeNull(void)
{
	pinLow();
	wait(60);
	pinInput(); //bus freigeben ? 
	wait(10); 
	
	return 0;
}	
	
//liest byte aus 
uint32_t readByte(uint8_t* var)
{
	
	for(int i = 0; i < 8; ++i)
	{
		pinLow();
		wait(6); 
		pinInput(); //bus freigeben ? 
		wait(9);
		*var |= (GPIOD->IDR & GPIO_PIN_0) << i; //pin D0 abfragen 
		wait(55);
	}
	
	return 0;
}
	
uint32_t reset(void)
{
	uint32_t var = 0; 
	pinLow();
	wait(480); 
	pinInput(); 
	wait(70); 
	var = (GPIOD->IDR & GPIO_PIN_0); //pin D0 abfragen ; wofür? 
	wait(410); 
	
	return 0; 
}

uint32_t readRom(uint8_t* famCode, uint8_t* serialNumber, uint8_t* crc)
{
	reset(); 
	
	for(int i = 0; i < 2; ++i)
	{
		writeOne(); 
		writeOne();
		writeNull(); 
		writeNull();
	}
	
	readByte(famCode);
	
	for(int i = 0; i < 6; ++i)
	{
		readByte(&serialNumber[i]); 
	}
	
	readByte(crc); 
	
	return 0; 
}
	
	
	
	