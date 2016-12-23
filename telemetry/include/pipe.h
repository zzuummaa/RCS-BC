/*
 * telemetryPipe.h
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

#ifndef PIPE_H_
#define PIPE_H_

typedef struct pipe_pack_s pipe_pack;

#define PIPE_DATA_SIZE 50

struct pipe_pack_s {
	unsigned short type;
	unsigned char data[PIPE_DATA_SIZE];
};

int telemetryPipe_make();
int telemetryPipe_remove();
int telemetryPipe_openReadOnly();
int telemetryPipe_openWriteOnly();
int telemetryPipe_close();
int telemetryPipe_read(pipe_pack* pp);
int telemetryPipe_write(const pipe_pack* pp);


#endif /* PIPE_H_ */
