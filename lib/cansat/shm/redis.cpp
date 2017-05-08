/*
 * redis.cpp
 *
 *  Created on: 16.02.2017
 *      Author: zzuummaa
 */

#include "redis.h"
#include <iostream>
#include <string.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

const string last_prefix = "l";

void getCurTime(int* sec, int* ms) {
	milliseconds curTime = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);

	*ms = curTime.count() % 1000;
	*sec = duration_cast<seconds>(curTime).count();
}

int redisDataService::create() {
	cerr << "Operation redisDataService_create() isn't supported" << endl;

	return 0;
}

int redisDataService::remove() {
	cerr << "Operation redisDataService_create() isn't supported" << endl;

	return 0;
}

int redisDataService::getSortedSetCount(string* objKey) {
	reply r1 = conn->run(command("ZCOUNT") << *objKey << "-inf" << "inf");

	if (r1.type() == reply::type_t::NIL) {
		return 0;
	}

	if (r1.type() != reply::type_t::INTEGER) {
		cerr << "redisDataService_getStoredSetCount() invalid get reply type\n" << endl;
		return -1;
	}

	return r1.integer();
}

int redisDataService::connect() {
	this->conn = connection::create();

	return 1;
}

int redisDataService::disconnect() {
	this->conn = NULL;

	return 1;
}

string redisDataService::getKey(uint16_t type, int sec, uint16_t num) {
	return to_string(type) + ":t:" + to_string(sec) + ":" + to_string(num);
}

string redisDataService::getSortedSetKey(uint16_t type, int sec) {
	return to_string(type) + ":" + to_string(sec) + ":s";
}

string redisDataService::getObjKey(uint16_t type, int sec) {
    return to_string(type) + ":s:" + to_string(sec);
}

int redisDataService::add_(string key, string data) {
	reply r = conn->run(command("SET") << key << data );

	if (r.type() == reply::type_t::STATUS) {
		if ( r.str().compare("OK") == 0 ) {
			return 1;
		}
	}

	return 0;
}

string redisDataService::getHashKey(uint16_t type, int sec) {
	return to_string(type) + ":" + to_string(sec) + ":h";
}

int redisDataService::addToHash(req_s* req, string* data) {
	string hashKey = getHashKey(req->type, req->sec);

	reply r1 = conn->run(command("HSET") << hashKey << to_string(req->ms) << *data);

	if (r1.type() == reply::type_t::INTEGER) {
		if (r1.integer() == 1) {
			return 1;
		}
	}

	return 0;
}

int redisDataService::getFromHash(req_s* req, string* data) {
	string hashKey = getHashKey(req->type, req->sec);

	reply r1 = conn->run(command("HGET") << hashKey << to_string(req->ms));

	if (r1.type() != reply::type_t::STRING) {
		return 0;
	}

	data->clear();
	data->append(r1.str());

	return 1;
}

int redisDataService::get_(string key, string* data) {
	reply r1 = conn->run(command("GET") << key);

	if (r1.type() != reply::type_t::STRING) {
		//cerr << "redisDataService_get() invalid get reply type\n" << endl;
		return 0;
	}

	*data = r1.str();

	return 1;
}

int redisDataService::get(req_s* req, int deltaMS, char* data) {
	string dat;

	if ( !getFromSortedSet(req, deltaMS, &dat) ) {
		perror("getFromeSortedSet error");
		return 0;
	}

	int oldms = req->ms;
	req->ms = *(uint16_t*)dat.c_str();

	dat.clear();
	if ( !getFromHash(req, &dat) ) {
		req->ms = oldms;
		perror("getFromHash error");
		return 0;
	}

	memcpy(data, dat.c_str(), dat.size());

	return 1;
}

int redisDataService::addLast(req_s* req) {
	string outKey = to_string(req->type) + ":" + last_prefix;
	string outData = string( (char*)req, sizeof(req_s) );

	return add_(outKey, outData);
}

int redisDataService::getLast(int type, char* buff) {
	resp_s resp;
	resp.data = buff;

	return getLast_(type, &resp);
}

int redisDataService::getLast_(int type, resp_s* resp) {
	string outKey = to_string(type) + ":" + last_prefix;
	string lastReq;

	if ( !get_(outKey, &lastReq) ) {
		return 0;
	}

	req_s* req = (req_s*)lastReq.c_str();
	string data;

	if ( !getFromHash(req, &data) ) {
		return 0;
	}

	memcpy(resp->data, data.c_str(), data.size());
	resp->dataSize = data.size();
	resp->type = req->type;
	resp->sec = req->sec;
	resp->ms =req->ms;

	return 1;
}

int redisDataService::addToSortedSet(req_s* req, string* data) {
	string setKey = getSortedSetKey(req->type, req->sec);

	reply r1 = conn->run(command("ZADD") << setKey << to_string(req->ms) << *data);

	if (r1.type() == reply::type_t::INTEGER) {
		if ( r1.integer() == 1 ) {
			return 1;
		}
	}

	return 0;
}

int redisDataService::getFromSortedSet(req_s* req, int deltaMS, string* data) {
	string setKey = getSortedSetKey(req->type, req->sec);

	reply r1 = conn->run(command("ZRANGEBYSCORE") << setKey << to_string(req->ms)
					 << to_string(req->ms + deltaMS) << "LIMIT" << to_string(0) << to_string(1));

	if (r1.type() != reply::type_t::ARRAY) {
		perror("reply type error");
		return 0;
	}

	if (r1.elements().size() < 1) {
		perror("element size error");
		return 0;
	}

	data->clear();
	*data = r1.elements()[0].str();

	return 1;
}

int redisDataService::add(int type, char* data, int size) {
	int sec, ms;
	getCurTime(&sec, &ms);

	req_s req;
	req.type = type;
	req.sec = sec;
	req.ms = ms;

	string valms((char*)( &(req.ms) ), sizeof(req.ms) );

	if ( !addToSortedSet(&req, &valms) ) {
		return 0;
	}

	string dat(data, size);

	if ( !addToHash(&req, &dat) ) {
		return 0;
	}

	if ( !addLast(&req) ) {
		return 0;
	}

	return 1;
}
