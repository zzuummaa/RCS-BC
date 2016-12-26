/*
 * telemetryStructs.h
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "../include_tel/pipe.h"

typedef struct telemetry_s telemetry;
typedef struct camera_s camera;

#define TYPE_CAMERA 2

struct camera_s {
	char last_img_name[30];
};

struct telemetry_s {
	camera cam;
};

int telemetry_update(telemetry *tel, pipe_pack *pp);

#endif /* STRUCTS_H_ */
