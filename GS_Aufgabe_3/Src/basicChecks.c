#include "basicChecks.h"

// Basis checks of BMF headers

void basicChecks(PBITMAPFILEHEADER  headerP, PBITMAPINFOHEADER infoP){
	if (NULL != headerP){
     ERROR_HANDLER(headerP->bfType != BMP_SIGNATURE, "basisChecks: wrong signature.");
     ERROR_HANDLER(headerP->bfOffBits >= headerP->bfSize, "basisChecks: bfOffBits points out of BMP file.");
	}
	if (NULL != infoP){		
     ERROR_HANDLER(infoP->biSize != sizeof(BITMAPINFOHEADER),"basisChecks: wrong biSize.");
     ERROR_HANDLER(infoP->biPlanes != 1, "basisChecks: wrong biPlanes (must be 1).");
     ERROR_HANDLER(infoP->biHeight < 1, "basisChecks: biHeight must be positive.");
 		 ERROR_HANDLER(infoP->biWidth < 1, "basisChecks: biWidth must be positive.");
     ERROR_HANDLER((8 != infoP->biBitCount) && (24 != infoP->biBitCount), "basisChecks:  wrong biBitCount (must be 8 or 24).");
     ERROR_HANDLER((24 == infoP->biBitCount) && (BI_RGB != infoP->biCompression), "basisChecks: 24 bit format cannot be compressed.");
     ERROR_HANDLER((BI_RLE8 != infoP->biCompression) && (BI_RGB != infoP->biCompression), "basisChecks: unexpected compression format.");
     ERROR_HANDLER(MAX_COLOR_TABLE_SIZE < infoP->biClrUsed, "basisChecks: biClrUsed out of range.\n");
     ERROR_HANDLER((24 == infoP->biBitCount) && (infoP->biClrUsed != 0), "basisChecks: biClrUsed must be 0 for 24 bit/pixel format.");
  }
}
