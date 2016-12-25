/*
 * telemetry.c
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include "stdio.h"

#include "filewriter.h"
#include "include/pipe.h"
#include "include/structs.h"

int main() {
	telemetryPipe_make();
	printf("Pipe waiting for writers...\n");
	telemetryPipe_openReadOnly();
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
		if (telemetryPipe_read(&pp) == 0) continue;
		telemetry_update(&tel, &pp);
		fw.write(&tel);
		printf("camera\t%s\n", tel.cam.last_img_name);
	}

	telemetryPipe_close();
	telemetryPipe_remove();
	fw.fileClose();
	return 0;
}
