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
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

/**
 * ===============================================
 * =                  memory                     =
 * ===============================================
 */

void initMem(memory* thism, char* memName, const int size) {
	thism->memName = new char[strlen(memName)];
	strcpy(thism->memName, memName);

	thism->size = size;
	thism->desc = -1;
	thism->mem = NULL;
}

void destroyMem(memory* thism) {
	delete[] thism->memName;
}

int memory::create() {
	if ( !openToCreate() ) {
		return 0;
	}

	if ( !fruncate() ) {
		return 0;
	}

	return mmap();
}

int memory::remove() {
	if ( unlink(this->memName) == 0) {
		return 1;
	} else {
		return 0;
	}
}

int memory::open_() {
	if ( !openToRW() ) {
		return 0;
	}

	return mmap();
}

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

int memory::mmap() {
	mem = mmap_mem(desc, size);

	if (mem == NULL) {
		perror("mmap");
		return 0;
	} else {
		return 1;
	}
}

int memory::fruncate() {
	if ( ftruncate(desc, size) == -1 ) {
		perror("ftruncate");
		return 0;
	} else {
		return 1;
	}
}

char* memory::getMem() {
	return mem;
}

int memory::close_() {
	int status = 1;

	if ( munmap(mem, size) == -1 ) {
		perror("munmap");
		status = 0;
	}

	if ( close(desc) == -1 ) {
		perror("close");
		status = 0;
	}

	return status;;
}

/**
 * ===============================================
 * =                sharedMemory                 =
 * ===============================================
 */

sharedMemory::sharedMemory(char* memName, const int size) {
	initMem(this, memName, size);
}

int sharedMemory::openToRW() {
	if ( (desc = shm_open(memName, 0|O_RDWR, 0777)) == -1 ) {
		perror("shm_open");
		return 0;
	} else {
		return 1;
	}
}

int sharedMemory::openToCreate() {
	if ( (desc = shm_open(memName, O_CREAT|O_RDWR, 0777)) == -1 ) {
		perror("shm_open");
		return 0;
	} else {
		return 1;
	}
}

int sharedMemory_remove(const char* memName) {
	if ( shm_unlink(memName) == -1 ) {
		perror("shm_unlink");
		return 0;
	} else {
		return 1;
	}
}

int sharedMemory::remove() {
	if ( shm_unlink(memName) == -1 ) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * ===============================================
 * =                 fileMemory                  =
 * ===============================================
 */

fileMemory::fileMemory(char* memName, const int size) {
	initMem(this, memName, size);
}

int fileMemory::allocMem() {
	char tmp[0xfffff];
	memset(tmp, '\0', sizeof(tmp));

	int status = 0, count = 0;
	int size = this->size;
	while (size > 0) {
		if (size / sizeof(tmp) > 0) {
			count = sizeof(tmp);
		} else {
			count = size % sizeof(tmp);
		}

		status = write(desc, tmp, count);
		if (status != count) {
			return 0;
		}
	}

	return 1;
}

int fileMemory::openToCreate() {
	if ( (desc = open(memName, O_CREAT|O_RDWR, 0777)) == -1) {
		perror("fileMemory_open");
		return 0;
	} else {
		return 1;
	}
}

int fileMemory::openToRW() {
	if ( (desc = open(memName, O_RDWR)) == -1) {
		perror("fileMemory_open");
		return 0;
	} else {
		return 1;
	}
}

int fileMemory::remove() {
	if ( unlink(memName) == -1) {
		return 0;
	} else {
		return 1;
	}
}
