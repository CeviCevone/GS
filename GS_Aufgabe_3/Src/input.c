#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "errorhandler.h"
#include "lcd.h"


#define HELLO_IN_CMD			'H'  // Hello cmd send from Python program until our programs answers with READY 
#define READY_OUT_CMD 		'R'  // Ready cmd send to Phyton program 
#define GO_IN_CMD   	    'G'  // This cmd will be send by Python program after it has received the Ready command.
                               // After this command has been sent, Python program opens first BITMAP files and send data

#define START_OUT_CMD 		'S'  // Ask Python program to transmit the first data package of a new fill.
                               // If all BITMAP files has been transmitted, Python program blocks until new files has been selected
#define DATA_OUT_CMD      'D'  // Ask Python program to transmit the next data package of the current file 
                               // An empty data package represents an EOF

#define BUF_SIZE   512
static char buf[BUF_SIZE];	       // data buffer
static int nextCharPos = BUF_SIZE; // next unread element in buffer (BUF_SIZE : alle elems has been consumed)
static int noElemsInBuf = 0;       // number of elems in buffer; noElemsInBuf == 0 : EOF


/**
* @brief This function clears input buffer
* @param None
* @retval None
*/
static void clearBuf(void){
   nextCharPos = BUF_SIZE;
	 noElemsInBuf = 0;
}

void initInput(void){
	char ch;
	// stdin and stdout are allready open.
	// wait for hello
	while (HELLO_IN_CMD != (char) fgetc(stdin)){};
	// got first hello
	fputc(READY_OUT_CMD, stdout);
	// Yet some other Hello cmds might be in output buffer of Python programs (e.g. due to long wait)
	do {
		 ch = (char)fgetc(stdin);
	   if((HELLO_IN_CMD != ch) && (GO_IN_CMD != ch)){
			  lcdPrintS("Unexpected input cmd received.");
		 }
	} while (GO_IN_CMD != ch);
}

/**
* @brief This function receives new data and stores them into buf.
*        It blocks until a data package has been received.
* @param openNewFile  A new file should be opened
* @retval None
*/

static void fillBuf(bool openNewFile){
	 clearBuf();
	 // Ask for data package
	 fputc(openNewFile? START_OUT_CMD : DATA_OUT_CMD, stdout);
	 // read data package
	 // read no of data that will be transmitted
	 noElemsInBuf = (((char) fgetc(stdin))& 0xff) << 8;
	 noElemsInBuf = noElemsInBuf | ((char) fgetc(stdin))& 0xff;
	 ERROR_HANDLER(noElemsInBuf > sizeof(buf), "fillBuf: To many input data.");
	 // read data
	  for(int i = 0; i < noElemsInBuf; i++){
	     buf[i] = (char) fgetc(stdin);
	 }
	 nextCharPos = 0;
}

int nextChar(void){
	 if(0 == noElemsInBuf){
		  return EOF;
	 }
	 if((noElemsInBuf == nextCharPos) || (BUF_SIZE == nextCharPos)){
		  // fetch new data
      fillBuf(false);
	 }
	 if(0 == noElemsInBuf){
      return EOF;
	 }
   return buf[nextCharPos++];
}

void openNextFile(void){
	 fillBuf(true);
}

int COMread(char* buf, unsigned int size, unsigned int count){
   for (int i = 0; i < size * count; i++){
		  int c = nextChar();
		  if (EOF == c) {
				 return EOF;
			}
	    buf[i] = (char) c;
	 }
	 return count;
}

//EOF