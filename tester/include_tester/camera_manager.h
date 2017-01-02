/*
 * camera_test.h
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: Cntgfy
 */

#ifndef INCLUDE_TESTER_CAMERA_MANAGER_H_
#define INCLUDE_TESTER_CAMERA_MANAGER_H_

#include "problem_solve.h"

#define CAM_PROG_PATH "/home/pi/rcs/camera"
extern char* CAM_CMD[];

typedef struct {
	//2 - starting status
	//1 - working status
	//0 - error status
	unsigned char status;
	unsigned short pid;
} camera_info;

int cameraManager_getMode(const camera_info *cam_info);

void cameraManager_updateProcInfo(const camera_info *cami, process_info *proci);

void cameraManager_initProgInfo(program_info *progi);

#endif /* INCLUDE_TESTER_CAMERA_MANAGER_H_ */
