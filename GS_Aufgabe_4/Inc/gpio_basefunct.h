#ifndef _GPIO_BASEFUNCT_H
#define	_GPIO_BASEFUNCT_H

#include <stdint.h> 

void init_GPIO(void); 
void open_drain(void); 
void push_pull(void); 
void GPIO_In(void); 
void GPIO_Out(void); 
void GPIO_High(void);
void GPIO_Low(void);
void GPIO_Read(uint64_t *var, uint32_t shift);

#endif