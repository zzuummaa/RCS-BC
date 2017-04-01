/*
 * transceiver.cpp
 *
 *  Created on: 1 апр. 2017 г.
 *      Author: Cntgfy
 */

#include "transceiver.h"
#include <stddef.h>

int main_last() {
	RFM22B *myRadio = new RFM22B("/dev/spidev1.0");
	myRadio->setMaxSpeedHz(100000);

	printf("Initial Settings...\n");
	printf("\tFrequency is %.3fMHz\n",myRadio->getCarrierFrequency()/1E6f);
	printf("\tFH Step is %.3fkHz\n",myRadio->getFrequencyHoppingStepSize()/1E3f);
	printf("\tChannel is %d\n",myRadio->getChannel());
	printf("\tFrequency deviation is %.3fkHz\n",myRadio->getFrequencyDeviation()/1E3f);
	printf("\tData rate is %.3fkbps\n",myRadio->getDataRate()/1E3f);
	printf("\tModulation Type %d\n",myRadio->getModulationType());
	printf("\tModulation Data Source %d\n",myRadio->getModulationDataSource());
	printf("\tData Clock Configuration %d\n",myRadio->getDataClockConfiguration());
	printf("\tTransmission Power is %ddBm\n",myRadio->getTransmissionPower());
	printf("\tGPIO0 Function is 0x%.2X\n",myRadio->getGPIOFunction(RFM22B::GPIO0));
	printf("\tGPIO1 Function is 0x%.2X\n",myRadio->getGPIOFunction(RFM22B::GPIO1));
	printf("\tGPIO2 Function is 0x%.2X\n",myRadio->getGPIOFunction(RFM22B::GPIO2));

	myRadio->setCarrierFrequency(869E6);
	myRadio->setFrequencyHoppingStepSize(20000);
	myRadio->setChannel(3);
	myRadio->setFrequencyDeviation(62500);
	myRadio->setDataRate(15000);
	myRadio->setModulationType(RFM22B::GFSK);
	myRadio->setModulationDataSource(RFM22B::FIFO);
	myRadio->setDataClockConfiguration(RFM22B::NONE);
	myRadio->setTransmissionPower(11);
	myRadio->setGPIOFunction(RFM22B::GPIO0, RFM22B::TX_STATE);
	myRadio->setGPIOFunction(RFM22B::GPIO1, RFM22B::RX_STATE);
	myRadio->setGPIOFunction(RFM22B::GPIO2, RFM22B::CLEAR_CHANNEL_ASSESSMENT);

	printf("\nNew Settings...\n");
	printf("\tFrequency is %.3fMHz\n",myRadio->getCarrierFrequency()/1E6f);
	printf("\tFH Step is %.3fkHz\n",myRadio->getFrequencyHoppingStepSize()/1E3f);
	printf("\tChannel is %d\n",myRadio->getChannel());
	printf("\tFrequency deviation is %.3fkHz\n",myRadio->getFrequencyDeviation()/1E3f);
	printf("\tData rate is %.3fkbps\n",myRadio->getDataRate()/1E3f);
	printf("\tModulation Type %d\n",myRadio->getModulationType());
	printf("\tModulation Data Source %d\n",myRadio->getModulationDataSource());
	printf("\tData Clock Configuration %d\n",myRadio->getDataClockConfiguration());
	printf("\tTransmission Power is %ddBm\n",myRadio->getTransmissionPower());
	printf("\tGPIO0 Function is 0x%.2X\n",myRadio->getGPIOFunction(RFM22B::GPIO0));
	printf("\tGPIO1 Function is 0x%.2X\n",myRadio->getGPIOFunction(RFM22B::GPIO1));
	printf("\tGPIO2 Function is 0x%.2X\n",myRadio->getGPIOFunction(RFM22B::GPIO2));

	myRadio->close();

	return 0;
}

int transceiver::init() {
	myRadio->setMaxSpeedHz(100000);

	myRadio->setCarrierFrequency(869E6);
	myRadio->setFrequencyHoppingStepSize(20000);
	myRadio->setChannel(3);
	myRadio->setFrequencyDeviation(62500);
	myRadio->setDataRate(15000);
	myRadio->setModulationType(RFM22B::GFSK);
	myRadio->setModulationDataSource(RFM22B::FIFO);
	myRadio->setDataClockConfiguration(RFM22B::NONE);
	myRadio->setTransmissionPower(11);
	myRadio->setGPIOFunction(RFM22B::GPIO0, RFM22B::TX_STATE);
	myRadio->setGPIOFunction(RFM22B::GPIO1, RFM22B::RX_STATE);
	myRadio->setGPIOFunction(RFM22B::GPIO2, RFM22B::CLEAR_CHANNEL_ASSESSMENT);

	return 1;
}

transceiver::transceiver(char* devName) {
	this->myRadio = new RFM22B(devName);
}

void transceiver::send(char* data, int dataSize) {
	myRadio->send((uint8_t*)data, dataSize);
}

int transceiver::receive(char* data, int timeout) {
	return myRadio->receive((unsigned char*)data, RFM22B::MAX_PACKET_LENGTH, timeout);
}

int transceiver::transferAsSlave(char* inData, int inDataSize, char* outData, int waitTime) {
	if ( !receive(outData, waitTime) ) {
		return 0;
	}

	send(inData, inDataSize);

	return 1;
}

int transceiver::transferAsMaster(char* inData, int inDataSize, char* outData, int waitTime) {
	send(inData, inDataSize);

	if ( !receive(outData, waitTime) ) {
		return 0;
	}

	return 1;
}
