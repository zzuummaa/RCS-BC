/*
 * image.cpp
 *
 *  Created on: 8 џэт. 2017 у.
 *      Author: Cntgfy
 */

#include "image.h"
#include "ppm2bmp/ppm2bmp.h"

image::image(unsigned char* ppmBuff, int w, int h) {
	buff = (char*)convert(ppmBuff, &size, w, h);
}

int image::getSize() {
	return size;
}

char* image::getBuff() {
	return buff;
}

image::~image() {
	free(buff);
	buff = NULL;
}
