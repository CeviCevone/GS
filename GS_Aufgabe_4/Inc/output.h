#ifndef _OUTPUT_H
#define	_OUTPUT_H

#include <stdint.h>

void lcdPrintTemp(int16_t vk, uint16_t nk, uint8_t pos); 
void lcdInitTemp(uint8_t pos);

#endif