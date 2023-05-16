#ifndef _TIME_H
#define	_TIME_H

#include <stdint.h> 

void initTime(void);
uint32_t getTime(uint32_t* time);
uint32_t resetTime(void);
uint32_t wait(uint32_t time);

#endif