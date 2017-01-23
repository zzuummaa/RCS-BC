/*
 * transmitter.cpp
 *
 *  Created on: 24.12.2016
 *      Author: zzuummaa
 */

#include <string.h>
#include "filewriter.h"

int filewriter::fileOpen(const char *file_name, const char* mode) {
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, file_name);

	f = fopen(fileName, mode);

	if (f == NULL) {
		printf("Error opening file '%s'\n", fileName);
		return 0;
	} else {
		printf("File '%s' opened\n", fileName);
		return 1;
	}
}

/**
 * return 1 - success write data
 * 		  0 - error write data
 */
int filewriter::write(char* buff, int size) {
	if (f == NULL) {
		printf("File not opened\n");
		return 0;
	}

	if (fwrite(buff, 1, size, f) != size) {
		printf("Error writing data to file '%s'\n", fileName);
		return 0;
	} else {
		fflush(f);
		printf("Wrote %d bytes to file '%s'\n", size, fileName);
		return 1;
	}
}

int filewriter::fileClose() {
	if (fclose(f) != 0) {
		printf("Error closing '%s'\n", fileName);
		return 0;
	} else {
		printf("File '%s' closed\n", fileName);
		return 1;
	}
}

