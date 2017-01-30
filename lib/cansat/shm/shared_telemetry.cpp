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

#include "shared_telemetry.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int size;
} shTelInfo;

int shTelemetry::connect() {
	if ( mutex->open() && shm->open() ) {
		mparser = new mapParser(shm->getMem(), shm->size);
		return 1;
	} else {
		return 0;
	}
}

int shTelemetry::disconnect() {
	if ( mutex->close() && shm->close_() ) {
		delete mparser;
		return 1;
	} else {
		return 0;
	}
}

int shTelemetry::get(int type, telData_t data) {
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

int shTelemetry::getAll(char* buff, int buff_size) {
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

int shTelemetry::add(int type, telData_t data, int size) {
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
