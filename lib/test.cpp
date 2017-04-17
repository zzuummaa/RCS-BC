/*
 * tester_main.cpp
 *
 *  Created on: 30 ���. 2016 �.
 *      Author: Cntgfy
 */

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string.h>

#include "cansat/camera.h"
#include "cansat/shm/data_service.h"
#include "cansat/shm/redis.h"
#include "cansat/validator.h"

using namespace std;

#define IMAGE_NAME "image.bmp"

/**
 * Function for testing
 *
 * return 1 - test success
 * 		  0 - test failure
 */
typedef int (*functi)(void);

void printResult(const char* fName, functi funct) {
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
int redisDataServiceTest();
int validatorTest();

int main() {
	printResult("Pipe", pipeTest);
	printResult("Camera", cameraTest);
	printResult("MultiPhoto", multiphotoCameraTest);
	//printResult("Shared telemetry", shTelemetryTest);
	printResult("Redis data service", redisDataServiceTest);
	printResult("Validator", validatorTest);
}

int validatorTest() {
	printf("Initialize validator\n");
	validator valid;
	valid.addType(0);
	valid.getWithType(0)->setFieldDiapason("test", 0, 10);

	printf("Validate values\n");
	if (!valid.getWithType(0)->checkFieldDiapason("test", 1)) {
		printf("Incorrect behaviour with a value within the range\n");
		return 0;
	}

	if (valid.getWithType(0)->checkFieldDiapason("test", 11)) {
		printf("Incorrect behaviour with a value outside the range\n");
		return 0;
	}

	printf("Validate time\n");
	valid.getWithType(0)->setMaxWaitTime(5);
	valid.getWithType(0)->checkWaitTime();
	usleep(2000);
	if (!valid.getWithType(0)->checkWaitTime()) {
		printf("Incorrect behaviour with time < max valid timeout\n");
		return 0;
	}

	usleep(7000);
	if (valid.getWithType(0)->checkWaitTime()) {
		printf("Incorrect behaviour with time > max valid timeout\n");
		return 0;
	}

	printf("Destroying validator\n");

	return 1;
}

int shTelemetryTest() {
	if ( !shTelemetry_create() ) {
		return 0;
	}

	IPCDataService shtel;
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

int redisDataServiceTest() {
	cout << "Creating service" << endl;

	redisDataService dserv;
	dserv.connect();

	char buff[] = {'F', 'e', 'c'};

	cout << "Add '" << string(buff, sizeof(buff)) << "' to service" << endl;

	if ( !dserv.add(0, buff, sizeof(buff)) ) {
		cout << "Adding failure" << endl;
		return 0;
	}

	cout << "Reading '" << string(buff, sizeof(buff)) << "' from service" << endl;

	char outKey[KEY_SIZE];
	char outBuff[sizeof(buff)];

	if ( !dserv.getLast(0, outKey, outBuff) ) {
		cout << "Reading failure" << endl;
		return 0;
	}

	if ( memcmp(buff, outBuff, sizeof(buff)) == 0) {
		cout << "Data is equals" << endl;
		return 1;
	} else {
		cout << "Data isn't equals" << endl;
		cout << "Out buffer: '" << string(outBuff, sizeof(outBuff)) << "'" << endl;
		return 0;
	}

}
