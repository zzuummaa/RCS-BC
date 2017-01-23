/*
 * telemetry.c
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "filewriter.h"
#include "telemetry_pipe.h"
#include "structs.h"
#include "shm/shared_telemetry.h"

#define TELEMETRY_FILE "telmery.txt"

int main() {
	if ( !shTelemetry_create() ) {
		return 1;
	}

	shTelemetry shtel;
	if ( !shtel.connect() ) {
		return 1;
	}

	printf("Telemetry initilize success\n");

	filewriter fw;
	fw.fileOpen(TELEMETRY_FILE, "a");

	printf("\n");
	char buff[DEFAULT_MEM_SIZE+2];
	while (1) {

		int count = shtel.get(buff, DEFAULT_MEM_SIZE);
		if (count < 1) continue;
		buff[count+1] = '\r';
		buff[count+2] = '\n';

		fw.write(buff, count+2);

		sleep(1);
		printf("\n");
	}

	fw.fileClose();
	return 0;
}
