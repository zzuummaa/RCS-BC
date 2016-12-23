/*
 * telemetry.c
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include "stdio.h"
#include "include/pipe.h"
#include "include/structs.h"

int main() {
	telemetryPipe_make();
	printf("Pipe was made\n");
	telemetryPipe_openReadOnly();
	printf("Pipe opened\n");

	telemetry tel;

	while (1) {
		pipe_pack pp;
		pp.type = 0;
		if (telemetryPipe_read(&pp) == 0) continue;

		telemetry_update(&tel, &pp);
		printf("camera\t%s\n", tel.cam.last_img_path);
	}

	telemetryPipe_close();
	telemetryPipe_remove();
	return 0;
}
