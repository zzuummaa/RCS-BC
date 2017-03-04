/*
 * i2c.h
 *
 *  Created on: 1 мар. 2017 г.
 *      Author: Cntgfy
 */

#ifndef CANSAT_I2C_H_
#define CANSAT_I2C_H_

#include <stdint.h>

#define I2C_FILE_NAME "/dev/i2c-1"

class I2C {
private:
	char* devName;
	int fd;
	unsigned char addr;
public:
	I2C(int addr);
	I2C(char* devName, int addr);
	void init();
	int writeByte(uint8_t subAddr, uint8_t data);
	int readByte(uint8_t subAddr);
	int readBytes(uint8_t subAddr, char* dst, int dataCount);
};



#endif /* CANSAT_I2C_H_ */
