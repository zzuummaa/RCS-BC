/*
 * camera.cpp
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <sys/wait.h>

#include "camera.h"

int camera_init(char* file_name) {
	char* cmd[] = {"raspistill", "-t", "500", "-o", "-"};

	int pid = fork();
	switch (pid) {
	case -1 :
		return 0;
	case 0  :
		freopen("out.jpg", "w", stdout);
		//if (stdout == NULL) exit(1);
		execv("/usr/bin/raspistill", cmd);
	default :
		printf("camera: init success\n");
		return 1;
	}
}

void initTakingPhoto(char* photo_name) {
	char* cmd[] = {"raspistill", "-o", photo_name, "-t", "500", (char*)0};

	//printf("camera: start taking photo '%s'\n", photo_name);
	fflush (stdout);
	execv("/usr/bin/raspistill", cmd);
}

/**
 * Called at the end of the work process of shooting
 */
void initCallback(char* photo_name, take_photo_callback tpc, int status) {
	fflush(stdout);
	tpc(photo_name, status);
}

/**
 * Make photo and save to photo_name path.
 *
 * Not block program running
 *
 * When the camera has finished work will be initialized
 * callback function
 */
void camera_takePhoto(char* photo_name, take_photo_callback tpc) {

	if (fork() == 0) {
		if (fork() != 0) {
			char tmp[40];
			strcpy(tmp, photo_name); //I fucked this line and will fuck you if this line will disappear from here

			int status;
			wait(&status);

			initCallback(tmp, tpc, status);
			exit(0);
		} else {
			initTakingPhoto(photo_name);
		}
		exit(0);
	}
}
