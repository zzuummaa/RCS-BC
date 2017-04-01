/*
 * transceiver.h
 *
 *  Created on: 1 апр. 2017 г.
 *      Author: Cntgfy
 */

#ifndef CANSAT_TRANSCEIVER_H_
#define CANSAT_TRANSCEIVER_H_

#include "rfm/rfm22b.h"

class transceiver {
private:
	RFM22B *myRadio;

public:
	transceiver(char* devName);

	int init();

	void send(char* data, int dataSize);
	int receive(char* data, int timeout);

	//int transfer(char* inData, int inDataSize, char* outData);
	int transferAsSlave(char* inData, int inDataSize, char* outData, int waitTime);
	int transferAsMaster(char* inData, int inDataSize, char* outData, int waitTime);
};


#endif /* CANSAT_TRANSCEIVER_H_ */
