#include "search_and_measure.h"
#include "one_wire.h"

#define FALSE 0
#define TRUE 1





void getTemp(uint64_t rom, int16_t* vk, uint16_t* nk)
{
	uint8_t temp[9] = {0};
	uint8_t negative = FALSE; 
	
	uint16_t res = 0;  
	
	readTemp(rom,temp);
	
	for(uint32_t i = 0; i < 2; ++i)
	{
		res |= ((int16_t) temp[i]) << (i*8); 
	}
	
	if(res & (0x1 << 15))
	{
		res = ~res; 
		res += 0x1;
			
		negative = TRUE; 
	}
	
	*vk = res >> 4;
	
	if(negative)
	{
		*vk = -*vk; 
	}
		
	if(res & (0x1 << 3))
	{
		*nk += 5000; 
	}
	if(res & (0x1 << 2))
	{
		*nk += 2500; 
	}
	if(res & (0x1 << 1))
	{
		*nk += 1250; 
	}
	if(res & (0x1 << 0))
	{
		*nk += 625; 
	}
}