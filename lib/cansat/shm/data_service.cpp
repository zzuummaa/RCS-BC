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
#include <assert.h>

#include "data_service.h"

void initDataService(dataService* thism, char* serviceName, const int memSize, memType memtype) {
	char tmp[100];
	sprintf(tmp, "shm_%s", serviceName);

	switch (memtype) {
	case MEM_FILE   : thism->shm = new fileMemory(tmp, memSize); break;
	case MEM_SHARED : thism->shm = new sharedMemory(tmp, memSize); break;
	defualt         : assert(memtype != MEM_FILE && memtype != MEM_SHARED);
	}

	sprintf(tmp, "/mut_%s", serviceName);
	thism->mutex = new IPCMutex(tmp);

	thism->mparser = new mapParser();
}

dataService::dataService(memType memtype) {
	initDataService(this, DEFAULT_D_SERV_NAME, DEFAULT_MEM_SIZE, memtype);
}

dataService::dataService() {
	initDataService(this, DEFAULT_D_SERV_NAME, DEFAULT_MEM_SIZE, MEM_SHARED);
}

dataService::dataService(char* serviceName, const int memSize, memType memtype) {
	initDataService(this, serviceName, memSize, memtype);
}

dataService::~dataService() {
	delete shm;
	delete mutex;
	delete mparser;
}

/**
 * Create new service with "serviceName" name.
 */
int dataService::create() {
	if ( !mutex->create() || !shm->create() ) {
		return 0;
	}

	if ( !markMem(shm->getMem(), shm->size) ) {
		perror("dataService_markMem");
		return 0;
	}

	mparser->setBuffPoint( shm->getMem() );
	mparser->setCapacity(  shm->size );

	return 1;
}

int dataService::connect() {
	if ( mutex->open() && shm->open_() ) {
		mparser->setBuffPoint( shm->getMem() );
		mparser->setCapacity(  shm->size );
		return 1;
	} else {
		return 0;
	}
}

int dataService::disconnect() {
	if ( mutex->close() && shm->close_() ) {
		return 1;
	} else {
		return 0;
	}
}

mapParser* dataService::getParser() {
	return mparser;
}

void dataService::setParser(mapParser* parser) {
	delete mparser;
	this->mparser = parser;
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
	dataService shtel;

	if ( !shtel.create() ) {
		return 0;
	}

	shtel.disconnect();

	return 1;
}
