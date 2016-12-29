/*
 * telemetry.c
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include "stdio.h"

#include "filewriter.h"
#include "include_tel/pipe.h"
#include "include_tel/structs.h"

int main() {
	int fd;
	pipe_make(TELEMETRY_PIPE);
	printf("Pipe waiting for writers...\n");
	fd = pipe_openReadOnly(TELEMETRY_PIPE);
	printf("Pipe opened\n");

	filewriter fw;

	if (fw.f != NULL) {
		printf("File opened\n");
	} else {
		printf("File can't be opened\n");
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
	pipe_remove(TELEMETRY_PIPE);
	fw.fileClose();
	return 0;
}
