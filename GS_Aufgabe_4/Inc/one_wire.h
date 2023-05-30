#ifndef _ONE_WIRE_H
#define	_ONE_WIRE_H

#include <stdint.h> 

uint32_t io_reset(void);
void write_one(void);
void write_zero(void);
uint32_t read_bit(uint8_t* var, uint32_t shift);
uint32_t readRom(uint64_t* var);
uint32_t read_byte(uint8_t* var);
void writeByte(uint8_t byte); //sendet ein byte  
uint8_t checkCRC(uint64_t rom, uint32_t bytecount);
uint32_t readTemp(uint64_t rom, uint8_t* res);

#endif