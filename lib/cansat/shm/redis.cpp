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

int redisDataService::create() {
	cerr << "Operation redisDataService_create() isn't supported" << endl;

	return 0;
}

int redisDataService::remove() {
	cerr << "Operation redisDataService_create() isn't supported" << endl;

	return 0;
}

int redisDataService::connect() {
	this->conn = connection::create();

	return 1;
}

int redisDataService::disconnect() {
	this->conn = NULL;

	return 1;
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

int redisDataService::get_(string key, string* data) {
	reply r1 = conn->run(command("GET") << key);

	if (r1.type() != reply::type_t::STRING) {
		cerr << "redisDataService_get() invalid get reply type\n" << endl;
		return 0;
	}

	*data = r1.str();

	return 1;
}

int redisDataService::get(int key, char* data) {
	cerr << "Operation redisDataService_get() isn't supported" << endl;

	return 0;
}

int redisDataService::addLast(int type, const char* key, const char* data, int size) {
	string outKey = to_string(type) + ":" + last_prefix;
	string outData = string(key, KEY_SIZE) + string(data, size);

	/*reply r = conn->run(command("SET") << outKey << outData);

	if (r.type() == reply::type_t::STATUS) {
		if ( r.str().compare("OK") == 0 ) {
			return 1;
		}
	}*/

	return add_(outKey, outData);
}

int redisDataService::getLast(int type, char* key, char* data) {
	string outKey = to_string(type) + ":" + last_prefix;
	string rData;

	if ( !get_(outKey, &rData) ) {
		return 0;
	}

	const char* pKey  = rData.c_str();
	const char* pData = rData.c_str() + KEY_SIZE;

	memcpy(key,  pKey,  KEY_SIZE);
	memcpy(data, pData, rData.size() - KEY_SIZE);

	return 1;
}

int redisDataService::getFromSec(int type, int sec, char* lastData) {
	string formatKey = to_string(type) + ":" + to_string(sec) + ":*";

	reply r = conn->run(command("KEYS") << formatKey);

	vector<reply> rvec = r.elements();

	if (rvec.size() == 0) return 0;

	reply backReply = rvec.back();

	if (backReply.type() != reply::type_t::STRING) {
		cerr << "redisDataService_get() invalid keys reply type" << endl;
		return 0;
	}

	string key = backReply.str();

	reply r1 = conn->run(command("GET") << key);

	if (r1.type() != reply::type_t::STRING) {
		cerr << "redisDataService_get() invalid get reply type\n" << endl;
		return 0;
	}

	string val = r1.str();

	memcpy(lastData, val.c_str(), val.size());

	return 1;
}

int redisDataService::add(int type, char* data, int size) {
	milliseconds curTime = duration_cast< milliseconds >(
	    system_clock::now().time_since_epoch()
	);

	int ms = curTime.count() % 1000;
	int sec = duration_cast<seconds>(curTime).count();

	string key = to_string(type) + ":" + to_string(sec) + ":" + to_string(ms);

	if ( addLast(type, key.c_str(), data, size) == 0) {
		return 0;
	}

	return add_(key, string(data, size));
}
