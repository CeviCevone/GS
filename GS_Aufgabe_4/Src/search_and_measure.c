#include "search_and_measure.h"
#include "one_wire.h"
#include "lcd.h"

#define FALSE 0
#define TRUE 1
#define OK 0 
#define ERR 1
#define SEARCH_COMMAND 0xF0
#define BIT_SET 1
#define BIT_NOT_SET 0
#define END_OF_ROM 64
#define MAX_NUM_ROM 4 
#define SRATCHPAD_SIZE 9
#define BITS_PER_BYTE 8
#define SIGNED_BIT (0x1 << 15)
#define INTEGER_PART 4
#define FIRST_DECIMAL_DIGIT (0x1 << 3)
#define SECOND_DECIMAL_DIGIT (0x1 << 2)
#define THIRD_DECIMAL_DIGIT (0x1 << 1)
#define FOURTH_DECIMAL_DIGIT (0x1 << 0)
#define FIRST_DECIMAL_DIGIT_VALUE 5000
#define SECOND_DECIMAL_DIGIT_VALUE 2500
#define THIRD_DECIMAL_DIGIT_VALUE 1250
#define FOURTH_DECIMAL_DIGIT_VALUE 625

static uint8_t LastDeviceFlag = FALSE;
static uint32_t LastDiscrepancy = 0; 
static uint32_t LastFamilyDiscrepancy = 0; 
 

uint8_t resetSearch(); 

uint8_t searchRom(uint64_t* rom, uint64_t rom_old)
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
	
	id_bit_number = 1; 
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
				search_direction = (1 &(rom_old >> (id_bit_number-1)));
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
		
		lcdPrintC(search_direction+'0');
		rom_no |= (((uint64_t) search_direction) << (id_bit_number-1));
	
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
	while(id_bit_number <= END_OF_ROM); 
	
	lcdPrintlnS(" ");
	
	lcdPrintInt(LastDiscrepancy);
	lcdPrintC('_');
	LastDiscrepancy = last_zero; 
	lcdPrintInt(last_zero);
	lcdPrintlnS(" ");
	
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
	uint8_t temp[SRATCHPAD_SIZE] = {0};
	uint8_t negative = FALSE; 
	
	uint16_t res = 0;  
	
	readTemp(rom,temp);
	
	for(uint32_t i = 0; i < 2; ++i)
	{
		res |= ((int16_t) temp[i]) << (i* BITS_PER_BYTE); //extrahiert erste 2 byte mit der temperatur info 
	}
	
	if(res & SIGNED_BIT)
	{
		res = ~res; 
		res += 0x1; //tauscht das vorzeichen von res 
			
		negative = TRUE; 
	}
	
	*vk = res >> INTEGER_PART;
	
	if(negative)
	{
		*vk = -*vk; // da vorkomma signed 
	}
		
	if(res & FIRST_DECIMAL_DIGIT) //wandelt binär nachkomma in integer um 
	{
		*nk += FIRST_DECIMAL_DIGIT_VALUE; 
	}
	if(res & SECOND_DECIMAL_DIGIT)
	{
		*nk += SECOND_DECIMAL_DIGIT_VALUE; 
	}
	if(res & THIRD_DECIMAL_DIGIT)
	{
		*nk += THIRD_DECIMAL_DIGIT_VALUE; 
	}
	if(res & FOURTH_DECIMAL_DIGIT)
	{
		*nk += FOURTH_DECIMAL_DIGIT_VALUE; 
	}
}

void searchAllROM(uint64_t* rom, uint32_t* numRom)
{
	for(uint32_t i = 0; i < MAX_NUM_ROM; ++i)
	{
		uint64_t old_rom = 0; 
		if(i)
		{
			old_rom = rom[i-1]; 
		}
		
		if(searchRom(&rom[i], old_rom))
		{
			*numRom += 1; 
		}
		else 
		{
			return;
		}
	}	
}