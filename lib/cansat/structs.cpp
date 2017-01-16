/*
 * structs.c
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

#include "structs.h"

typedef void (*funct)(telemetry *tel, const char *data);

void camera_update(telemetry *tel, const char* data) {
	tel_camera *cam = (tel_camera*)(data);
	tel->cam = *cam;
}

/**
 * Add current info to telemetry pack
 * from pipe_pack
 *
 * Looking for the appropriate update procedure
 * for type pack
 */
int telemetry_update(telemetry *tel, const char *data, DATA_TYPE type) {
	funct f;

	switch (type) {
	case TYPE_CAMERA: f = &camera_update; break;
	}

	(*f)(tel, data);

	return 1;
}
