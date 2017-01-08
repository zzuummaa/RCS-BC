/*
 * tester_main.cpp
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: Cntgfy
 */

#include <iostream>
#include <unistd.h>
#include <fstream>

#include "camera.h"

using namespace std;

#define IMAGE_NAME "image.bmp"

int main() {
	camera Camera;

	//Open camera

	cout<<"Opening Camera..."<<endl;

	if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}

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

	return 0;
}
