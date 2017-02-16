//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "dserv/data_service.h"
#include "dserv/shared_memory.h"
#include "dserv/ipcmutex.h"
#include "dserv/redis.h"

#define SEMAPHORE_NAME "/my_named_semaphore"

#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 50
#define SHM_CREATE 1
#define SHM_PRINT  3
#define SHM_CLOSE  4

int example_mutex(int argc, char ** argv);
int example_sharedMemory (int argc, char ** argv);
int example_data_services(int argc, char ** argv);
int example_redis_data_service(int argc, char** argv);

int main(int argc, char ** argv) {
	return example_redis_data_service(argc, argv);
}


int example_redis_data_service(int argc, char** argv) {
	redisDataService* dserv = new redisDataService();

	char str[] = "Fuck eah!";
	char buff[50];
	int type = 1;

	dserv->connect();
	if ( !dserv->add(type, str, sizeof(str) - 1) ) {
		return 1;
	}

	if ( !dserv->get(type, time(0), buff) ) {
		return 1;
	}

	if ( !memcmp(str, buff, sizeof(str) - 1) ) {
		printf("Wrote and red data is equals\n");
	} else {
		printf("Wrote and red data isn't equals\n");
	}

	dserv->disconnect();

	printf("All done\n");

	return 0;
}

int example_data_services(int argc, char ** argv) {

	if (argc < 3) {
		printf("Usage: %s <f|s|d> <create|write|read|remove> [other params]\n", argv[0]);
		return 0;
	}

	memType memtype;
	if (!strcmp(argv[1], "f"  )) memtype = MEM_FILE;
	if (!strcmp(argv[1], "s")) memtype = MEM_SHARED;

	dataService* shtel;

	if (!strcmp(argv[1], "d")) {
		shtel = new DBMDataService();
	} else {
		shtel = new IPCDataService(memtype);
	}

	if (!strcmp(argv[2], "remove")) {
		if ( shtel->remove() ) {
			printf("Remove success\n");
			return 0;
		} else {
			printf("Remove failure\n");
			return 1;
		}
	}

	if (!strcmp(argv[2], "create")) {
		if ( shtel->create() ) {
			printf("Shared memory created\n");
		}
	} else {
		if ( !shtel->connect() ) {
			return 1;
		}
	}

	int type;;
	char* data;
	int status;

	if (!strcmp(argv[2], "write")) {
		assert(argc > 4);
		type = atoi(argv[3]);
		data = argv[4];
		printf("type=%d data=%s\n", type, data);

		status = shtel->add(type, data, strlen(data)+1);
		if (status == 1) {
			printf("Write data success\n");
		} else {
			printf("Write data failure\n");
		}
	}

	if (!strcmp(argv[2], "read")) {
		assert(argc > 3);
		type = atoi(argv[3]);
		printf("type=%d\n", type);

		char buff[200];
		status = shtel->get(type, buff);
		if (status == 1) {
			printf("Read data success: %s\n", buff);
		} else {
			printf("Read data failure\n");
		}
	}

	int memlen = 200;
	char* words[] = {new char[memlen], new char[memlen], new char[memlen]};
	int counter = 0;
	time_t timer = time(0);
	int inc = 0;

	if (!strcmp(argv[2], "trashwrite")) {
		assert(argc > 4);
		type = atoi(argv[3]);
		inc = atoi(argv[4]);

		int i = 0;
		while(1) {
			int status = shtel->add(type, words[i], memlen );
			if (status == 0) {
				break;
			}
			i = (i+1) % 3;
			type += inc;

			counter++;
			if (difftime(time(0), timer) >= 1) {
				printf("Write %d records, %d bytes\n", counter, counter * memlen);
				counter = 0;
				timer = time(0);
			}
		}
	}

	if (!strcmp(argv[2], "trashread")) {
		assert(argc > 4);
		type = atoi(argv[3]);
		inc = atoi(argv[4]);

		while(1) {
			char buff[memlen];
			int status = shtel->get(type, buff);
			if (status == 0) {
				break;
			}
			type += inc;

			status = 0;
			for (int i=0; i < 3; i++) {
				if ( !strcmp(words[i], buff) ) {
					status = 1;
					break;
				}
			}
			if (status == 0) {
				printf("Data %d isn't euqals\n", type);
				break;
			}

			counter++;
			if (difftime(time(0), timer) >= 1) {
				printf("Read %d data\n");
				timer = time(0);
				counter = 0;
			}
		}
	}

	shtel->disconnect();
	for (int i = 0; i < 3; i++) {
		delete[] words[i];
	}

	delete shtel;

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

		scanf("/n");

		mut.unlock();
		printf("mutex unlcoked\n");
	}

	return 0;
}*/
