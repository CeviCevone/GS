#include "decode.h" 
#include "BMP_types.h"
#include "input.h"
#include "output.h"
#include "basic_checks.h"
#include "errorhandler.h"
#include "MS_basetypes.h"
#include "LCD_general.h"
#include <stdio.h>
#include <stdlib.h> 

#define BMP_UNCOMPRESSED_24BIT 24
#define BMP8BIT 8
#define OK 0
#define MAXWIDTH 2000
#define MAX_COLORS_USED 0
#define MAX_NUM_COLORS 256
#define UNCOMPRESSED 0
#define COMPRESSED 1 
#define TRUE 1
#define FALSE 0
#define START_OF_LINE 0
#define BYTES_PER_24BIT 3

static BITMAPFILEHEADER fileHeader; 
static BITMAPINFOHEADER infoHeader; 
static RGBQUAD colorTable[MAX_NUM_COLORS];
static COLOR* colors; 

uint32_t toSmallRGB(uint8_t red, uint8_t green, uint8_t blue, uint16_t* color);
uint32_t decodeFileHeader(void);
uint32_t decodeInfoHeader(void);
uint32_t fillColorTable(void);
uint32_t printAllPixels(void);
uint32_t compressed8bit(void);
uint32_t uncompressed8bit(void);
uint32_t uncompressed24bit(void);
uint32_t addToArr(uint16_t x, COLOR color);
	
/**
* @brief  decodes and prints the send bmp
* @retval 0 
*/
uint32_t decodeAndPrint()
{
	
	decodeFileHeader();
	decodeInfoHeader();
	fillColorTable();
	printAllPixels();
	free(colors);			//gibt allokierten speicherplatz wieder frei
	colors = NULL;
	return OK; 
}

/**
  * @brief decodes the file header 
	* @param None
  * @retval 0
  */
uint32_t decodeFileHeader(void)
{
	
	ERROR_HANDLER(1!= COMread((char *) &fileHeader, sizeof(BITMAPFILEHEADER), 1), "readHeaders: Error during read.");
	basicChecks(&fileHeader, NULL);
	return OK; 
}

/**
  * @brief  decodes the info header 
  * @param  None
  * @retval 0
  */
uint32_t decodeInfoHeader(void)
{
	ERROR_HANDLER(1!= COMread((char *) &infoHeader, sizeof(BITMAPINFOHEADER), 1), "readHeaders: Error during read.");
	basicChecks(NULL, &infoHeader); 
	ERROR_HANDLER(infoHeader.biWidth > MAXWIDTH, "readHeaders: Picture too wide"); 	
	return OK; 
}

/**
  * @brief  fills the color Table with the colors 
  * @param  None
  * @retval 0
  */
uint32_t fillColorTable(void)
{
	if(BMP_UNCOMPRESSED_24BIT == infoHeader.biBitCount)
	{
		return OK; 
	}
	else 
	{
		uint32_t numColors = infoHeader.biClrUsed;		
		
		if(MAX_COLORS_USED == infoHeader.biClrUsed)
		{
			numColors = MAX_NUM_COLORS; 
		}
		
		for(int i = 0; i < numColors; ++i)
		{
			ERROR_HANDLER( 1 != COMread((char*) &colorTable[i], sizeof(RGBQUAD),1), "readTable: Error during read."); 
		}
	}
	return OK; 
}

/**
  * @brief  prints all the pixels on the LCD 
  * @param  None
  * @retval 0
  */
uint32_t printAllPixels(void)
{
	colors = (COLOR *)malloc(infoHeader.biWidth * sizeof(COLOR));
	
	if((BMP_UNCOMPRESSED_24BIT == infoHeader.biBitCount)) //holt die 3 Byte pixel 
	{
		uncompressed24bit();
	}
	else if((BMP8BIT == infoHeader.biBitCount) && (UNCOMPRESSED == infoHeader.biCompression))
	{
		uncompressed8bit();
	}
	else if((BMP8BIT == infoHeader.biBitCount) && (COMPRESSED == infoHeader.biCompression))
	{
		compressed8bit();
	}
	return OK; 
}

/**
  * @brief  prints 24 bit bmp
  * @param  None
  * @retval 0
  */
uint32_t uncompressed24bit(void)
{
	uint16_t y = LCD_HEIGHT-1; 
	uint16_t x = START_OF_LINE; 
	uint16_t color = 0;
	uint16_t totalBytesPerRow = ((((infoHeader.biWidth)*(24) + 31) / 32) * 4);

	uint32_t numPixelPerRow = infoHeader.biWidth;
		RGBTRIPLE tempPixel; 
		for(int i = 0; i < (infoHeader.biWidth * infoHeader.biHeight); ++i)
		{
			ERROR_HANDLER( 1 != COMread((char*) &tempPixel, sizeof(RGBTRIPLE),1), "getNextPixel: Error during read."); //holt einen Pixel mit Farbinfo
			toSmallRGB(tempPixel.rgbtRed,tempPixel.rgbtGreen,tempPixel.rgbtBlue,&color); //konvertiert 24bit rgb zu 16bit 
			addToArr(x, color);
			if(x + 1 == numPixelPerRow) //wenn die max zeilenlänge erreicht ist -> neue zeile
			{
				printLine(y, LCD_WIDTH, colors); // drucke zeile
						
				uint16_t bytesPerRow = (x+1)*BYTES_PER_24BIT; 
				
				while(bytesPerRow < totalBytesPerRow) //padding bytes ueberspringen 
				{
					nextChar(); 
					++bytesPerRow; 
				}
				
				x = START_OF_LINE;	//setzte x zurück  
				--y;								//gehe eine zeile weiter 
			}
			else 
			{
				++x; //gehe eine Stelle weiter 
			}
		}
	return OK; 
}

