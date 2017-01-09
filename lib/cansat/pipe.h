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
#define PIPE_TELEMETRY	"/tmp/pipe_telemetry"
#define PIPE_TESTER		"/tmp/pipe_tester"

/**
 * Values of expected data type
 */
typedef enum {
	TYPE_NOTHING = 0,
	TYPE_CAMERA  = 2
} PACK_TYPE;

typedef struct {
	unsigned short type;
	unsigned char data[PIPE_DATA_SIZE];
} pipe_pack;

class mypipe {
protected:
	char name[100];
	int pack_type;
	int fd;
	void init(char* name, PACK_TYPE pack_type);
public:
	int close();
};

class pipeWriter : public mypipe {
public:
	pipeWriter(char* name) {
		init(name, TYPE_CAMERA);
	}
	pipeWriter(char* name, PACK_TYPE pack_type);
	int openPipe();
	int write_(char* buff, int size);
};

class pipeReader : public mypipe {
public:
	pipeReader(char* name) {
		init(name, TYPE_NOTHING);
	}
	pipeReader(char* name, PACK_TYPE pack_type) {
		init(name, pack_type);
	}
	int openPipe();
	int read_(char* buff, int size);
};

int pipe_make(const char *pipe_name);
int pipe_remove(const char *pipe_name);

int pipe_openReadOnly(const char *pipe_name);
int pipe_openWriteOnly(const char *pipe_name);
int pipe_close(int file_descriptor);
int pipe_read(int file_descriptor, pipe_pack* pp);
int pipe_write(int file_descriptor, const pipe_pack* pp);


#endif /* PIPE_H_ */
