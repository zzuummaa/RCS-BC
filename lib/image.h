/*
 * image.h
 *
 *  Created on: 8 џэт. 2017 у.
 *      Author: Cntgfy
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "raspicam/raspicam.h"

class image {
private:
	int size;
	char* buff;
public:
	image(unsigned char* ppmBuff, int w, int h);
	int getSize();
	char* getBuff();
	~image();
};


#endif /* IMAGE_H_ */
