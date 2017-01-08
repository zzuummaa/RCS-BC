/*
 * photo.h
 *
 *  Created on: 05.01.2017
 *      Author: zzuummaa
 */

#ifndef PHOTO_H_
#define PHOTO_H_

#include <stddef.h>

typedef struct {
	char *buff;
	int size;
} photo;

int photo_readPhoto(int fd, char* buff, int size);
int photo_countPhotoLen(unsigned char *buff, int size);


#endif /* PHOTO_H_ */
