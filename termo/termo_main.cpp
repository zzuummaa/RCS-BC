/*
 * termo_main.cpp
 *
 *  Created on: 9 мар. 2017 г.
 *      Author: Cntgfy
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.h"
#include "shm/redis.h"

const char path[] = "/sys/bus/w1/devices";

int searchDevice(char* dev) {
	struct dirent *dirent;
	DIR* dir = opendir (path);

	if (dir != NULL) {
	while ((dirent = readdir (dir)))
		// 1-wire devices are links beginning with 28-
		if (dirent->d_type == DT_LNK
		 && strstr(dirent->d_name, "28-") != NULL) {
			strcpy(dev, dirent->d_name);
			printf("\nDevice: %s\n", dev);
		}
		(void) closedir (dir);
	} else {
		return 0;
	}

	return 1;
}

int readTemperature(int fd, float* tempC) {
	char buf[256];
	char tmpData[6];   // Temp C * 1000 reported by device
	int numRead;

	if ( (numRead = read(fd, buf, 256)) <= 0 ) {
		return 0;
	}

	strncpy(tmpData, strstr(buf, "t=") + 2, 5);
	*tempC = strtof(tmpData, NULL) / 1000;

	return 1;
}

int handleTempSens(const char* devPath, int sensNum, tel_termo* term) {
	int fd = open(devPath, O_RDONLY);
	if(fd == -1) {
		perror ("Couldn't open the w1 device.");
		return 0;
	}

	float tempC;

	if( readTemperature(fd, &tempC) ) {
		printf("Device: %s  - ", devPath);
		printf("Temp: %.3f C  \n", tempC);
	} else {
		perror("readTemperature");
	}

	term->sens_temp[sensNum] = tempC;

	close(fd);

	return 1;
}

int main (void) {
	int TERM_SENS_COUNT = 2;

	char* TERM_SENS[TERM_SENS_COUNT] = {"28-00000889fcf8",
					  	  	  	  	  	"28-0000088a5c9c"};

	char devPath[TERM_SENS_COUNT][128]; // Path to device

	/*if ( !searchDevice(dev) ) {
		perror ("Couldn't open the w1 devices directory");
		return 1;
	}*/

	dataService* dserv = new redisDataService();
	dserv->connect();

	// Assemble path to OneWire device
	for (int i = 0; i < TERM_SENS_COUNT; i++) {
		sprintf(devPath[i], "%s/%s/w1_slave", path, TERM_SENS[i]);
	}

	// Read temp continuously
	// Opening the device's file triggers new reading
	tel_termo term;
	while(1) {
		for (int i = 0; i < TERM_SENS_COUNT; i++) {
			if( !handleTempSens(devPath[i], i, &term) ) {
				sleep(1);
				continue;
			}
		}

		dserv->add(TYPE_TERMO, (char*)&term, sizeof(term));
	}
		/* return 0; --never called due to loop */
}
