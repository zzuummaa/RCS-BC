/*
 * linux_wrapper.h
 *
 *  Created on: 21 февр. 2017 г.
 *      Author: Cntgfy
 */

#ifndef LINUX_WRAPPER_H_
#define LINUX_WRAPPER_H_

#include <stdint.h>

uint8_t SS_PIN;

unsigned long millis();

void sail_print_format_uint(char* data, int size, char* title, uint8_t val);

int32_t sail_strlen(char* str);

void console_print(char* data);

void io_bit_clear(uint8_t reg, int32_t bit);

void io_bit_set(uint8_t reg, int32_t bit, );

void SPI_begin();

uint8_t SPI_transfer();

void twi_writeTo();

void twi_readFrom();

#endif /* LINUX_WRAPPER_H_ */
