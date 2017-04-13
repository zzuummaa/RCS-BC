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
	map<int, double> begin;
	map<int, double> end;
public:
	telemetryConditions();
	~telemetryConditions();
	void setValidFieldDiapason(int field_num, double begin, double end);
	bool checkFieldDiapason(int field_num, double val);
};

class validator {
private:
	map<int, telemetryConditions*> telCons;
public:
	validator();
	~validator();
	void addTelInfo(int tel_type);
	telemetryConditions* getTelInfo(int tel_type);
};

#endif /* CANSAT_VALIDATOR_H_ */
