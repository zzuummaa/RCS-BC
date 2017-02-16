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
#include "db.h"


#define DEFAULT_D_SERV_NAME		"dataService"
#define DEFAULT_MUTEX_NAME		"/tel_mutex"
#define DEFAULT_SH_MEM_NAME		"tel_sh_memory"
#define DEFAULT_MEM_SIZE		50000

class dataService {
public:
	virtual ~dataService() {};

	/**
	 * Create service for data drive.
	 *
	 * Remove old dataService with similar name/descriptor/...etc.
	 *
	 * When the service is created you are already connected to it.
	 *
	 * return 1 - success
	 * 		  0 - error occurred
	 */
	virtual int create() {return 0;};

	/**
	 * Remove service
	 *
	 * Don't need to connect to the service previously
	 *
	 * return 1 success
	 * 	  	  0 error occurred
	 */
	virtual int remove() {return 0;};

	/**
	 * Connect to the service
	 *
	 * return 1 success
	 * 		  0 error occurred
	 */
	virtual int connect() {return 0;};

	/**
	 * Disconnect from the service
	 *
	 * return 1 success
	 * 		  0 error occurred
	 */
	virtual int disconnect() {return 0;};

	/**
	 * Fills the data buffer by reading them by type
	 *
	 * return 1 success
	 * 		  0 fail (not found, other error)
	 */
	virtual int get(int type, char* data) { return 0;};

	/**
	 *
	 */
	virtual int add(int type, char* data, int size) { return 0;};
};

#define DEFAULT_BLOCK_SIZE 512

typedef struct {
	int32_t time;
	int32_t type;
} dbm_key;

class DBMDataService : public dataService, private dataBase {
public:

	DBMDataService();
	DBMDataService(char* serviceName);

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
	int get(int key, char* data);
	int add(int type, char* data, int size);

};

typedef enum memType {
	MEM_FILE,
	MEM_SHARED
};

class IPCDataService : public dataService {
private:
	memory* shm;
	IPCMutex* mutex;
	parser* mparser;
public:
	friend void initIPCDataService(IPCDataService* thism, char* serviceName, const int memSize, memType memtype);

	IPCDataService();
	IPCDataService(memType memtype);
	IPCDataService(char* serviceName, const int memSize, memType memtype);
	~IPCDataService();
	int create();
	int remove();
	int connect();
	int disconnect();

	int clear();

	parser* getParser();
	void setParser(parser* parser);

	/**
	 * write telemetry data with input type
	 *
	 * return 1 if success
	 * 		  0 if fail
	 */
	int get(int type, char* data);

	/**
	 * copy all telemetry to buff
	 *
	 * return size of wrote data
	 * 		  0 if error
	 */
	int getBuff(char* buff, int buff_size);
	int add(int type, char* data, int size);
};

int shTelemetry_create();

#endif /* SHARED_TELEMETRY_H_ */
