/*
 * shared_memory.h
 *
 *  Created on: 21.01.2017
 *      Author: zzuummaa
 */

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

#include <stddef.h>

class memory {
public:
	char* memName;
	int desc, size;
	char* mem;

	friend void initMem(memory* thism, char* memName, const int size);
	friend void destroyMem(memory* thism);

	char* getMem();

	int create();
	int open_();
	int close_();
protected:
	int mmap();
};

class sharedMemory : public memory {
public:
	sharedMemory(char* memName, const int size);
	~sharedMemory();

	int create();
	int open_();
	int close_();
};

int sharedMemory_remove(const char* memName);

class fileMemory : public memory {
private:
	int allocMem();
public:
	fileMemory(char* memName, const int size);
	~fileMemory();

	int create();
	int open_();
	int close_();
};

#endif /* SHARED_MEMORY_H_ */
