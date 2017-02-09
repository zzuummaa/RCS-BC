/*
 * db_service.cpp
 *
 *  Created on: 05.02.2017
 *      Author: zzuummaa
 */

#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "db.h"

#ifdef _GDBM_H_

/**
 * ===============================================
 * =            DBMDataService                   =
 * ===============================================
 */

void initDataBase(dataBase* thism, char* serviceName) {
	thism->dbName = new char[strlen(serviceName)];
	strcpy(thism->dbName, serviceName);
}

dataBase::dataBase() {
	initDataBase(this, DEFAULT_D_BASE_NAME);
}

dataBase::dataBase(char* serviceName) {
	initDataBase(this, serviceName);
}

dataBase::~dataBase() {
	delete[] dbName;
}

int dataBase::create() {
	dbmf = gdbm_open(dbName, DEFAULT_BLOCK_SIZE, GDBM_NEWDB | GDBM_NOLOCK | GDBM_SYNC, 0777, NULL);

	if (dbmf == NULL) {
		perror("gdbm_create");
		return 0;
	} else {
		return 1;
	}
}

int dataBase::connect() {
	dbmf = gdbm_open(dbName, DEFAULT_BLOCK_SIZE, GDBM_WRITER | GDBM_NOLOCK | GDBM_SYNC, 0777, NULL);

	if (dbmf == NULL) {
		perror("gdbm_open");
		return 0;
	} else {
		return 1;
	}
}

int dataBase::add(char* key, const int keySize, char* data, const int dataSize) {
	datum key_datum;
	datum data_datum;

	key_datum.dptr = key;
	key_datum.dsize = keySize;

	data_datum.dptr = data;
	data_datum.dsize = dataSize;

	if ( gdbm_store(dbmf, key_datum, data_datum, GDBM_REPLACE) ) {
		perror("gdbm_store");
		return 0;
	}

	return 1;
}

int dataBase::disconnect() {
	if (dbmf == NULL) return 0;

	gdbm_close(dbmf);
	dbmf = NULL;

	return 1;
}

int dataBase::get(char* key, const int keySize, char* data) {
	datum key_datum;
	datum data_datum;

	key_datum.dptr = key;
	key_datum.dsize = keySize;

	data_datum = gdbm_fetch(dbmf, key_datum);

	if ( data_datum.dptr == NULL ) {
		return 0;
	}

	memcpy(data, data_datum.dptr, data_datum.dsize);

	return 1;
}

int dataBase::remove() {
	disconnect();

	if ( unlink(dbName) == 0) {
		return 1;
	} else {
		return 0;
	}
}

#endif
