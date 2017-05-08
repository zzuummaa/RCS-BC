/*
 * spi_raspi.h
 *
 *  Created on: 16 апр. 2017 г.
 *      Author: Cntgfy
 */

#ifndef SRC_SPI_RASPI_H_
#define SRC_SPI_RASPI_H_

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int spi_init(const char* device);

int spi_transfer(int fd, uint8_t* tx, uint8_t* rx, int length);

void spi_close(int fd);

#endif /* SRC_SPI_RASPI_H_ */
