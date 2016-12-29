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
 * String constants for pipe names
 */
#define TELEMETRY_PIPE "/tmp/telemetry_pipe"

/**
 * Values of expected data type
 */
#define TYPE_CAMERA 2

typedef struct {
	unsigned short type;
	unsigned char data[PIPE_DATA_SIZE];
} pipe_pack;

int pipe_make(char *pipe_name);
int pipe_remove(char *pipe_name);
int pipe_openReadOnly(char *pipe_name);
int pipe_openWriteOnly(char *pipe_name);
int pipe_close(int file_descriptor);
int pipe_read(int file_descriptor, pipe_pack* pp);
int pipe_write(int file_descriptor, const pipe_pack* pp);


#endif /* PIPE_H_ */
