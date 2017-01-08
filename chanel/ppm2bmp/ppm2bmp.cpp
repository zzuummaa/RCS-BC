// Convert ppm to bmp
//
// Reference:
//
// Windows Data Types
// https://msdn.microsoft.com/zh-cn/windows/hardware/aa383751(v=vs.80)#WORD
// 
// ����ͼ����-PPMͼ���ʽ
// http://www.jianshu.com/p/e809269b4ad7
//
// Bitmap/Write a PPM file
// http://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C
//
// 24λRGB���ݱ���ΪBMPͼƬ
// http://blog.csdn.net/wesleyluo/article/details/6561518
//
// RGB����������bmp�ļ���demo
// http://blog.csdn.net/nogodoss/article/details/4183882

#include "ppm2bmp.h"
#include <string.h>

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[1];
} BITMAPINFO, *PBITMAPINFO;

typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

void realizeAll(FILE *fpPpm, FILE *fpBmp, BYTE *bufPpm, BYTE *bufBmp) {
	if (bufPpm) free(bufPpm);
	if (bufBmp) free(bufBmp);
	if (fpPpm) fclose(fpPpm);
	if (fpBmp) fclose(fpBmp);
}

/**
 * return pointer to position of title end in bufBmp
 */
BYTE* writeBmpTitle(BYTE *bmp, int w, int h, int bufBmpSize) {
	// bmp info
	BITMAPINFO bitmapinfo;
	memset(&bitmapinfo, 0, sizeof(BITMAPINFO));
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = w;
	bitmapinfo.bmiHeader.biHeight = h;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 24;
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biSizeImage = bufBmpSize;
	bitmapinfo.bmiHeader.biClrUsed = 0;
	bitmapinfo.bmiHeader.biClrImportant = 0;

	// bmp file header
	BITMAPFILEHEADER bmpHeader;
	memset(&bmpHeader, 0, sizeof(BITMAPFILEHEADER));
	bmpHeader.bfType = 0x4D42;
	bmpHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bmpHeader.bfSize = bmpHeader.bfOffBits + bufBmpSize;

	memcpy(bmp, &bmpHeader, sizeof(BITMAPFILEHEADER));
	bmp += sizeof(BITMAPFILEHEADER);

	memcpy(bmp, &(bitmapinfo.bmiHeader), sizeof(BITMAPINFOHEADER));
	bmp += sizeof(BITMAPINFOHEADER);

	return bmp;
}

BYTE* convert(BYTE *bufPpm, int *bmpSize) {
	// read ppm file header
	int w, h, c;
	sscanf((char*)bufPpm, "P6\n%d %d\n%d\n", &w, &h, &c);

	int widthStep = (((w * 24) + 31) & (~31)) / 8; // line width of buffer % 4 == 0
	int bufBmpSize = widthStep * h;
	*bmpSize = bufBmpSize + sizeof(BITMAPINFO) + sizeof(BITMAPFILEHEADER);
	BYTE *bmp = (BYTE *)malloc(*bmpSize);

	if (!bmp) return NULL;

	memset(bmp, 0, bufBmpSize);

	BYTE *bufBmp = writeBmpTitle(bmp, w, h, bufBmpSize);

	// seems ppm data is bottom to top
	BYTE *pp = bufPpm;
	BYTE *pb = bufBmp + bufBmpSize;
	int hh = h;
	while (hh > 0){
		pb -= widthStep;
		memcpy(pb, pp, w * 3);
		pp += w * 3;

		hh--;
	}

	return bmp;
}

/*ErrorID ppm2bmp(char *ppmFilename, char *bmpFilename) {
	ErrorID err = errOK;
	FILE *fpPpm = NULL;
	FILE *fpBmp = NULL;
	BYTE *bufPpm = NULL;
	BYTE *bufBmp = NULL;

	// check params
	if (!errParamError || !bmpFilename) {
		err = errParamError;
		realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);
		return err;
	}

	printf("ppm file = %s, bmp file = %s\n", ppmFilename, bmpFilename);

	//--- read ppm file ---

	// open ppm file 
	fpPpm = fopen(ppmFilename, "rb");
	if (!fpPpm){
		printf("read ppm file %s failed.\n", ppmFilename);
		err = errOpenPpmError;
		realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);
		return err;
	}

	// read ppm file header
	int w, h, c;
	(void)fscanf(fpPpm, "P6\n%d %d\n%d\n", &w, &h, &c);
	printf("ppm file %s, w=%d, h=%d, c=%d\n", ppmFilename, w, h, c);

	// only deal with 24bit ppm (each pixel cost 3 bytes)
	if (c!=0xFF){
		printf("can not handle ppm file %s.\n", ppmFilename);
		err = errOpenPpmError;
		realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);
		return err;
	}

	// read ppm file buffer
	int bufPpmSize = w * h * 3;
	bufPpm = (BYTE *)malloc(bufPpmSize);
	if (!bufPpm){
		err = errMemoryError;
		realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);
		return err;
	}
	memset(bufPpm, 0, bufPpmSize);
	if (bufPpmSize != fread(bufPpm, 1, bufPpmSize, fpPpm)){
		err = errOpenBmpError;
		realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);
		return err;
	};

	//--- convert ppm buffer to bmp buffer ---

	bufBmp = convert(bufPpm);
	if (!bufBmp){
		err = errMemoryError;
		realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);
		return err;
	}


	//--- write bmp file ---

	// bmp info
	BITMAPINFO bitmapinfo;
	memset(&bitmapinfo, 0, sizeof(BITMAPINFO));
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = w;
	bitmapinfo.bmiHeader.biHeight = h;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 24;
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biSizeImage = bufBmpSize;
	bitmapinfo.bmiHeader.biClrUsed = 0;
	bitmapinfo.bmiHeader.biClrImportant = 0;

	// bmp file header
	BITMAPFILEHEADER bmpHeader;
	memset(&bmpHeader, 0, sizeof(BITMAPFILEHEADER));
	bmpHeader.bfType = 0x4D42;
	bmpHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bmpHeader.bfSize = bmpHeader.bfOffBits + bufBmpSize;

	// open bmp file
	fpBmp = fopen(bmpFilename, "wb");
	if (!fpBmp){
		printf("bmp file %s open failed.\n", bmpFilename);
		err = errWriteBmpError;
		realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);
		return err;
	}

	// write bmp file
	fwrite(&bmpHeader, 1, sizeof(BITMAPFILEHEADER), fpBmp);
	fwrite(&(bitmapinfo.bmiHeader), 1, sizeof(BITMAPINFOHEADER), fpBmp);
	fwrite(bufBmp, 1, bufBmpSize, fpBmp);

	realizeAll(fpPpm, fpBmp, bufPpm, bufBmp);

	return err;
}*/

/*int main(int argc, char *argv[]) {
	// make sure args count ok
	if (argc < 3) {
		printf("Usage example: ppm2bmp.exe in.ppm out.bmp\n");
		return errParamError;
	}

	return ppm2bmp(argv[1], argv[2]);
}*/
