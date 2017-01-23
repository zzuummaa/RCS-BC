//============================================================================
// Name        : shared_memory.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "shared_memory.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

/**
 * Map addresses from shm - file descriptor.
 *
 * shm - not NULL file descriptor
 *
 * return value is the actual mapping address chosen
 * 		  NULL if error
 */
char* mmap_mem(int shm, int size) {
	char* addr = (char*) mmap(0, size, PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
	if ( addr == MAP_FAILED ) {
		return NULL;
	}

	return addr;
}

int sharedMemory::mmap() {
	mem = mmap_mem(shm, size);

	if (mem == NULL) {
		perror("mmap");
		return 0;
	} else {
		return 1;
	}
}

int sharedMemory::create() {
	if ( (shm = shm_open(memName, O_CREAT|O_RDWR, 0777)) == -1 ) {
		perror("shm_open");
		return 0;
	}

	if ( ftruncate(shm, size) == -1 ) {
		perror("ftruncate");
		return 0;
	}

	return mmap();
}

int sharedMemory::open() {
	if ( (shm = shm_open(memName, 0|O_RDWR, 0777)) == -1 ) {
		perror("shm_open");
		return 0;
	}

	return mmap();
}

char* sharedMemory::getMem() {
	return mem;
}

int sharedMemory::close_() {
	int status = 1;

	if ( munmap(mem, size) == -1 ) {
		perror("munmap");
		status = 0;
	}

	if ( close(shm) == -1 ) {
		perror("close");
		status = 0;
	}

	return status;;
}

int sharedMemory_remove(const char* memName) {
	if ( shm_unlink(memName) == -1 ) {
		perror("shm_unlink");
		return 0;
	} else {
		return 1;
	}
}
