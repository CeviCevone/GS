/**
 ******************************************************************************
 * @file    main.c
 * @author  Cevi, Ruben
 * @version 0.1
 *
 * @date    01.05.2023
 * @brief   Programm zum einlesen und Darstellen von BMP Dateien 
 ******************************************************************************
 */

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"
#include "decode.h" 
#include "BMP_types.h"
#include "input.h"
#include "basic_checks.h"
#include "errorhandler.h"
#include "MS_basetypes.h"
#include "output.h"

#define TRUE 1
#define FALSE 0


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){
	
	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}
	
	initInput();	//stelle verbidung her 	
	
	openNextFile();  	//holt naeste bmp datei
	decodeAndPrint(); //decodiert und druckt das bild
	
	uint8_t buttonPressed = FALSE; 
	
	while(TRUE) //superloop
	{
		checkButton(&buttonPressed); //prueft, ob S7 gedrueckt wurde 
		
		if(buttonPressed) //wenn S7 gedrueckt -> 
		{
			resetLCD(); 			//setzt bildschirm zurück 
			openNextFile(); 	//holt naeste bmp datei
			decodeAndPrint(); //decodiert und druckt das bild
		}
	}
	
}
	
// EOF
