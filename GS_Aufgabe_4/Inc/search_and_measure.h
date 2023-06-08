#ifndef _SEARCH_AND_MEASURE_H
#define	_SEARCH_AND_MEASURE_H

#include <stdint.h> 

void getTemp(uint64_t rom, int16_t* vk, uint16_t* nk);
uint8_t searchRom(uint64_t* rom, uint64_t rom_old);
void searchAllROM(uint64_t* rom, uint32_t* numRom);

#endif