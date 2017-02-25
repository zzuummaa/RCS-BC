/*
 * linux_wrapper.cpp
 *
 *  Created on: 21 февр. 2017 г.
 *      Author: Cntgfy
 */

#include "linux_wrapper.h"

#include <chrono>

using namespace std::chrono;

unsigned long millis() {
	milliseconds curTime = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);

	return (unsigned long) curTime.count();
}

void sail_print_format_uint(char* data, int size, char* title, uint8_t val) {

}

void console_print(char* data) {

}

void io_bit_clear(uint8_t reg, int32_t bit) {

}
