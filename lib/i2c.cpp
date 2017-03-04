/*
 * i2c.cpp
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

#define DEV_ADDRESS 0x5c
#define DEV_REGISTERS_NUM 256
int main()
{
    int i;
    char buf[DEV_REGISTERS_NUM];
    const char * devName = "/dev/i2c-1";

    // Open up the I2C bus
    int file = open(devName, O_RDWR);
    if (file == -1)
    {
        perror(devName);
        exit(1);
    }

    // Specify the address of the slave device.
    if (ioctl(file, I2C_SLAVE, DEV_ADDRESS) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
        exit(1);
    }

    // Write a byte to the slave.
    buf[0] = 0;
    if (write(file, buf, 1) != 1)
    {
        perror("Failed to write to the i2c bus");
        exit(1);
    }

    // Read a byte from the slave.
    if (read(file,buf,DEV_REGISTERS_NUM) != DEV_REGISTERS_NUM)
    {
        perror("Failed to read from the i2c bus");
        exit(1);
    }

    int j;
    for(i=0;i<DEV_REGISTERS_NUM;i++)
        printf("0x%02X ", buf[i]);

    printf("\n");

    return 0;
}
