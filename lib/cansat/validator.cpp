/*
 * validator.cpp
 *
 *  Created on: 13 апр. 2017 г.
 *      Author: Cntgfy
 */

#include "validator.h"
#include "limits.h"

#include <chrono>

using namespace std::chrono;

/**
 * return current time in milliseconds
 */
int getCurTime() {
	milliseconds curTime = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);

	return curTime.count();
}

validator::validator() {
}


validator::~validator() {
	for (std::map<int, telemetryConditions*>::iterator it=telCons.begin(); it!=telCons.end(); ++it) {
		delete it->second;
	}
}

void validator::addType(int tel_type) {
	if (telCons.find(tel_type) != telCons.end()) {
		cerr << "Error: telemetry with type " << tel_type << " already exist" << endl;
		exit(0);
	}

	telCons[tel_type] = new telemetryConditions();
}

telemetryConditions* validator::getWithType(int tel_type) {
	std::map<int, telemetryConditions*>::iterator iter = telCons.find(tel_type);
	if (iter == telCons.end()) {
		cerr << "Error: element with type " << tel_type << " not found" << endl;
		exit(0);
	}

	return iter->second;
}

telemetryConditions::telemetryConditions() {
	maxWaitTimeMS = INT_MAX;
	lastTimeMS = getCurTime();
}

telemetryConditions::~telemetryConditions() {
}

void telemetryConditions::setFieldDiapason(int field_num, double begin, double end) {
	this->begin[field_num] = begin;
	this->end[field_num]   = end;
}

bool telemetryConditions::checkFieldDiapason(int field_num, double val) {
	if (begin.find(field_num) == begin.end()) {
		cerr << "Error: field_num=" << field_num << ", and not found" << endl;
		exit(0);
	}

	if (begin[field_num] < val && val < end[field_num]) {
		return true;
	} else {
		return false;
	}
}

void telemetryConditions::setMaxWaitTime(int ms) {
	maxWaitTimeMS = ms;
}

bool telemetryConditions::checkWaitTime() {
	int waitTimeMS;
	return checkWaitTime(&waitTimeMS);
}

bool telemetryConditions::checkWaitTime(int* waitTimeMS) {
	int curTimeMS = getCurTime();
	*waitTimeMS = curTimeMS - lastTimeMS;

	bool flag = *waitTimeMS < maxWaitTimeMS;

	lastTimeMS = curTimeMS;

	return flag;
}
