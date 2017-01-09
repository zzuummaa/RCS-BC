/*
 * structs.c
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

#include "../include_tel/structs.h"

typedef void (*funct)(telemetry *tel, const pipe_pack *pp);

void camera_update(telemetry *tel, const pipe_pack *pp) {
	tel_camera *cam = (tel_camera*)(pp->data);
	tel->cam = *cam;
}

/**
 * Add current info to telemetry pack
 * from pipe_pack
 *
 * Looking for the appropriate update procedure
 * for type pack
 */
int telemetry_update(telemetry *tel, const pipe_pack *pp) {
	funct f;

	switch (pp->type) {
	case TYPE_CAMERA: f = &camera_update; break;
	}

	(*f)(tel, pp);

	return 1;
}
