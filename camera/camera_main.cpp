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

#include "camera.h"
#include "structs.h"
#include "filewriter.h"
#include "shm/redis.h"

#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

#define IMG_PATH "img"

const char* FILENAME_FORMAT = "%d.bmp";

/**
 * Write file name to dst
 */
void formatted_filename(char* dst, int num) {
	strcpy(dst, IMG_PATH);
	dst += sizeof(IMG_PATH);

	sprintf(dst, FILENAME_FORMAT, num);
}

int getFilenameNum(const char* formated_filename) {
	int photoNum;

	if (sscanf(formated_filename, FILENAME_FORMAT, &photoNum) == 1) {
		return photoNum;
	} else {
		return -1;
	}
}

/**
 * return 0 if nothing found
 * 		  max number of files
 */
int getMaxPhotoNum(char* dir) {
	int maxPhotoNum = 0;

	for (fs::recursive_directory_iterator it(string("./").append(dir)), end; it != end; ++it) {
		std::string fullFname = it->path().string();
		std::string fname = fullFname.substr( fullFname.rfind('/')+1 );

		int tmpNum = getFilenameNum(fname.c_str());

		if (tmpNum > maxPhotoNum) {
			maxPhotoNum = tmpNum;
		}
	}

	return maxPhotoNum;
}

int main() {
	dataService* shtel = new redisDataService();
	shtel->connect();

	filewriter fwriter;

	int photo_num = getMaxPhotoNum(IMG_PATH) + 1;
	std::cout << "Next num=" << photo_num << endl;

	camera cam;
	if ( !cam.open() ) {
		return 1;
	}

	tel_camera tcam;
	char photoName[100];

	printf("Waiting for camera 3 seconds...\n");
	sleep(3);

	while (true) {
		printf("\n");

		if ( !cam.grab() ) continue;

		image img = cam.getLastImage();

		formatted_filename(photoName, photo_num);
		fwriter.fileOpen(photoName, "w");
		fwriter.write(img.getBuff(), img.getSize());

		fwriter.fileClose();
		img.release();

		shtel->add(TYPE_CAMERA, (char*)&tcam, sizeof(tel_camera));

		sleep(2);
	}

	shtel->disconnect();
	delete shtel;

	return 0;
}
