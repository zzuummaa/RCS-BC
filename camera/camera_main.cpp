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

#include "camera_manager.h"

#include "pipe.h"
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

/*void camera_callback(char *photo_name, int status) {
	if ( status != 0 ) {
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
}*/

int main() {
	pipeWriter pwriter(PIPE_TELEMETRY, TYPE_CAMERA);
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

		sleep(2);
	}

	/*pipe_make(PIPE_CAM_IMAGES);
	int res = camera_init(PIPE_CAM_IMAGES);
	imgfd = pipe_openReadOnly(PIPE_CAM_IMAGES);

	pid = getpid();

	telfd = pipe_openWriteOnly(PIPE_TELEMETRY);
	if (telfd == -1) {
		printf("Can't open telemetry pipe\n");
	}

	testfd = pipe_openWriteOnly(PIPE_TESTER);
	if (testfd == -1) {
		printf("Can't open tester pipe\n");
	}

	int SIZE = 0xffffff;
	char *imgbuff = (char*)malloc(SIZE);
	char photo_name[40];
	while (1) {
		int size = photo_readPhoto(imgfd, imgbuff, SIZE);
		if (size == -1) {
			printf("Error reading image\n");
			return 1;
		}
		if (size == 0) continue;

		memset(photo_name, 0, sizeof(photo_name));
		formatted_filename(photo_name);

		int bytes = save_image("img.jpg", imgbuff, size);
		if (bytes != -1) {
			printf("Saved %d bytes\n", bytes);
		} else {
			printf("Can't save image to file '%s'\n", photo_name);
		}

		//camera_takePhoto(photo_name, &camera_callback);
	}*/

	return 0;
}
