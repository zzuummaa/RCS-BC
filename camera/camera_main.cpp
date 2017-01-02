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

#include "pipe.h"
#include "structs.h"
#include "camera_manager.h"
#include "src/camera.h"

#define IMG_PATH "img"

int telfd, testfd, pid;

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
	if ( GET_EXIT_STATUS(status) != 1 ) {
		printf("camera: take photo failed\n");
		return;
	}

	pipe_pack telpp;
	telpp.type = TYPE_CAMERA;

	camera *cam = (camera*)telpp.data;
	strcpy(cam->last_img_name, photo_name);

	printf("camera: take photo with status=%d path '%s'\n", status, photo_name);
	pipe_write(telfd, &telpp);

	pipe_pack testpp;
	testpp.type = TYPE_CAMERA;

	camera_info *cami = (camera_info*)testpp.data;
	cami->pid = pid;
	cami->status = status == 0 ? 1 : 0;

	pipe_write(testfd, &testpp);
}

int main() {
	camera_init();
	pid = getpid();

	telfd = pipe_openWriteOnly(PIPE_TELEMETRY);
	if (telfd == -1) {
		printf("Can't open telemetry pipe\n");
	}

	testfd = pipe_openWriteOnly(PIPE_TESTER);
	if (testfd == -1) {
		printf("Can't open tester pipe\n");
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
