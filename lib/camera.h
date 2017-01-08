/*
 * raspicam_wraper.h
 *
 *  Created on: 8 џэт. 2017 у.
 *      Author: Cntgfy
 */

/**
 * Description:
 *
 * This wrapper tools to work with the camera
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "raspicam/raspicam.h"
#include "image.h"

class camera : public raspicam::RaspiCam {
public:
	image getLastImage();
};



#endif /* CAMERA_H_ */
