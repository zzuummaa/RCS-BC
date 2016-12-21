/*
 * camera.cpp
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include "stdio.h"
#include "camera.h"

int camera_init(char* camera_name) {
	printf("camera: init with name '%s'\n", camera_name);

	return 1;
}

int camera_takePhoto(char* photo_path) {
	printf("camera: take photo with path '%s'\n", photo_path);

	return 1;
}
