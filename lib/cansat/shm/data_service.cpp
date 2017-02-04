/*
 * shared_telemetry.cpp
 *
 *  Created on: 22.01.2017
 *      Author: zzuummaa
 */

/**
 * Description:
 *
 * Shared memory organization ( I suppose:) )
 *
 * [size][title 1][  data2  ][title 2][  data2   ]..[title {lastTitNum}]
 *
 * In title with number "lastTitNum", field "type" is equals zero (type=0).
 * "size" - size in bytes of all memory which are in use in current moment
 */

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "data_service.h"

dataService::dataService(char* serviceName, const int memSize, memType memtype) {
	char tmp[100];
	sprintf(tmp, "shm_%s", serviceName);
	shm = new sharedMemory(tmp, memSize);

	sprintf(tmp, "/mut_%s", serviceName);
	mutex = new IPCMutex(tmp);

	mparser = NULL;
}

int dataService::connect() {
	if ( mutex->open() && shm->open_() ) {
		mparser = new mapParser(shm->getMem(), shm->size);
		return 1;
	} else {
		return 0;
	}
}

int dataService::disconnect() {
	if ( mutex->close() && shm->close_() ) {
		delete mparser;
		mparser = NULL;
		return 1;
	} else {
		return 0;
	}
}

int dataService::get(int type, telData_t data) {
	int res = 1;

	if ( !mutex->lock() ) return 0;

	try {
		char* buffP;

		int count = mparser->get(type, &buffP);
		if ( count == -1) throw(2);

		memcpy(data, buffP, count);
	} catch(int e) {
		printf("Error caught: ");
		switch (e) {
		case 1: printf("Search title error\n"); break;
		case 2: printf("Title with type %d not found\n", type); break;
		}
		res = 0;
	}

	if ( !mutex->unlock() ) return 0;

	return res;
}

int dataService::getBuff(char* buff, int buff_size) {
	int size = 0;

	if ( !mutex->lock() ) return 0;

	try {
		size = mparser->cpyBuff(buff, buff_size);
		if (size < 0) throw(5);
	} catch(int e) {
		printf("Error caught: ");
		switch (e) {
		case 5: printf("Read memory error\n"); break;
		case 6: printf("To small input buffer error\n"); break;
		}
		size = 0;
	}

	if ( !mutex->unlock() ) return 0;

	return size;
}

int dataService::add(int type, telData_t data, int size) {
	int res = 1;

	if ( !mutex->lock() ) return 0;

	try {
		if ( mparser->add(type, data, size) != 1 ) {
			throw(1);
		}
	} catch (int e) {
		printf("Error caught: ");
		switch (e) {
		case 1: printf("Parse data error\n");
		}
		res = 0;
	}

	if ( !mutex->unlock() ) return 0;

	return res;
}

int shTelemetry_create() {
	sharedMemory shm(DEFAULT_SH_MEM_NAME, DEFAULT_MEM_SIZE);
	if ( !shm.create() ) {
		return 0;
	}

	if ( !markMem(shm.getMem(), shm.size) ) {
		return 0;
	}

	if ( !shm.close_() ) {
		return 0;
	}

	IPCMutex mutex(DEFAULT_MUTEX_NAME);
	if ( !mutex.create() ) {
		return 0;
	}
	if ( !mutex.close() ) {
		return 0;
	}

	return 1;
}
