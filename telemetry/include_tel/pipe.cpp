/*
 * mkfifo.c
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */


#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "pipe.h"

#define PIPE_NAME "/tmp/telemetry_pipe"

static int file_descriptor = -1;

/**
 * Make a pipe with name PIPE_NAME,
 * which is specific for this module.
 *
 * return 0 - success
 * 	      1 - error
 */
int telemetryPipe_make() {
	return mkfifo(PIPE_NAME, 0777);
}

int telemetryPipe_remove() {
	return remove(PIPE_NAME);
}

/**
 * Open pipe to reading data.
 *
 * Interrupt program execution until writers not connected
 * to pipe.
 *
 * return file descriptor or -1 if file can't be opened
 */
int telemetryPipe_openReadOnly() {
	if (file_descriptor != -1) return -1;

	file_descriptor = open(PIPE_NAME, O_RDONLY);
	return file_descriptor;
}

/**
 * Open pipe to write data.
 *
 * Pipe must be already opened for reading when this function
 * called.
 *
 * return file descriptor or -1 if file can't be opened
 */
int telemetryPipe_openWriteOnly() {
	if (file_descriptor != -1) return -1;

	file_descriptor = open(PIPE_NAME, O_WRONLY);
	return file_descriptor;
}

int telemetryPipe_close() {
	file_descriptor = -1;

	return close(file_descriptor);
}

/**
 * Read telemetry data
 *
 * return 1 - success readed
 * 		  0 - appeared errors
 */
int telemetryPipe_read(pipe_pack* pp) {
	char* buf = (char*) pp;

	if ( (read(file_descriptor, buf, sizeof(pipe_pack))) != sizeof(pipe_pack) ) {
		return 0;
	} else {
		return 1;
	}
}

int telemetryPipe_write(const pipe_pack* pp) {
	char* buf = (char*) pp;

	if (write(file_descriptor, buf, sizeof(pipe_pack)) != sizeof(pipe_pack)) {
		return 0;
	} else {
		return 1;
	}
}
