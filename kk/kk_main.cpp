//============================================================================
// Name        : test1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "shm/redis.h"
#include "structs.h"
#include "GPIOClass.h"

#include <iostream>
#include <chrono>
#include <unistd.h>
#include <string.h>

using namespace std;
using namespace std::chrono;

int main() {
	dataService* dserv = new redisDataService();
	dserv->connect();

	GPIOClass* gpio4 = new GPIOClass("4"); //create new GPIO object to be attached to  GPIO4
	GPIOClass* gpio2 = new GPIOClass("2"); //create new GPIO object to be attached to  GPIO17

	gpio2->setdir_gpio("out"); //GPIO4 set to output
	gpio4->setdir_gpio("in");

	string inVal = "0";
	string oldInVal = "0";
	int subImpulsCount = 0;
	int dataServiceTimer = 0;

	while (1) {
		gpio2->setval_gpio("1");
		usleep(50000);
		gpio2->setval_gpio("0");
		usleep(50000);

		gpio4->getval_gpio(inVal);
		if ( inVal.compare(oldInVal) != 0 ) {
			subImpulsCount++;
			oldInVal = inVal;
		}

		dataServiceTimer += 100;

		if (dataServiceTimer > 1000) {
			tel_kk tel;
			tel.impulsPerSec = subImpulsCount / 2; //low and high signals is one impulse

			dserv->add(TYPE_KK, (char*)&tel, sizeof(tel_kk));

			dataServiceTimer = 0;
			subImpulsCount = 0;
		}
	}

	return 0;
}
