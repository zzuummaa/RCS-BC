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

#include "include_tel/structs.h"

#define OUT_FILE_NAME "/home/telemetry.txt"

class filewriter {
public:
	FILE *f = NULL;

	int fileOpen(char *file_name);
	int write(telemetry *tel);
	int fileClose();
	filewriter() {
		fileOpen(OUT_FILE_NAME);
	}
	filewriter(char *file_name) {
		fileOpen(file_name);
	}
};


#endif /* FILEWRITER_H_ */
