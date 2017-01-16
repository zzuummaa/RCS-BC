/*
 * image.h
 *
 *  Created on: 8 џэт. 2017 у.
 *      Author: Cntgfy
 */

#ifndef IMAGE_H_
#define IMAGE_H_

class image {
private:
	int size;
	char* buff;
public:
	/**
	 * ppBuff - binary image in RGB format
	 * w	  - width
	 * h	  - height
	 */
	image(unsigned char* ppmBuff, int w, int h);
	/**
	 * return image data size
	 */
	int getSize();
	/**
	 * return pointer to img data
	 */
	char* getBuff();
	/**
	 * Frees heap resources
	 */
	void release();
};


#endif /* IMAGE_H_ */
