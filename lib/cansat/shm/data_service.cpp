/*
 * shared_telemetry.cpp
 *
 *  Created on: 22.01.2017
 *      Author: zzuummaa
 */

#include "data_service.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


/**
 * ===============================================
 * =            DBMDataService                   =
 * ===============================================
 */

DBMDataService::DBMDataService() {
	initDataBase(this, DEFAULT_D_BASE_NAME);
}

DBMDataService::DBMDataService(char* serviceName) {
	initDataBase(this, serviceName);
}

int DBMDataService::create() {
	return dataBase::create();
}

int DBMDataService::remove() {
	return dataBase::remove();
}

int DBMDataService::connect() {
	return dataBase::connect();
}

int DBMDataService::disconnect() {
	return dataBase::disconnect();
}

int DBMDataService::get(int key, char* data) {
	return dataBase::get((char*)&key, sizeof(key), data);
}

int DBMDataService::add(int key, char* data, int size) {
	return dataBase::add((char*)&key, sizeof(key), data, size);
}

/**
 * ===============================================
 * =            IPCDataService                   =
 * ===============================================
 */

void initIPCDataService(IPCDataService* thism, char* serviceName, const int memSize, memType memtype) {
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

IPCDataService::IPCDataService(memType memtype) {
	initIPCDataService(this, DEFAULT_D_SERV_NAME, DEFAULT_MEM_SIZE, memtype);
}

IPCDataService::IPCDataService() {
	initIPCDataService(this, DEFAULT_D_SERV_NAME, DEFAULT_MEM_SIZE, MEM_SHARED);
}

IPCDataService::IPCDataService(char* serviceName, const int memSize, memType memtype) {
	initIPCDataService(this, serviceName, memSize, memtype);
}

IPCDataService::~IPCDataService() {
	delete shm;
	delete mutex;
	delete mparser;
}

/**
 * Create new service with "serviceName" name.
 */
int IPCDataService::create() {
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

int IPCDataService::remove() {
	if (mutex->remove() && shm->remove()) {
		return 1;
	} else {
		return 0;
	}
}

int IPCDataService::connect() {
	if ( mutex->open() && shm->open_() ) {
		mparser->setBuffPoint( shm->getMem() );
		mparser->setCapacity(  shm->size );
		return 1;
	} else {
		return 0;
	}
}

int IPCDataService::disconnect() {
	if ( mutex->close() && shm->close_() ) {
		return 1;
	} else {
		return 0;
	}
}

int IPCDataService::clear() {
	int res = 1;

	if ( !mutex->lock() ) return 0;

	mparser->clear();

	if ( !mutex->unlock() ) return 0;

	return res;
}

parser* IPCDataService::getParser() {
	return mparser;
}

void IPCDataService::setParser(parser* parser) {
	delete mparser;
	this->mparser = parser;
}

int IPCDataService::get(int type, char* data) {
	int res = 1;

	if ( !mutex->lock() ) return 0;

	try {
		char* buffP;

		int count = mparser->get(type, buffP);
		if ( count == -1) throw(2);

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

int IPCDataService::getBuff(char* buff, int buff_size) {
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

int IPCDataService::add(int type, char* data, int size) {
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
	IPCDataService shtel;

	if ( !shtel.create() ) {
		return 0;
	}

	shtel.disconnect();

	return 1;
}
