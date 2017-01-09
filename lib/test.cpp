/*
 * tester_main.cpp
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: Cntgfy
 */

#include <iostream>
#include <unistd.h>
#include <fstream>

#include "cansat/camera.h"

using namespace std;

#define IMAGE_NAME "image.bmp"

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

int main() {
	printResult("Pipe", pipeTest);
	printResult("Camera", cameraTest);
	printResult("MultiPhoto", multiphotoCameraTest);
}

camera Camera;

int multiphotoCameraTest() {
	//capture

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
