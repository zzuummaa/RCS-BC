/*
 * camera.cpp
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include <unistd.h>
#include <stdio.h>
#include <cstdlib>

#include "camera.h"

int camera_init(char* camera_name) {
	printf("camera: init with name '%s'\n", camera_name);
	return 1;
}

int camera_takePhoto(char* photo_name) {
	char* cmd[] = {"raspistill", "-o", photo_name, "-t", "500", "-rot", "180", (char*)0};
	if (fork() == 0) {
		execv("/usr/bin/raspistill", cmd);
		exit(0);
	}
	printf("camera: take photo with path '%s'\n", photo_name);
	return 1;
}
