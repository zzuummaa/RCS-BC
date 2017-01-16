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

/**
 * Values of expected data type
 */
typedef enum {
	TYPE_NOTHING = 0,
	TYPE_CAMERA  = 2
} DATA_TYPE;

typedef struct {
	char last_img_name[30];
} tel_camera;

/**
 * Pack of all system condition
 */
typedef struct {
	tel_camera cam;
} telemetry;

int telemetry_update(telemetry *tel, const char* data, DATA_TYPE type);
//void telemetry_

#endif /* STRUCTS_H_ */
