/*
 * transmitter.cpp
 *
 *  Created on: 24.12.2016
 *      Author: zzuummaa
 */

#include "filewriter.h"

int filewriter::fileOpen(char *file_name) {
	f = fopen(file_name, "a");

	return f == NULL ? 0 : 1;
}

int filewriter::write(telemetry *tel) {
	if (f == NULL) return 0;

	fprintf(f, "camera\t%s\n", tel->cam.last_img_name);
	fflush(f);

	return 1;
}

int filewriter::fileClose() {
	return fclose(f) == 0 ? 0 : 1;
}

