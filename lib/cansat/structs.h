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
	TYPE_NOTHING	= 0,
	TYPE_CAMERA		= 2,
	TYPE_KK			= 3,
	TYPE_BAROMETER	= 4,
	TYPE_TERMO		= 5,
	TYPE_GPS_NAV	= 6
} DATA_TYPE;

typedef struct {
	float latitude;
	float longitude;
	float speed;
	bool isValid;
} tel_GPSNavigation;

typedef struct {
	float sens_temp[2];
} tel_termo;

typedef struct {
	float pressure;
	float altitude;
} tel_barometer;

typedef struct {
	int photo_num;
} tel_camera;

typedef struct {
	int impulsPerSec;
} tel_kk;

/**
 * Deprecated
 *
 * Pack of all system condition
 */
typedef struct {
	tel_camera cam;
} telemetry;

int telemetry_update(telemetry *tel, const char* data, DATA_TYPE type);
//void telemetry_

#endif /* STRUCTS_H_ */
