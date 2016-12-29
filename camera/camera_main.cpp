/*
 * camera_test.cpp
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#include "../telemetry/include_tel/pipe.h"
#include "../telemetry/include_tel/structs.h"
#include "src/camera.h"

#define IMG_PATH "img"

int fd;

/**
 * Write file name to dst
 */
void formatted_filename(char* dst) {
	time_t t = time(NULL);
	tm* aTm = localtime(&t);

	sprintf(dst, "%s/img%d-%d-%d.jpg",
			IMG_PATH, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
}

void camera_callback(char *photo_name, int status) {
	if ( WIFEXITED(status) != 1 ) {
		printf("camera: take photo failed\n");
		return;
	}

	pipe_pack pp;
	pp.type = TYPE_CAMERA;

	camera *cam = (camera*)pp.data;
	strcpy(cam->last_img_name, photo_name);

	printf("camera: take photo with path '%s'\n", photo_name);
	pipe_write(fd, &pp);
}

int main() {
	camera_init();

	fd = pipe_openWriteOnly(TELEMETRY_PIPE);
	if (fd == -1) {
		printf("Can't open pipe\n");
		return 1;
	}

	char photo_name[40];
	while (1) {
		memset(photo_name, 0, sizeof(photo_name));
		formatted_filename(photo_name);
		camera_takePhoto(photo_name, &camera_callback);
		sleep(2);
	}

	return 0;
}
