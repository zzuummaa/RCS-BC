/*
 * tester_main.cpp
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: Cntgfy
 */

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string.h>

#include "cansat/camera.h"
#include "cansat/shm/data_service.h"

using namespace std;

#define IMAGE_NAME "image.bmp"

/**
 * Function for testing
 *
 * return 1 - test success
 * 		  0 - test failure
 */
typedef int (*function)();

void printResult(const char* fName, function funct) {
	printf("===========================\n");
	printf("==Start '%s'==\n", fName);
	printf("===========================\n");

	const char* resFormat;
	if (funct() == 1) {
		resFormat = "==Success '%s'==\n";
	} else {
		resFormat = "==Failure '%s'==\n";
	}

	printf("===========================\n");
	printf(resFormat, fName);
	printf("===========================\n\n");
}

int pipeTest();
int cameraTest();
int multiphotoCameraTest();
int shTelemetryTest();

int main() {
	printResult("Pipe", pipeTest);
	printResult("Camera", cameraTest);
	printResult("MultiPhoto", multiphotoCameraTest);
	printResult("Shared telemetry", shTelemetryTest);
}

int shTelemetryTest() {
	if ( !shTelemetry_create() ) {
		return 0;
	}

	dataService shtel;
	if ( !shtel.connect() ) {
		return 0;
	}

	for (int i=0 ; i < 2; i++) {
		printf("Writing entry %d\n", i+1);

		int type = i+1;
		int buffSize = 100;
		char buff1[buffSize];
		char buff2[buffSize];

		if ( !shtel.add(type, buff1, buffSize)
		  || !shtel.get(type, buff2) ) {
			return 0;
		}

		if ( memcmp(buff1, buff2, buffSize) != 0 ) {
			printf("Incorrect entry in the memory\n");
			return 0;
		}
	}

	return 1;
}

camera Camera;

int multiphotoCameraTest() {
	//capture

	if ( !Camera.isOpened() ) {
		printf("camera isn't opened\n");
		return 0;
	}

	int photoCount = 100;

	cout<<"Start making "<<photoCount<<" photos"<<endl;

	streambuf *bak;
	ofstream file;

	bak = cerr.rdbuf();
	cerr.rdbuf(file.rdbuf());

	int i;
	for (i = 0; i < photoCount; i++) {
		Camera.grab();
		image img = Camera.getLastImage();
		img.release();
		usleep(30 * 1000);
	}

	cerr.rdbuf(bak);

	cout<<"Made "<<i<<" photos"<<endl;

	//free resrources
	Camera.release();

	return 1;
}

int cameraTest() {
	//Open camera

	cout<<"Opening Camera..."<<endl;

	if ( !Camera.isOpened() ) {
		if ( !Camera.open()) {
			cerr<<"Error opening camera"<<endl;
			return 0;
		}
	}

	//wait a while until camera stabilizes

	cout<<"Sleeping for 3 secs"<<endl;

	sleep(3);

	//capture

	Camera.grab();

	image img = Camera.getLastImage();

	//save

	std::ofstream outFile ( IMAGE_NAME,std::ios::binary );

	outFile.write ( img.getBuff(), img.getSize() );

	cout<<"Image saved at "<<IMAGE_NAME<<endl;

	//free resrources

	img.release();
	Camera.release();
	sleep(1);

	return 1;
}

int pipeTest() {
	return 1;
}
