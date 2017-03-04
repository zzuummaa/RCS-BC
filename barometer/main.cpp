/*
 * main.cpp
 *
 *  Created on: 1 мар. 2017 г.
 *      Author: Cntgfy
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <iostream>
using namespace std;

#include "barometer.h"

int main() {
	Barometer barom;

	barom.begin();

	int count = 1;
	double pressure = 0;
	double temperature = 0;

	for (int i=0; i < count; i++) {
		pressure += barom.readPressureMillibars();
		temperature += barom.readTemperatureC();
		usleep(10 * 1000);
	}

	pressure /= count;
	temperature /= count;

	cout << "pressure in millibars: " << pressure << endl;
	cout << "temperature in Celsius: " << temperature << endl;

	return 0;
}
