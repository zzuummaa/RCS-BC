/*
 * validator.h
 *
 *  Created on: 13 апр. 2017 г.
 *      Author: Cntgfy
 */

#ifndef CANSAT_VALIDATOR_H_
#define CANSAT_VALIDATOR_H_

#include <map>
#include <iostream>

using namespace std;

class telemetryConditions {
private:
	int maxWaitTimeMS;
	int lastTimeMS;
	map<int, double> begin;
	map<int, double> end;
public:
	telemetryConditions();
	~telemetryConditions();

	void setFieldDiapason(int field_num, double begin, double end);
	bool checkFieldDiapason(int field_num, double val);

	void setMaxWaitTime(int ms);
	bool checkWaitTime();
	bool checkWaitTime(int* waitTimeMS);
};

class validator {
private:
	map<int, telemetryConditions*> telCons;
public:
	validator();
	~validator();
	void addType(int tel_type);
	telemetryConditions* getWithType(int tel_type);
};

/**
 * return current time in milliseconds
 */
int getCurTime();

#endif /* CANSAT_VALIDATOR_H_ */
