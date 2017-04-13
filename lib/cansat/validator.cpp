/*
 * validator.cpp
 *
 *  Created on: 13 апр. 2017 г.
 *      Author: Cntgfy
 */

#include "validator.h"

validator::validator() {
}


validator::~validator() {
	for (std::map<int, telemetryConditions*>::iterator it=telCons.begin(); it!=telCons.end(); ++it) {
		delete it->second;
	}
}

void validator::addTelInfo(int tel_type) {
	if (telCons.find(tel_type) != telCons.end()) {
		cerr << "Error: telemetry with type " << tel_type << " already exist" << endl;
		exit(0);
	}

	telCons[tel_type] = new telemetryConditions();
}

telemetryConditions* validator::getTelInfo(int tel_type) {
	std::map<int, telemetryConditions*>::iterator iter = telCons.find(tel_type);
	if (iter == telCons.end()) {
		cerr << "Error: element with type " << tel_type << " not found" << endl;
		exit(0);
	}

	return iter->second;
}

telemetryConditions::telemetryConditions() {
}

telemetryConditions::~telemetryConditions() {
}

void telemetryConditions::setValidFieldDiapason(int field_num, double begin, double end) {
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
