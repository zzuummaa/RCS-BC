/*
 * redis.h
 *
 *  Created on: 16.02.2017
 *      Author: zzuummaa
 */

#ifndef DSERV_REDIS_H_
#define DSERV_REDIS_H_

#include <redis3m/redis3m.hpp>

using namespace std;
using namespace redis3m;

const int KEY_SIZE = 9;

typedef struct {
	uint16_t type;
	int sec;
	uint16_t ms;
} req_s;

typedef struct {
	uint16_t type;
	int sec;
	uint16_t ms;
	char* data;
	uint8_t dataSize;
} resp_s;

class redisDataService {
private:

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
	 * return count or -1 if erorr
	 */
	int getSortedSetCount(string* objKey);

    string getSortedSetKey(uint16_t type, int sec);

    int addToSortedSet(req_s* req, string* data);

    int getFromSortedSet(req_s* req, int deltaTime, string* data);

    string getHashKey(uint16_t type, int sec);

    int addToHash(req_s* req, string* data);

    int getFromHash(req_s* req, string* data);

    string getKey(uint16_t type, int sec, uint16_t num);

    string getObjKey(uint16_t type, int sec);

	/**
	 * return 1 if success, 0 if failure
	 */
	int add_(string key, string data);

	int get_(string key, string* data);

	/**
	 * requested telemetry data with ms range [req.ms, req.ms + deltaMS]
	 *
	 * change req.ms value to response ms value
	 *
	 * return 1 if success
	 * 		  0 if fail
	 */
	int get(req_s* req, int deltaMS, char* data);

	int getLast_(int type, resp_s* resp);

	int getLast(int type, char* buff);

	int add(int type, char* data, int size);
	int addLast(req_s* req);
};

void getCurTime(int* sec, int* ms);

#endif /* DSERV_REDIS_H_ */
