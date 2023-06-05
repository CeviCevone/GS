#include "gpio_basefunct.h"
#include "stm32f4xx_hal.h" 
#include "lcd.h"

#define PIN1 1 
#define PIN0 0  
#define BIT_SET 1 
#define BIT_NOT_SET 0
#define BIT_MASK 0x01
#define BSSR_HIGH 16
#define MODER_MASK_IN (0x03 << (2*0))
#define MODER_MASK_OUT 0x01 <<(2*0)

void init_GPIO(void)
{
	GPIOD->OTYPER &= ~(BIT_SET << PIN1); 
	GPIOD->BSRR = (BIT_SET << PIN1); 
	open_drain(); 
}

void open_drain(void)
{
	GPIOD->OTYPER |= (BIT_SET << PIN0);
}

void push_pull(void)
{
	GPIOD->OTYPER &= ~(BIT_SET << PIN0); 
}

void GPIO_In(void)
{
	GPIOD->MODER &= ~(MODER_MASK_IN); 
}

void GPIO_Out(void)
{
	GPIOD->MODER |= (MODER_MASK_OUT); 
}

void GPIO_High(void)
{
	GPIOD->BSRR = (BIT_SET << PIN0); 
}

void GPIO_Low(void)
{
	GPIOD->BSRR = ((BIT_SET << PIN0) << BSSR_HIGH); 
}


void GPIO_Read(uint8_t *var, uint32_t shift)
{
	uint8_t temp = (GPIOD->IDR & BIT_MASK); 

	//if(temp)   //test
	//{
	//	lcdPrintC('1');
	//}
	//else 
	//{
	//	lcdPrintC('0'); 
	//}	//test ende 
	
	*var |= (temp << shift);
}

