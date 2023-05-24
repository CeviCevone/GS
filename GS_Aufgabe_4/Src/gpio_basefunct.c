#include "gpio_basefunct.h"
#include "stm32f4xx_hal.h" 
#include "lcd.h"

#define PIN1 1 
#define Pin0 0  

void init_GPIO(void)
{
	GPIOD->OTYPER &= ~(1 << PIN1); 
	GPIOD->BSRR = (1 << PIN1); 
	open_drain(); 
}

void open_drain(void)
{
	GPIOD->OTYPER |= (1<<0);
}

void push_pull(void)
{
	GPIOD->OTYPER &= ~(1 << 0); 
}

void GPIO_In(void)
{
	GPIOD->MODER &= ~(0x03 << (2*0)); 
}

void GPIO_Out(void)
{
	GPIOD->MODER |= (0x01 <<(2*0)); 
}

void GPIO_High(void)
{
	GPIOD->BSRR = (1 << 0); 
}

void GPIO_Low(void)
{
	GPIOD->BSRR = ((1 << 0) << 16); 
}

void GPIO_Read(uint64_t *var, uint32_t shift)
{
	uint64_t temp = (GPIOD->IDR & 0x01); 
	
	if(temp)   //test
	{
		lcdPrintC('1');
	}
	else 
	{
		lcdPrintC('0'); 
	}	//test ende 
	
	*var |= (temp << shift);
}

