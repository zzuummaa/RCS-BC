/*
 * main.cpp
 *
 *  Created on: 1 мар. 2017 г.
 *      Author: Cntgfy
 */

#include "barometer.h"
#include "structs.h"
#include "shm/redis.h"

#include <unistd.h>

#include <iostream>
using namespace std;

int main() {
	dataService* dserv = new redisDataService();
	dserv->connect();

	Barometer barom;
	barom.begin();

	tel_barometer tel_bar;

	for(;;) {
		tel_bar.pressure = barom.readPressurePascals();
		tel_bar.altitude = barom.GOST4401_altitude(tel_bar.pressure);

		dserv->add(TYPE_BAROMETER, (char*)&tel_bar, sizeof(tel_bar));

		sleep(1);
	}

	return 0;
}
