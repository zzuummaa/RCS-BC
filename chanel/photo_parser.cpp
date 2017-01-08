/*
 * photo.cpp
 *
 *  Created on: 05.01.2017
 *      Author: zzuummaa
 */

#include <unistd.h>

#include "photo_parser.h"

/**
 * Check bytes. If eof values return 1 else 0
 */
int EOF(unsigned char b1, unsigned char b2) {
	return (b1 == 0xff) && (b2 == 0xD9);
}

/**
 * Check bytes. If image begin values return 1 else 0
 */
int BEGIN(unsigned char b1, unsigned char b2) {
	return (b1 == 0xff) && (b2 == 0xD8);
}

int SOS(unsigned char b1, unsigned char b2) {
	return (b1 == 0xff) && (b2 == 0xDA);
}

/**
 * Read new byte from stream and put it's byte
 * into end of array. First array value will lost.
 *
 * return 1 - success read
 * 		  0 - error read
 */
int readNewPair(int fd, char b[2]) {
	char tmp;
	int status = 0;

	while (status == 0) {
		status = read(fd, &tmp, 1);
	}

	if (status == -1) return 0;

	b[0] = b[1];
	b[1] = tmp;
	return 1;
}

/**
 * Read bytes from stream until it hits begin bytes.
 * After that read bytes to buffer until hits end bytes.
 *
 * return photo size in bytes
 * 		  -1 if can't read photo
 */
int photo_readPhoto(int fd, char* buff, int size) {
	if (size < 2) return -1;

	//Search begin bytes
	char b[2] = {0x0, 0x0};
	do {
		if ( !readNewPair(fd, b) ) {
			return -1;
		}
	} while (!BEGIN(b[0], b[1]));

	buff[0] = b[0];
	buff[1] = b[1];
	buff   += 2;

	int count = 2;
	do {
		if ( !readNewPair(fd, b) ) {
			return -1;
		}
		buff[0] = b[1];

		count++;
		buff++;

		if (count+2 > size) return -1;
	} while (!EOF(b[0], b[1]));

	return count;
}

/**
 * Search begin of image
 *
 * return pointer to begin
 * 	   or NULL if not found
 */
unsigned char* photo_searchBegin(unsigned char *buff, int size) {
	int count = 0;
	while (count-2 < size) {
		if ( BEGIN(buff[0], buff[1]) ) {
			return buff;
		} else {
			count++;
			buff++;
		}
	}

	return NULL;
}

/**
 * Search end of image
 */
int photo_countPhotoLen(unsigned char *buff, int size) {
	if (size < 6) return 0;

	int count = 0;

	/*if ( BEGIN(buff[0], buff[1]) ) {
		buff += 2;
		count+= 2;
	} else {
		return 0;
	}

	while ( !EOF(buff[0], buff[1]) ) {
		int len = ( (int)buff[2] << 8 ) + buff[3];
		len += 2;
		if (size - count < len) return 0;

		buff += len;
		count+= len;

		if ( SOS(buff[0], buff[1]) ) break;
	}*/

	while ( !EOF(buff[0], buff[1]) ) {
		buff++; count++;
		if (count+2 > size) return 0;
	}

	return count+2;
}
