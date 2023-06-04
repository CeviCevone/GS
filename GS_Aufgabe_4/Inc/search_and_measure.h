#ifndef _SEARCH_AND_MEASURE_H
#define	_SEARCH_AND_MEASURE_H

#include <stdint.h> 

void getTemp(uint64_t rom, int16_t* vk, uint16_t* nk);
uint8_t searchRom(uint64_t* rom);

#endif