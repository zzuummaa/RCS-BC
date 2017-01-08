/*
 * camera.cpp
 *
 *  Created on: 8 џэт. 2017 у.
 *      Author: Cntgfy
 */

#include "camera.h"

image camera::getLastImage() {
	int dataSize = this->getImageTypeSize( raspicam::RASPICAM_FORMAT_RGB );
	unsigned char *data=new unsigned char[dataSize];

	this->retrieve(data, raspicam::RASPICAM_FORMAT_RGB);

	image image(data, getWidth(), getHeight());
	return image;
}
