/*
 * shared_telemetry.h
 *
 *  Created on: 22.01.2017
 *      Author: zzuummaa
 */

#ifndef SHARED_TELEMETRY_H_
#define SHARED_TELEMETRY_H_

#include "shared_memory.h"
#include "ipcmutex.h"
#include "parser.h"

#define DEFAULT_MUTEX_NAME		"/tel_mutex"
#define DEFAULT_SH_MEM_NAME		"tel_sh_memory"
#define DEFAULT_MEM_SIZE		1000

enum memType {
	ERAM,
	SHM
};

/**
 * Type of telemetry data
 */
typedef char* telData_t;

class dataService {
private:
	sharedMemory* shm;
	IPCMutex* mutex;
	mapParser* mparser;
public:
	dataService() {
		shm = new sharedMemory(DEFAULT_SH_MEM_NAME, DEFAULT_MEM_SIZE);
		mutex = new IPCMutex(DEFAULT_MUTEX_NAME);
		mparser = NULL;
	}
	dataService(char* serviceName, const int memSize, memType memtype);
	~dataService() {
		delete shm;
		delete mutex;
		delete mparser;
	}
	int connect();
	int disconnect();

	/**
	 * write telemetry data with input type
	 *
	 * return 1 if success
	 * 		  0 if fail
	 */
	int get(int type, telData_t data);

	/**
	 * copy all telemetry to buff
	 *
	 * return size of wrote data
	 * 		  0 if error
	 */
	int getBuff(char* buff, int buff_size);
	int add(int type, telData_t, int size);
	//int update(int type, telData_t data);
};

int shTelemetry_create();

#endif /* SHARED_TELEMETRY_H_ */
