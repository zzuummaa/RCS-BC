/*
 * camera_test.cpp
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#include "telemetry_pipe.h"
#include "filewriter.h"
#include "camera.h"

#define IMG_PATH "img"

int telfd;
int testfd;
int imgfd;
int pid;

/**
 * Write file name to dst
 */
void formatted_filename(char* dst) {
	time_t t = time(NULL);
	tm* aTm = localtime(&t);

	sprintf(dst, "%s/img%d-%d-%d.bmp",
			IMG_PATH, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
}

int main() {
	telemetryPipeWriter pwriter(PIPE_TELEMETRY);
	pwriter.openPipe();

	filewriter fwriter;

	camera cam;
	if ( !cam.open() ) {
		return 1;
	}

	printf("Waiting for camera 3 seconds...\n");
	sleep(3);

	char fileName[100];
	while (true) {
		printf("\n");

		cam.grab();

		image img = cam.getLastImage();

		formatted_filename(fileName);
		fwriter.fileOpen(fileName, "w");
		fwriter.write(img.getBuff(), img.getSize());

		fwriter.fileClose();
		img.release();

		pwriter.write_telemetry(fileName, strlen(fileName), TYPE_CAMERA);

		sleep(2);
	}

	return 0;
}
