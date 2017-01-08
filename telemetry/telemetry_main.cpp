/*
 * telemetry.c
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include "stdio.h"
#include <signal.h>
#include <stdlib.h>

#include "filewriter.h"
#include "include_tel/pipe.h"
#include "include_tel/structs.h"

void remove_pipe(int status) {
	printf("Signal quit\n");
	pipe_remove(PIPE_TELEMETRY);
	exit(0);
}

int main() {
	signal(SIGTERM, remove_pipe);

	filewriter fw;

	if (fw.f != NULL) {
		printf("File opened\n");
	} else {
		printf("File can't be opened\n");
		return 1;
	}

	int fd;
	pipe_make(PIPE_TELEMETRY);
	printf("Pipe waiting for writers...\n");
	fd = pipe_openReadOnly(PIPE_TELEMETRY);
	if (fd != -1) {
		printf("Pipe opened\n");
	} else {
		printf("Pipe error opening\n");
		return 1;
	}

	telemetry tel;

	while (1) {
		pipe_pack pp;
		pp.type = 0;
		if (pipe_read(fd, &pp) == 0) continue;
		telemetry_update(&tel, &pp);
		fw.write(&tel);
		printf("camera\t%s\n", tel.cam.last_img_name);
	}

	pipe_close(fd);
	pipe_remove(PIPE_TELEMETRY);
	fw.fileClose();
	return 0;
}
