/*
 * shared_memory.h
 *
 *  Created on: 21.01.2017
 *      Author: zzuummaa
 */

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

#include <stddef.h>

class sharedMemory {
public:
	char* memName;
	int shm, size;
	char *mem;

	sharedMemory(char* memName, const int size) {
		this->memName = memName;
		this->size = size;
		this->shm = -1;
		this->mem = NULL;
	}

	int create();
	int open();
	char* getMem();
	int close_();
private:
	int mmap();
};

int sharedMemory_remove(const char* memName);

#endif /* SHARED_MEMORY_H_ */
