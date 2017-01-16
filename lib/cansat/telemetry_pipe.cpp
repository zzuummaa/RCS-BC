/*
 * telemetry_pipe.cpp
 *
 *  Created on: 16 џэт. 2017 у.
 *      Author: Cntgfy
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "telemetry_pipe.h"

/*========================================================*/
/**
 *               TELEMETRY PIPE WRITER
 */
/*========================================================*/

int telemetryPipeWriter::write_telemetry(char* buff, int size, DATA_TYPE type) {
	pipe_pack pp;
	pp.type = type;

	if (size > PIPE_DATA_SIZE) {
		printf("buff size too large\n");
		return 0;
	}
	memcpy(pp.data, buff, size);

	return write_((char*)&pp, sizeof(pp));
}

/*========================================================*/
/**
 *               TELEMETRY PIPE READER
 */
/*========================================================*/

int telemetryPipeReader::read_telemetry(char *buff, int size, DATA_TYPE* type) {
	pipe_pack pp;

	int status = read_((char*)&pp, sizeof(pp));

	if (status == 1) {
		*type = pp.type;
		return 1;
	} else {
		return 0;
	}
}

int telemetryPipeReader::read_telemetry(pipe_pack *pp) {
	return read_telemetry(pp->data, PIPE_DATA_SIZE, &pp->type);
}

/*========================================================*/
/**
 *                   OTHER FUNCTIONS
 */
/*========================================================*/

/**
 * Read telemetry data
 *
 * return 1 - success readed
 * 		  0 - appeared errors
 */
int pipe_read(int file_descriptor, pipe_pack* pp) {
	char* buf = (char*) pp;

	if ( read(file_descriptor, buf, sizeof(pipe_pack)) != sizeof(pipe_pack) ) {
		return 0;
	} else {
		return 1;
	}
}

int pipe_write(int file_descriptor, const pipe_pack* pp) {
	if (file_descriptor == -1) return 1;

	char* buf = (char*) pp;

	if (write(file_descriptor, buf, sizeof(pipe_pack)) != sizeof(pipe_pack)) {
		return 0;
	} else {
		return 1;
	}
}
