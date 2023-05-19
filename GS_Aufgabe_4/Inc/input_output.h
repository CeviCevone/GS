#ifndef _INPUT_OUTPUT_H
#define	_INPUT_OUTPUT_H

#include <stdint.h> 

uint32_t pinInput(void);
uint32_t pinOutput(void);
uint32_t readRom(uint8_t* famCode, uint8_t* serialNumber, uint8_t* crc);

#endif