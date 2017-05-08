/*
 * rfmTest.c
 *
 *  Created on: 16 апр. 2017 г.
 *      Author: Cntgfy
 */

#include "spi.h"
#include "sys/types.h"
#include "string.h"

static char* dev = "/dev/spidev0.0";
static uint8_t reg;

#include "rfm22b.h"
#include "rfm22b_enums.h"

int main_rfm_setup(int argc, char* argv[]);
int main_spi(int argc, char* argv[]);
int main_rfm_receive(int argc, char* argv[]);
int main_rfm_send(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	return main_rfm_setup(argc, argv);
}

int main_rfm_send(int argc, char* argv[]) {
	// Initialise the radio
	RFM22B *myRadio = new RFM22B("/dev/spidev0.0");

	// Set the bus speed
	//myRadio->setMaxSpeedHz(200000);

	// Radio configuration
	myRadio->reset();
	/*myRadio->setCarrierFrequency(869.5E6);
	myRadio->setModulationType(RFM22B::GFSK);
	myRadio->setModulationDataSource(RFM22B::FIFO);
	myRadio->setDataClockConfiguration(RFM22B::NONE);
	myRadio->setTransmissionPower(5);*/
	myRadio->setGPIOFunction(RFM22B::GPIO1, RFM22B::RX_STATE);
	myRadio->setGPIOFunction(RFM22B::GPIO0, RFM22B::TX_STATE);

	// What header are we broadcasting
	/*myRadio->setTransmitHeader(123456789);*/

	char output[RFM22B::MAX_PACKET_LENGTH] = "Hello World!";
	printf("Sending '%s'\n", output);
	myRadio->send((uint8_t*)output, RFM22B::MAX_PACKET_LENGTH);
	myRadio->close();

	return 0;
}

int main_rfm_receive(int argc, char* argv[]) {
	// Initialise the radio
		RFM22B *myRadio = new RFM22B("/dev/spidev0.0");

		// Set the bus speed
		//myRadio->setMaxSpeedHz(200000);

		// Radio configuration
		/*myRadio->reset();
		myRadio->setCarrierFrequency(869.5E6);
		myRadio->setModulationType(RFM22B::GFSK);
		myRadio->setModulationDataSource(RFM22B::FIFO);
		myRadio->setDataClockConfiguration(RFM22B::NONE);
		myRadio->setTransmissionPower(5);*/
		myRadio->setGPIOFunction(RFM22B::GPIO1, RFM22B::RX_STATE);
		myRadio->setGPIOFunction(RFM22B::GPIO0, RFM22B::TX_STATE);

		// What header do we want?
		//myRadio->setCheckHeader(123456789);

		// Listen for a packet
		printf("Listening to a message...\n");

		char input[RFM22B::MAX_PACKET_LENGTH];
		myRadio->receive((uint8_t*)input, RFM22B::MAX_PACKET_LENGTH);
		printf("Received string: '%s'\n", input);

		myRadio->close();

		return 0;
}

int main_rfm_setup(int argc, char* argv[]) {
	RFM22B *myRadio = new RFM22B("/dev/spidev0.0");

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

int main_spi(int argc, char* argv[]) {
	uint8_t tx[2] = {0x00, 0x00};

	if (argc > 2) {
		reg = atoi(argv[2]);
	} else {
		reg = 0x00;
	}

	if (argc > 1) {
		if ( !strcmp("-s", argv[1]) ) {
			tx[0] = reg | (1<<7);
			if ( argc > 3) {
				tx[1] = atoi(argv[3]);
			}
		}

		if ( !strcmp("-g", argv[1]) ) {
			tx[0] = reg;
		}
	} else {
		printf("Invalid input args\n");
		return 0;
	}

	SPI spi(dev);
	spi.setMaxSpeedHz(100000);

	printf("SPI initialized\n");

	uint8_t rx[sizeof(tx)] = {0x00, };

	if ( spi.transfer(tx, rx, sizeof(tx)) ) {
		printf("0x%.2X\n", rx[1]);
	} else {
		perror("transfer error");
	}

	return 0;
}
