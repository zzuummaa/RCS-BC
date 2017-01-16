/*
 * telemetry_pipe.h
 *
 *  Created on: 16 џэт. 2017 у.
 *      Author: Cntgfy
 */

#ifndef TELEMETRY_PIPE_H_
#define TELEMETRY_PIPE_H_

#include "pipe.h"

typedef struct {
	DATA_TYPE type;
	char data[PIPE_DATA_SIZE];
} pipe_pack;

class telemetryPipeWriter : public pipeWriter {
public:
	telemetryPipeWriter(char* name) {
		init(name);
	}
	int write_telemetry(char *buff, int size, DATA_TYPE type);
};

class telemetryPipeReader : public pipeReader {
public:
	telemetryPipeReader(char* name) {
		init(name);
	}
	int read_telemetry(char *buff, int size, DATA_TYPE* type);
	int read_telemetry(pipe_pack *pp);
};

int pipe_read(int file_descriptor, pipe_pack* pp);
int pipe_write(int file_descriptor, const pipe_pack* pp);

#endif /* TELEMETRY_PIPE_H_ */
