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
		return 1;
	} else {
		return 0;
	}
}

int shTelemetry::disconnect() {
	if ( mutex->close() && shm->close_() ) {
		return 1;
	} else {
		return 0;
	}
}

char* shTelemetry::getMem() {
	return shm->getMem() + sizeof(shTelInfo);
}

int shTelemetry::getSize() {
	char* memp = shm->getMem();
	if (memp == NULL) {
		return 0;
	}

	shTelInfo* sh = (shTelInfo*)memp;
	return sh->size;
}

int shTelemetry::setSize(int size) {
	char* memp = shm->getMem();
	if (memp == NULL) {
		return 0;
	}

	shTelInfo* sh = (shTelInfo*)memp;
	sh->size = size;
	return 1;
}

/**
 * It Finds data with input type
 * return pointer to desired title
 * 	  or  NULL if error occurred
 * 	  or  title with type=0 if not found
 */
title* shTelemetry::searchTitle(int type) {
	char* memp = getMem();
	if (memp == NULL) {
		return NULL;
	}

	title* tit = (title*) memp;
	while (tit->type != 0) {
		if (tit->type == type) {
			break;
		}
		tit += tit->size + sizeof(title);
	}

	return tit;
}

int shTelemetry::get(int type, telData_t data) {
	int res = 1;

	if ( !mutex->lock() ) return 0;

	try {
		title* tit = searchTitle(type);
		if (tit == NULL) {
			throw(1);
		}
		if (tit->type == 0) {
			throw(2);
		}

		memcpy(data, (char*)tit + sizeof(title), tit->size);
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

int shTelemetry::get(char* buff, int buff_size) {
	int size = 0;

	if ( !mutex->lock() ) return 0;

	try {
		size = getSize() - sizeof(title) - sizeof(shTelInfo);
		char* memp = getMem();

		if (memp == NULL) {
			throw(5);
		}

		if (size > buff_size) {
			throw(6);
		}

		memcpy(buff, memp, size);
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
		//We are remember that search title return last
		//title if not found desired title or
		title* tit = searchTitle(type);
		if (tit == NULL) {
			throw(1);
		}

		if (tit->type == type) {
			if (tit->size != size) {
				throw(4);
			}
		}

		if (tit->type == 0) {
			int requiredMem = getSize() + sizeof(title) + size;
			if (requiredMem > shm->size) {
				throw(3);
			}

			title* newLastTitle = (title*) ( (char*)tit + sizeof(title) + size );
			newLastTitle->size = 0;
			newLastTitle->type = 0;

			tit->type = type;
			tit->size = size;

			setSize(requiredMem);
		}

		memcpy((char*)tit + sizeof(title), data, size);
	} catch (int e) {
		printf("Error caught: ");
		switch (e) {
		case 1: printf("Search title error\n"); break;
		case 3: printf("Out of shared memory error\n"); break;
		case 4: printf("Not equals data size error\n"); break;
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

	char* memp = shm.getMem();
	memset(memp, '\0', shm.size);

	shTelInfo* telinfo = (shTelInfo*)memp;
	telinfo->size = sizeof(shTelInfo) + sizeof(title);

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
