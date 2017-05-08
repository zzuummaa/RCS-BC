/*
 * adapter.h
 *
 *  Created on: 7 мая 2017 г.
 *      Author: Cntgfy
 */

#ifndef ADAPTER_H_
#define ADAPTER_H_

#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

typedef unsigned char byte;
#define true 1
#define false 0

void delayMicroseconds(uint32_t us) {
	usleep(us);
}

uint64_t millis() {
	struct timeval *tv;
	struct timezone *tz;
	gettimeofday(tv, tz);

	return tv->tv_sec * 1000 + tv->tv_usec / 1000;
}

#endif /* ADAPTER_H_ */
