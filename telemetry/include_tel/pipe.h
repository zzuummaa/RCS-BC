/*
 * telemetryPipe.h
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

#ifndef PIPE_H_
#define PIPE_H_

#define PIPE_DATA_SIZE 50

/**
 * Values of expected data type
 */
#define TYPE_CAMERA 2

typedef struct {
	unsigned short type;
	unsigned char data[PIPE_DATA_SIZE];
} pipe_pack;

int telemetryPipe_make();
int telemetryPipe_remove();
int telemetryPipe_openReadOnly();
int telemetryPipe_openWriteOnly();
int telemetryPipe_close();
int telemetryPipe_read(pipe_pack* pp);
int telemetryPipe_write(const pipe_pack* pp);


#endif /* PIPE_H_ */
