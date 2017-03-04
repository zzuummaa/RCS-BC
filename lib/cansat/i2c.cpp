/*
 * i2c.cpp
 *
 *  Created on: 2 мар. 2017 г.
 *      Author: Cntgfy
 */

#include "i2c.h"

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <string.h>

I2C::I2C(int addr) {
	this->fd = -1;
	this->addr = addr;
	this->devName = new char[strlen(I2C_FILE_NAME)];
	strcpy(this->devName, I2C_FILE_NAME);
}

void I2C::init() {
	fd = open(devName, O_RDWR);
	if (fd == -1)
	{
		perror(devName);
		exit(1);
	}

	// Specify the address of the slave device.
	if (ioctl(fd, I2C_SLAVE, addr) < 0)
	{
		perror("Failed to acquire bus access and/or talk to slave");
		exit(1);
	}
}

static int set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value) {

    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;

    /* The first byte indicates which register we'll write */
    outbuf[0] = reg;

    /*
     * The second byte indicates the value to write.  Note that for many
     * devices, we can write multiple, sequential registers at once by
     * simply making outbuf bigger.
     */
    outbuf[1] = value;

    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }

    return 0;
}

static int get_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val) {
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     * In order to read a register, we first do a "dummy write" by writing
     * 0 bytes to the register we want to read from.  This is similar to
     * the packet in set_i2c_register, except it's 1 byte rather than 2.
     */
    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* The data will get returned in this structure */
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = sizeof(inbuf);
    messages[1].buf   = &inbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }
    *val = inbuf;

    return 0;
}

int I2C::writeByte(uint8_t subAddr, uint8_t data) {
	return set_i2c_register(fd, addr, subAddr, data);
}

int I2C::readByte(uint8_t subAddr) {
	uint8_t byt;

	int status = get_i2c_register(fd, addr, subAddr, &byt );
	if (status == 1) {
		return -1;
	}

	return byt;
}

int I2C::readBytes(uint8_t subAddr, char* dst, int dataCount) {

	for (int i=0; i < dataCount; i++) {
		int status = get_i2c_register(fd, addr, subAddr+i, (uint8_t*)(dst+i) );
		if (status == 1) {
			return 0;
		}
	}

	return 1;
}
