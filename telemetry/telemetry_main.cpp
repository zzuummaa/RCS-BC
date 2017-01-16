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
#include "telemetry_pipe.h"
#include "structs.h"

#define TELEMETRY_FILE "telmery.txt"

void remove_pipe(int status) {
	printf("Signal quit\n");
	pipe_remove(PIPE_TELEMETRY);
	exit(0);
}

int main() {
	signal(SIGTERM, remove_pipe);

	filewriter fw;
	fw.fileOpen(TELEMETRY_FILE, "a");

	int fd;
	pipe_make(PIPE_TELEMETRY);

	printf("Pipe waiting for writers...\n");
	telemetryPipeReader preader(PIPE_TELEMETRY);
	preader.openPipe();

	telemetry tel;

	while (1) {
		printf("\n");

		pipe_pack pp;
		pp.type = TYPE_NOTHING;
		preader.read_telemetry(&pp);

		telemetry_update(&tel, pp.data, pp.type);

		fw.write((char*)&tel, sizeof(tel));
	}

	pipe_close(fd);
	pipe_remove(PIPE_TELEMETRY);
	fw.fileClose();
	return 0;
}
