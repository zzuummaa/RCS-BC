#define _CRT_SECURE_NO_DEPRECATE
#pragma pack(1)

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef unsigned char BYTE;

typedef enum _ErrorID
{
	errOK,
	errParamError,
	errOpenPpmError,
	errReadPpmError,
	errOpenBmpError,
	errWriteBmpError,
	errMemoryError
} ErrorID;

/**
 * Convert ppm image to bmp image.
 * Dynamically allocates memory for bmp image.
 *
 * bmpSize - bmp image result size
 *
 * return pointer to bmp image
 */
BYTE* convert(BYTE *bufPpm, int *bmpSize);

// convert ppm file to bmp file
//ErrorID ppm2bmp(char *ppmFilename, char *bmpFilename);
