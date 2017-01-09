/*
 * transmitter.h
 *
 *  Created on: 24.12.2016
 *      Author: zzuummaa
 */

/**
 * Description:
 * This module contains functions that
 * help to save and translate telemetry.
 */
#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <stdio.h>

#define OUT_FILE_NAME "telemetry.txt"

class filewriter {
public:
	FILE *f = NULL;
	char fileName[100];

	int fileOpen(const char *file_name, const char* mode);
	int write(char* buff, int size);
	int fileClose();
	filewriter() {}
};


#endif /* FILEWRITER_H_ */
