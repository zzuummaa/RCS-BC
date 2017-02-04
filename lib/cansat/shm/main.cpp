//============================================================================
// Name        : shared_memory.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "data_service.h"
#include "shared_memory.h"
#include "ipcmutex.h"

#define SEMAPHORE_NAME "/my_named_semaphore"

#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 50
#define SHM_CREATE 1
#define SHM_PRINT  3
#define SHM_CLOSE  4

int example_mutex(int argc, char ** argv);
int example_sharedMemory (int argc, char ** argv);
int example_shared_telemetry(int argc, char ** argv);

/*int main(int argc, char ** argv) {
	return example_shared_telemetry(argc, argv);
}*/

int example_shared_telemetry(int argc, char ** argv) {

	if (argc < 2) {
		printf("Usage: %s <create|write|read> [other params]\n", argv[0]);
		return 0;
	}

	if (!strcmp(argv[1], "create")) {
		if ( shTelemetry_create() ) {
			printf("Shared telemetry created!\n");
		}
		return 0;
	}

	dataService shtel;
	shtel.connect();

	int type;;
	char* data;
	int status;

	if (!strcmp(argv[1], "write")) {
		type = atoi(argv[2]);
		data = argv[3];
		printf("type=%d data=%s\n", type, data);

		status = shtel.add(type, data, strlen(data)+1);
		if (status == 1) {
			printf("Write data success\n");
		}
	}

	if (!strcmp(argv[1], "read")) {
		type = atoi(argv[2]);
		printf("type=%d\n", type);

		char buff[200];
		status = shtel.get(type, buff);
		if (status == 1) {
			printf("Read data success: %s\n", buff);
		}
	}

	char* words[] = {"fuck", "suck", "duck"};

	if (!strcmp(argv[1], "trashwrite")) {
		type = atoi(argv[2]);

		int i = 0;
		while(1) {
			int status = shtel.add(type, words[i], strlen(words[i])+1 );
			if (status == 0) {
				break;
			}
			i = (i+1) % 3;
		}
	}

	if (!strcmp(argv[1], "trashread")) {
		type = atoi(argv[2]);

		while(1) {
			char buff[200];
			int status = shtel.get(type, buff);
			if (status == 0) {
				break;
			}

			status = 0;
			for (int i=0; i < 3; i++) {
				if ( !strcmp(words[i], buff) ) {
					status = 1;
					break;
				}
			}
			if (status == 0) {
				printf("Datas isn't euqals\n");
				break;
			}
		}
	}

	shtel.disconnect();

	return 0;
}

void usage(const char * s) {
    printf("Usage: %s <create|write|read|unlink> ['text']\n", s);
}

int example_sharedMemory (int argc, char ** argv) {
	int len, cmd = 0;
	char *addr;

	if ( argc < 2 ) {
		usage(argv[0]);
		return 1;
	}

	if ( (!strcmp(argv[1], "create") || !strcmp(argv[1], "write")) && (argc == 3) ) {
		len = strlen(argv[2]);
		len = (len<=SHARED_MEMORY_OBJECT_SIZE)?len:SHARED_MEMORY_OBJECT_SIZE;
		cmd = SHM_CREATE;
	} else if ( ! strcmp(argv[1], "print" ) ) {
		cmd = SHM_PRINT;
	} else if ( ! strcmp(argv[1], "unlink" ) ) {
		cmd = SHM_CLOSE;
	} else {
		usage(argv[0]);
		return 1;
	}

	sharedMemory shm(SHARED_MEMORY_OBJECT_NAME, SHARED_MEMORY_OBJECT_SIZE+1);

	if (cmd == SHM_CREATE) shm.create();
	if (cmd == SHM_PRINT ) shm.open_();
	if (cmd == SHM_CLOSE ) {
		sharedMemory_remove(SHARED_MEMORY_OBJECT_NAME);
		return 0;
	}

	addr = shm.getMem();

	switch ( cmd ) {
	case SHM_CREATE:
		memcpy(addr, argv[2], len);
		addr[len] = '\0';
		printf("Shared memory filled in. You may run '%s print' to see value.\n", argv[0]);
		break;
	case SHM_PRINT:
		printf("Got from shared memory: %s\n", addr);
		break;
	}

	shm.close_();

	return 0;
}


int example_mutex(int argc, char ** argv) {
	IPCMutex mut(SEMAPHORE_NAME);

	if (argc == 2) {
		if ( !mut.create() ) {
			return 1;
		}
		printf("Mutex created\n");

		char tmp[100];
		scanf("/n");

		mut.close();
		mut.remove();
		printf("Mutex removed\n");
	} else {
		if ( !mut.open() ) {
			return 1;
		}
		printf("Mutex opened\n");

		mut.lock();
		printf("Mutex locked\n");

		char tmp[100];
		scanf("/n");

		mut.unlock();
		printf("mutex unlcoked\n");
	}

	return 0;
}
