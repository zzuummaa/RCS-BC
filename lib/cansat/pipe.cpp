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

/*========================================================*/
/**
 *                    PIPE
 */
/*========================================================*/

void mypipe::init(char* name, PACK_TYPE pack_type) {
	this->fd = -1;
	this->pack_type = pack_type;
	strcpy(this->name, name);
}

int mypipe::close() {
	return pipe_close(fd);
}
/*========================================================*/
/**
 *                PIPE WRITER
 */
/*========================================================*/

pipeWriter::pipeWriter(char* name, PACK_TYPE pack_type) {
	init(name, pack_type);
}

int pipeWriter::openPipe() {
	int fd = open(name, O_WRONLY);

	if (fd == -1) {
		printf("Can't open to write pipe '%s'\n", name);
		return 0;
	} else {
		printf("Pipe '%s' opened to write\n", name);
		return 1;
	}
}

int pipeWriter::write_(char* buff, int size) {
	if (write(fd, buff, size) != size) {
		printf("Error writing data to pipe '%s'\n", name);
		return 0;
	} else {
		return 1;
	}
}

/*========================================================*/
/**
 *                  PIPE READER
 */
/*========================================================*/

int pipeReader::openPipe() {
	int fd = open(name, O_WRONLY);

	if (fd == -1) {
		printf("Can't open to read pipe '%s'\n", name);
		return 0;
	} else {
		printf("Pipe '%s' opened to read\n", name);
		return 1;
	}
}

int pipeReader::read_(char* buff, int size) {
	if ( read(fd, buff, sizeof(pipe_pack)) != sizeof(pipe_pack) ) {
		return 0;
	} else {
		return 1;
	}
}

/*========================================================*/
/**
 *                  Other functions
 */
/*========================================================*/

/**
 * Make a pipe with name PIPE_NAME,
 * which is specific for this module.
 *
 * return 1 - success
 * 	      0 - error
 */
int pipe_make(const char *pipe_name) {
	int status = mkfifo(pipe_name, 0777);
	if (status == -1) {
		printf("Can't make pipe '%s'\n", pipe_name);
		return 0;
	} else {
		printf("Pipe made '%s'\n", pipe_name);
		return 1;
	}
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
	int fd = open(pipe_name, O_WRONLY);

	if (fd == -1) {
		printf("Can't open to write pipe '%s'\n", pipe_name);
		return 0;
	} else {
		printf("Pipe '%s' opened to write\n", pipe_name);
		return 1;
	}
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
