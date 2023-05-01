#include "decode.h" 
#include "BMP_types.h"
#include "input.h"
#include "output.h"
#include "basic_checks.h"
#include "errorhandler.h"
#include "MS_basetypes.h"
#include "LCD_general.h"
#include <stdio.h>

static BITMAPFILEHEADER fileHeader; 
static BITMAPINFOHEADER infoHeader; 
static RGBQUAD bitmap[256];

int toSmallRGB(uint8_t red, uint8_t green, uint8_t blue, uint16_t* color);
int decodeFileHeader(void);
int decodeInfoHeader(void);
int fillBitmap(void);
int printAllPixels(void);

int decodeAndPrint()
{
	decodeFileHeader();
	decodeInfoHeader();
	fillBitmap();
	printAllPixels();
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
	return 0; 
}

int fillBitmap(void)
{
	if(infoHeader.biBitCount == 24)
	{
		return 0; 
	}
	else 
	{
		for(int i = 0; i < infoHeader.biSizeImage; ++i)
		{
			ERROR_HANDLER( 1 != COMread((char*) &bitmap[i], sizeof(RGBQUAD),1), "readTable: Error during read."); 
		}
	}
	return 0; 
}

int printAllPixels(void)
{
	uint16_t y = 0; 
	uint16_t x = 0; 
	uint16_t color = 0; 
	
	if(infoHeader.biBitCount == 24) //holt die 3 Byte pixel 
	{
		for(int i = 0; i < infoHeader.biSizeImage; ++i)
		{
			RGBTRIPLE tempPixel; 
			ERROR_HANDLER( 1 != COMread((char*) &tempPixel, sizeof(RGBTRIPLE),1), "getNextPixel: Error during read."); //holt einen Pixel mit Farbinfo
			toSmallRGB(tempPixel.rgbtRed,tempPixel.rgbtGreen,tempPixel.rgbtBlue,&color); //konvertiert 24bit rgb zu 16bit 
			if(x == infoHeader.biWidth) //wenn die max zeilenlänge erreicht ist -> neue zeile
			{
				while(x != (((LCD_WIDTH)*(sizeof(PRGBTRIPLE)*8) +31)/32*4)) //füllt mit padding Bytes auf 
				{
					printPixel(x,y,WHITE);
					++x; 
				}
				x = 0; 
				++y;
			}
			printPixel(x,y,color); //druckt aktuellen pixel
			++x;
		}
	}
	else //holt 8 bit pixel
	{
		RGBQUAD temPixel; 
		for(int i = 0; i < infoHeader.biSizeImage; ++i)
		{
			temPixel = bitmap[nextChar()]; //holt die farbinfo für den Pixel 
			toSmallRGB(temPixel.rgbRed, temPixel.rgbGreen, temPixel.rgbBlue, &color); //konvertiert 24bit rgb zu 16bit
			if(x == infoHeader.biWidth) //wenn die max zeilenlänge erreicht ist -> neue zeile
			{
				while(x != (((LCD_WIDTH)*(sizeof(PRGBTRIPLE)*8) +31)/32*4)) //füllt mit padding Bytes auf 
				{
					printPixel(x,y,WHITE);
					++x; 
				}
				x = 0; 
				++y;
			}
			printPixel(x,y,color); //druckt aktuellen pixel
			++x;
		}
	}
	return 0; 
}

int toSmallRGB(uint8_t red, uint8_t green, uint8_t blue, uint16_t* color)
{
	red = red >> 3; 
	green = green >> 2; 
	blue = blue >> 3; 
	
	*color = ((red << 11) | (green << 5) | blue); 
	
	return 0; 
}
	