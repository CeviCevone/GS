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

static BITMAPFILEHEADER fileHeader; 
static BITMAPINFOHEADER infoHeader; 
static RGBQUAD colorTable[256];
static COLOR *colors; 

int toSmallRGB(uint8_t red, uint8_t green, uint8_t blue, uint16_t* color);
int decodeFileHeader(void);
int decodeInfoHeader(void);
int fillColorTable(void);
int printAllPixels(void);
int compressed8bit(void);
int uncompressed8bit(void);
int uncompressed24bit(void);
int addToArr(uint16_t x, COLOR color);
	
int decodeAndPrint()
{
	decodeFileHeader();
	decodeInfoHeader();
	fillColorTable();
	printAllPixels();
	free(colors); 
	colors = NULL; 
	return 0; 
}

int decodeFileHeader(void)
{
	ERROR_HANDLER(1!= COMread((char *) &fileHeader, sizeof(BITMAPFILEHEADER), 1), "readHeaders: Error during read.");
	basicChecks(&fileHeader, NULL);
	return 0; 
}

int decodeInfoHeader(void)
{
	ERROR_HANDLER(1!= COMread((char *) &infoHeader, sizeof(BITMAPINFOHEADER), 1), "readHeaders: Error during read.");
	basicChecks(NULL, &infoHeader); 
	ERROR_HANDLER(infoHeader.biWidth > 2000, "readHeaders: Picture too wide"); 	
	return 0; 
}

int fillColorTable(void)
{
	if(24 == infoHeader.biBitCount)
	{
		return 0; 
	}
	else 
	{
		uint32_t numColors = infoHeader.biClrUsed;		
		
		if(0 == infoHeader.biClrUsed)
		{
			numColors = 256; 
		}
		
		for(int i = 0; i < numColors; ++i)
		{
			ERROR_HANDLER( 1 != COMread((char*) &colorTable[i], sizeof(RGBQUAD),1), "readTable: Error during read."); 
		}
	}
	return 0; 
}

int printAllPixels(void)
{
	colors = (COLOR *)malloc(infoHeader.biWidth * sizeof(COLOR));
	
	if((24 == infoHeader.biBitCount)) //holt die 3 Byte pixel 
	{
		uncompressed24bit();
	}
	else if((8 == infoHeader.biBitCount) && (0 == infoHeader.biCompression))
	{
		uncompressed8bit();
	}
	else if((8 == infoHeader.biBitCount) && (1 == infoHeader.biCompression))
	{
		compressed8bit();
	}
	return 0; 
}

int uncompressed24bit(void)
{
	uint16_t y = LCD_HEIGHT-1; 
	uint16_t x = 0; 
	uint16_t color = 0;

	uint32_t numPixelPerRow = infoHeader.biWidth;
		RGBTRIPLE tempPixel; 
		for(int i = 0; i < (infoHeader.biWidth * infoHeader.biHeight); ++i)
		{
			ERROR_HANDLER( 1 != COMread((char*) &tempPixel, sizeof(RGBTRIPLE),1), "getNextPixel: Error during read."); //holt einen Pixel mit Farbinfo
			toSmallRGB(tempPixel.rgbtRed,tempPixel.rgbtGreen,tempPixel.rgbtBlue,&color); //konvertiert 24bit rgb zu 16bit 
			addToArr(x, color);
			if(x + 1 == numPixelPerRow) //wenn die max zeilenlänge erreicht ist -> neue zeile
			{
				printLine(y, 480, colors); // drucke zeile
						
				uint16_t bytesPerRow = (x+1)*3; 
				
				while(bytesPerRow < ((((infoHeader.biWidth)*(24) + 31) / 32) * 4)) //padding bytes ueberspringen 
				{
					nextChar(); 
					++bytesPerRow; 
				}
				
				x = 0; 
				--y;
			}
			else 
			{
				++x;
			}
		}
	return 0; 
}

int uncompressed8bit(void)
{
	uint16_t y = LCD_HEIGHT-1; 
	uint16_t x = 0; 
	uint16_t color = 0; 
	
	RGBQUAD temPixel;
		uint32_t numPixelPerRow = infoHeader.biWidth;
		for(int i = 0; i < (infoHeader.biWidth * infoHeader.biHeight); ++i)
		{
			temPixel = colorTable[nextChar()]; //holt die farbinfo für den Pixel 
			toSmallRGB(temPixel.rgbRed, temPixel.rgbGreen, temPixel.rgbBlue, &color); //konvertiert 24bit rgb zu 16bit
			addToArr(x, color);
			if(x + 1 == numPixelPerRow) //wenn die max zeilenlänge erreicht ist -> neue zeile
			{	
				printLine(y, 480, colors);
				
				uint16_t bytesPerRow = (x+1); 
				
				while(bytesPerRow < ((((infoHeader.biWidth)*(8) + 31) / 32) * 4))
				{
					nextChar(); 
					++bytesPerRow; 
				}
				
				x = 0; 
				--y;
			}
			else 
			{
				++x;
			}
		}
	return 0;
}

int compressed8bit(void)
{
	int endOfBitmap = 0; 
	int endline = 0; 
	uint8_t byte = 0;
	uint16_t y = LCD_HEIGHT-1; 
	uint16_t x = 0; 
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
						endOfBitmap = 1;  //00 01 -> ende des Bildes
					case 0 : 
						endline = 1;  		//00 00 -> ende der zeile 
						break; 
					case 2 :  
						oldy = y; 
						x += nextChar();	//00 02 -> verschiebung um die nächstes byte nach x und übernächstes nach y 
						y -= nextChar();
						
						if(oldy != y)			//Wenn eine neue zeile begonnen wurde, 
						{
							printLine(oldy, 480, colors);
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
						
						if(0 != (byte % 2))
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
		printLine(y, 480, colors);
		
		for(int i = 0; i < infoHeader.biWidth; ++i)
		{
			colors[i] = WHITE; 
		}
		
		--y; 
		x = 0; 
		endline = 0; 
	}
	return 0; 
}
							
							
		
int toSmallRGB(uint8_t red, uint8_t green, uint8_t blue, uint16_t* color) //wandelt 24 bit rgb zu 16 bit rgb um 
{
	red = red >> 3; 
	green = green >> 2; 
	blue = blue >> 3; 
	
	*color = ((red << 11) | (green << 5) | blue); 
	
	return 0; 
}

int addToArr(uint16_t x, COLOR color)
{
	colors [x] = color; 
	return 0; 
}
	