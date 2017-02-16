/*
 * telemetry.cpp
 *
 *  Created on: 10 февр. 2017 г.
 *      Author: Cntgfy
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "shm/data_service.h"

int main() {
	IPCDataService* shtel = new IPCDataService();
	if ( !shtel->connect() ) {
		printf("Can't connect to IPC data service. Trying to create new service...\n");
		if ( !shtel->create() ) {
			printf("Create IPC data service failure\n");
			return 1;
		}
	}

	DBMDataService* gdbm = new DBMDataService();
	if ( !gdbm->connect() ) {
		printf("Can't connect to data base. Trying to create new service...\n");
		if ( !gdbm->create() ) {
			printf("Create data base failure\n");
			return 1;
		}
	}

	parser* pars = shtel->getParser();

	printf("Telemetry initilize success\n");

	printf("\n");
	char buff[DEFAULT_MEM_SIZE];
	while (1) {

		int count = shtel->getBuff(buff, DEFAULT_MEM_SIZE);
		if (count <= 12) continue;

		gdbm->add(time(0), buff, count);
		shtel->clear();

		printf("Saved %d bytes\n", count);
		sleep(1);
	}

	shtel->disconnect();
	gdbm->disconnect();

	delete shtel;
	delete gdbm;

	return 0;
}

