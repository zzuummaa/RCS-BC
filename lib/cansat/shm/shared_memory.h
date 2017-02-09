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

	virtual ~memory() {
		destroyMem(this);
	}

	char* getMem();

	int create();
	virtual int remove();
	int open_();
	int close_();
protected:
	int mmap();
	int fruncate();
	virtual int openToCreate() { return 0;};
	virtual int openToRW() { return 0; };
};

class sharedMemory : public memory {
public:
	sharedMemory(char* memName, const int size);
	int remove();
protected:
	int openToCreate();
	int openToRW();
};

int sharedMemory_remove(const char* memName);

class fileMemory : public memory {
private:
	int allocMem();
public:
	fileMemory(char* memName, const int size);
	int remove();
protected:
	int openToCreate();
	int openToRW();
};

#endif /* SHARED_MEMORY_H_ */
