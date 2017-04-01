/*
 * redisTest.cpp
 *
 *  Created on: 30 мар. 2017 г.
 *      Author: Cntgfy
 */

#include "shm/redis.h"

int main() {
	redisDataService* dserv = new redisDataService();

	if ( !dserv->connect() ) {
		return 1;
	}

	string key = "foo";
	string outData = "bar";

	if ( !dserv->add_(key, outData) ) {
		return 1;
	}

	string inData;

	if ( !dserv->get_(key, &inData) ) {
		return 1;
	}

	if (outData != inData) {
		return 1;
	}

	printf("Success\n");

	return 0;
}
