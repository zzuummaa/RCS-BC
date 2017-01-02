/*
 * mkfifo.c
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include "pipe.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

/**
 * Make a pipe with name PIPE_NAME,
 * which is specific for this module.
 *
 * return 0 - success
 * 	      1 - error
 */
int pipe_make(const char *pipe_name) {
	return mkfifo(pipe_name, 0777);
}

int pipe_remove(const char *pipe_name) {
	return remove(pipe_name);
}

/**
 * Open pipe to reading data.
 *
 * Interrupt program execution until writers not connected
 * to pipe.
 *
 * return file descriptor or -1 if file can't be opened
 */
int pipe_openReadOnly(const char *pipe_name) {
	int file_descriptor = open(pipe_name, O_RDONLY);

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
int pipe_openWriteOnly(const char *pipe_name) {
	int file_descriptor = open(pipe_name, O_WRONLY);

	return file_descriptor;
}

int pipe_close(int file_descriptor) {
	return close(file_descriptor);
}

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
