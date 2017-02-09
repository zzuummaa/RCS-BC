/*
 * db_service.h
 *
 *  Created on: 05.02.2017
 *      Author: zzuummaa
 */

#ifndef DSERV_DB_H_
#define DSERV_DB_H_

#include <gdbm.h>

#define DEFAULT_D_BASE_NAME			"dataBase"
#define DEFAULT_BLOCK_SIZE 			512

class dataBase {
private:
	char* dbName;
	GDBM_FILE dbmf;
public:
	friend void initDataBase(dataBase* thism, char* serviceName);

	dataBase();
	dataBase(char* serviceName);
	~dataBase();

	int create();
	int remove();
	int connect();
	int disconnect();

	/**
	 * write telemetry data with input type
	 *
	 * return 1 if success
	 * 		  0 if fail
	 */
	int get(char* key, const int keySize, char* data);

	int add(char* key, const int keySize, char* data, const int dataSize);
};

#endif /* DSERV_DB_H_ */
