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

int redisDataService::get(int key, char* data) {
	cerr << "Operation redisDataService_get() isn't supported" << endl;

	return 0;
}

int redisDataService::get(int type, int time, char* lastData) {
	string formatKey = to_string(type) + ":" + to_string(time) + ":*";

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

	reply r = conn->run(command("SET") << key << string(data, size));

	if (r.type() == reply::type_t::STATUS) {
		if ( r.str().compare("OK") == 0 ) {
			return 1;
		}
	}

	return 0;
}
