/*
 * telemetryPipe.h
 *
 *  Created on: 23.12.2016
 *      Author: zzuummaa
 */

#ifndef PIPE_H_
#define PIPE_H_

#include "structs.h"

#define PIPE_DATA_SIZE 50

/**
 * String constants for pipe names
 */
#define PIPE_TELEMETRY	"/tmp/pipe_telemetry"
#define PIPE_TESTER		"/tmp/pipe_tester"

class mypipe {
protected:
	char name[100];
	int fd;
	void init(char* name);
public:
	int close();
};

class pipeWriter : public mypipe {
protected:
	pipeWriter() {}
public:
	pipeWriter(char* name) {
		init(name);
	}
	int openPipe();
	int write_(char* buff, int size);
};

class pipeReader : public mypipe {
protected:
	pipeReader() {}
public:
	pipeReader(char* name) {
		init(name);
	}
	int openPipe();
	int read_(char* buff, int size);
};

int pipe_make(const char *pipe_name);
int pipe_remove(const char *pipe_name);

int pipe_openReadOnly(const char *pipe_name);
int pipe_openWriteOnly(const char *pipe_name);
int pipe_close(int file_descriptor);


#endif /* PIPE_H_ */
