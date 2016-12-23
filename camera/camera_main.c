/*
 * camera_test.cpp
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include <unistd.h>
#include <stdio.h>

#include "src/camera.h"
#include "pipe.h"
#include "structs.h"

int main() {
	char img_path[30] = "/home/img";
	unsigned int counter = 0;

	camera_init("test name");

	if (telemetryPipe_openWriteOnly() == -1) {
		printf("Can't open pipe");
		return 1;
	}

	while (1) {
		pipe_pack pp;
		pp.type = 2;
		camera *cam = (camera*)pp.data;
		sprintf(cam->last_img_path, "%s/img%u.jpg", img_path, counter);

		if (camera_takePhoto(cam->last_img_path) == 1) {
			counter++;
			telemetryPipe_write(&pp);
		}
		sleep(2);
	}

	return 0;
}
