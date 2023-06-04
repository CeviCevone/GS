#include "search_and_measure.h"
#include "one_wire.h"

#define FALSE 0
#define TRUE 1
#define OK 0 
#define ERR 1
#define SEARCH_COMMAND 0xF0
#define BIT_SET 1
#define BIT_NOT_SET 0
#define END_OF_ROM 64


static uint8_t LastDeviceFlag = FALSE;
static uint32_t LastDiscrepancy = 0; 
static uint32_t LastFamilyDiscrepancy = 0; 
 

uint8_t resetSearch(); 

uint8_t searchRom(uint64_t* rom)
{ 
	uint64_t rom_no = 0;
	uint8_t cmp_id_bit = 0; 
	uint8_t id_bit = 0; 
	uint32_t id_bit_number = 0; 
	uint32_t last_zero = 0; 
	uint32_t search_direction = 0; 
	
	if(io_reset())
	{
		resetSearch();
		return FALSE; 
	}
	
	if(TRUE == LastDeviceFlag)
	{
		resetSearch(); 
		return FALSE; 
	}
	
	id_bit_number = 0; 
	last_zero = 0; 
	
	writeByte(SEARCH_COMMAND); 
	
	do
	{	
		id_bit = 0; 
		cmp_id_bit = 0; 
		
		read_bit(&id_bit,0); 
		read_bit(&cmp_id_bit,0);
	
		if((BIT_SET == id_bit) && (BIT_SET == cmp_id_bit))
		{
			resetSearch(); 
			return FALSE; 
		}
	
		if((BIT_NOT_SET == id_bit) && (BIT_NOT_SET == cmp_id_bit))
		{
			if(id_bit_number == LastDiscrepancy)
			{
				search_direction = 1; 
			}
			else if(id_bit_number > LastDiscrepancy)
			{
				search_direction = 0; 
			}
			else 
			{
				search_direction = (rom_no & (((uint64_t) BIT_SET )<< id_bit_number));
			}
		
			if(0 == search_direction)
			{
				last_zero = id_bit_number; 
			
				if(last_zero < 9)
				{
					LastFamilyDiscrepancy = last_zero; 
				}
			}
		}
		else
		{
			search_direction = id_bit; 
		}
		rom_no |= (((uint64_t) search_direction) << id_bit_number);
	
		if(search_direction)
		{
			write_one(); 
		}
		else 
		{
			write_zero();
		}
		++id_bit_number; 
	
	}
	while(id_bit_number < END_OF_ROM); 
	
	LastDiscrepancy = last_zero; 
	
	if(FALSE == LastDiscrepancy)
	{
		LastDeviceFlag = TRUE; 
	}
	if(checkCRC(rom_no,7))
	{
		*rom = rom_no;
		return TRUE; 
	}
	else 
	{
		resetSearch(); 
		return FALSE; 
	}
}

uint8_t resetSearch()
{
	LastDiscrepancy = 0; 
	LastFamilyDiscrepancy = 0; 
	LastDeviceFlag = 0; 
	return OK; 
}

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