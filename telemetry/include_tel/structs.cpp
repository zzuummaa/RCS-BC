/*
 * structs.c
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

#include "../include_tel/structs.h"

typedef void (*funct)(telemetry *tel, pipe_pack *pp);

void camera_update(telemetry *tel, pipe_pack *pp) {
	camera *cam = (camera*)(pp->data);
	tel->cam = *cam;
}

int telemetry_update(telemetry *tel, pipe_pack *pp) {
	funct f;

	switch (pp->type) {
	case TYPE_CAMERA: f = &camera_update; break;
	}

	(*f)(tel, pp);

	return 1;
}
