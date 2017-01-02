/*
 * camera_manager.cpp
 *
 *  Created on: 31 дек. 2016 г.
 *      Author: Cntgfy
 */


#include "camera_manager.h"
#include "pipe.h"

char* CAM_CMD[] = {"camera"};

void cameraManager_initProgInfo(program_info *progi) {
	progi->path = CAM_PROG_PATH;
	progi->cmd = CAM_CMD;
}

int cameraManager_getMode(const camera_info *cami) {
	if (cami->status != 1) {
		return MODE_ERROR;
	}

	return MODE_NORMAL;
}

void cameraManager_updateProcInfo(const camera_info *cami, process_info *proci) {
	proci->pid = cami->pid;

	int newMode = cameraManager_getMode(cami);
	probSolv_updateMode(proci, newMode);
}
