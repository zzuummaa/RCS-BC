/*
 * ipcmutex.h
 *
 *  Created on: 21.01.2017
 *      Author: zzuummaa
 */

#ifndef IPCMUTEX_H_
#define IPCMUTEX_H_

#include <stddef.h>
#include <semaphore.h>

class IPCMutex {
public:
	char* semName;
	sem_t *sem;
	IPCMutex(char* semName);
	~IPCMutex();
	int create();
	int open();
	int close();
	int remove();
	int lock();
	int unlock();
};

#endif /* IPCMUTEX_H_ */
