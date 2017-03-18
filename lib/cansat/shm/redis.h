/*
 * redis.h
 *
 *  Created on: 16.02.2017
 *      Author: zzuummaa
 */

#ifndef DSERV_REDIS_H_
#define DSERV_REDIS_H_

#include <redis3m/redis3m.hpp>
#include "data_service.h"

using namespace redis3m;

class redisDataService : public dataService {
public:
	connection::ptr_t conn;

	~redisDataService() {
		disconnect();
	}

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

	/**
	 * copy to "lastData" last writing data in "time" moment
	 *
	 * time - write moment in seconds
	 *
	 * return 1 if success
	 * 		  0 if failure
	 */
	int getFromSec(int type, int sec, char* lastData);

	int getLast(int type, char* data);

	int add(int type, char* data, int size);
	int addLast(int type, char* data, int size);
};


#endif /* DSERV_REDIS_H_ */