/**
  * @brief  prints uncompressed 8 bit bmp
  * @param  None
  * @retval 0
  */
uint32_t uncompressed8bit(void)
{
	uint16_t y = LCD_HEIGHT-1; 
	uint16_t x = START_OF_LINE; 
	uint16_t color = 0; 
	uint16_t totalBytesPerRow = ((((infoHeader.biWidth)*(8) + 31) / 32) * 4);
	
	RGBQUAD temPixel;
		uint32_t numPixelPerRow = infoHeader.biWidth;
		for(int i = 0; i < (infoHeader.biWidth * infoHeader.biHeight); ++i)
		{
			temPixel = colorTable[nextChar()]; //holt die farbinfo für den Pixel 
			toSmallRGB(temPixel.rgbRed, temPixel.rgbGreen, temPixel.rgbBlue, &color); //konvertiert 24bit rgb zu 16bit
			addToArr(x, color);
			if(x + 1 == numPixelPerRow) //wenn die max zeilenlänge erreicht ist -> neue zeile
			{	
				printLine(y, LCD_WIDTH, colors);
				
				uint16_t bytesPerRow = (x+1); 
				
				while(bytesPerRow < totalBytesPerRow)
				{
					nextChar(); 
					++bytesPerRow; 
				}
				
				x = START_OF_LINE; 
				--y;
			}
			else 
			{
				++x;
			}
		}
	return OK;
}

/**
  * @brief  prints compressed 8 bit bmp
  * @param  None
  * @retval 0
  */
uint32_t compressed8bit(void)
{
	int endOfBitmap = 0; 
	int endline = 0; 
	uint8_t byte = 0;
	uint16_t y = LCD_HEIGHT-1; 
	uint16_t x = START_OF_LINE; 
	RGBQUAD pixel;
	uint16_t color = 0; 
	uint16_t oldy = 0; 
	
	while(!endOfBitmap)
	{
		while(!endline)
		{
			byte = nextChar();  //erstes byte
		
			if(0 == byte)
			{
				byte = nextChar();  //zweites byte 
				
				switch(byte)
				{
					case 1 : 
						endOfBitmap = TRUE;  //00 01 -> ende des Bildes
					case 0 : 
						endline = TRUE;  		//00 00 -> ende der zeile 
						break; 
					case 2 :  
						oldy = y; 
						x += nextChar();	//00 02 -> verschiebung um die nächstes byte nach x und übernächstes nach y 
						y -= nextChar();
						
						if(oldy != y)			//Wenn eine neue zeile begonnen wurde, drucke zeile
						{
							printLine(oldy, LCD_WIDTH, colors);
						}
						break; 
					default: 						//00 03/FF -> 03/FF nächste bytes als unkomprimiert interpretieren
						for(int i = 0; i < byte; ++i)
						{
							pixel = colorTable[nextChar()]; //holt die farbinfo für den Pixel  
							toSmallRGB(pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue, &color); //konvertiert 24bit rgb zu 16bit
							addToArr(x, color); //druckt aktuellen pixel
							++x; 
						}
						
						if(0 != (byte % 2)) //handle padding bytes 
						{
							nextChar(); 
						}
					}
				}
				else // 01/FF -> 01/FF mal ein pixel mit der farbe auf die das nächste byte verweist   
				{
					pixel = colorTable[nextChar()]; //holt die farbinfo für den Pixel  
					
					for(int i = 0; i < byte; ++i)
					{
						toSmallRGB(pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue, &color); //konvertiert 24bit rgb zu 16bit
						addToArr(x, color); //druckt aktuellen pixel
						++x; 
					}
				}
		}
		printLine(y, LCD_WIDTH, colors);
		
		for(int i = 0; i < infoHeader.biWidth; ++i)
		{
			colors[i] = WHITE; 
		}
		
		--y; 
		x = START_OF_LINE; 
		endline = FALSE; 
	}
	return OK; 
}
							
/**
  * @brief  converts 24 bit rgb to 16 bit rgb
  * @param  red the red value of the 24 bit rgb
	* @param  green the green value of the 24 bit rgb
  * @param  blue the bluevalue of the 24 bit rgb
  * @param  color the the return variable 
  * @retval 0
  */
uint32_t toSmallRGB(uint8_t red, uint8_t green, uint8_t blue, uint16_t* color) //wandelt 24 bit rgb zu 16 bit rgb um 
{
	red = red >> 3; 
	green = green >> 2; 
	blue = blue >> 3; 
	
	*color = ((red << 11) | (green << 5) | blue); 
	
	return OK; 
}

/**
  * @brief  adds a color to the array 
  * @param  x the position of the color 
  * @param  color the color
  * @retval 0
  */
uint32_t addToArr(uint16_t x, COLOR color)
{
	colors [x] = color; 
	return OK; 
}