/*
 * telemetryStructs.h
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

/**
 * Description:
 * This module contains definitions of telemetry
 * packs and procedures for handling this packs
 */
#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "../include_tel/pipe.h"

typedef struct {
	char last_img_name[30];
} tel_camera;

/**
 * Pack of all system condition
 */
typedef struct {
	tel_camera cam;
} telemetry;

int telemetry_update(telemetry *tel, const pipe_pack *pp);

#endif /* STRUCTS_H_ */
