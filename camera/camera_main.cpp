/*
 * camera_test.cpp
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "../telemetry/include_tel/pipe.h"
#include "../telemetry/include_tel/structs.h"
#include "src/camera.h"

#define IMG_PATH "img"

/**
 * Write file name to dst
 */
void formatted_filename(char* dst) {
	time_t t = time(NULL);
	tm* aTm = localtime(&t);

	sprintf(dst, "%s/img%d-%d-%d.jpg",
			IMG_PATH, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
}

int main() {
	unsigned int counter = 0;

	camera_init("test name");

	if (telemetryPipe_openWriteOnly() == -1) {
		printf("Can't open pipe\n");
		return 1;
	}

	while (1) {
		pipe_pack pp;
		pp.type = TYPE_CAMERA;
		camera *cam = (camera*)pp.data;
		formatted_filename(cam->last_img_name);

		if (camera_takePhoto(cam->last_img_name) == 1) {
			counter++;
			telemetryPipe_write(&pp);
		}
		sleep(2);
	}

	return 0;
}
