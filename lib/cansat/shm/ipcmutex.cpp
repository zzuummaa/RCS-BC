/*
 * ipcmutex.cpp
 *
 *  Created on: 21.01.2017
 *      Author: zzuummaa
 */

#include "ipcmutex.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

IPCMutex::IPCMutex(char* semName) {
	this->semName = new char[strlen(semName)];
	strcpy(this->semName, semName);
	sem = NULL;
}

IPCMutex::~IPCMutex() {
	delete[] this->semName;
}

int IPCMutex::create() {
	if ( (sem = sem_open(semName, O_CREAT, 0644, 0)) == SEM_FAILED ) {
		perror("sem_create");
		sem_unlink(semName);
		return 0;
	}
	sem_post(sem);

	return 1;
}

int IPCMutex::open() {
	if ( (sem = sem_open(semName, 0, 0644, 0)) == SEM_FAILED ) {
		perror("sem_open");
		sem_unlink(semName);
		return 0;
	}

	return 1;
}

int IPCMutex::close() {
	if ( sem_close(sem) != 0 ) {
		perror("sem_close");
		return 0;
	}

	return 1;
}

int IPCMutex::remove() {
	if (sem_unlink(semName) != 0) {
		perror("sem_unlink");
		return 0;
	}

	return 1;
}


int IPCMutex::lock() {
	if (sem_wait(sem) != 0) {
		perror("sem_post");
		return 0;
	}

	return 1;
}

int IPCMutex::unlock() {
	if (sem_post(sem) != 0) {
		perror("sem_post");
		return 0;
	}

	return 1;
}
