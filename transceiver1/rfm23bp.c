#include <stdio.h>
#include <string.h>
#include <math.h>

#include "rfm23bp.h"

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))


#define RH_SPI_WRITE_MASK                                  0x80

#define RFM23BP_TIMEOUT                                    100



const char* SPI_DEV_NAME = "/dev/spidev0.0";
int fd;


static rfm23bp_params * internal_params;
rfm23bp_params params;
uint8_t RFM_DATA[RFM23_FIFO_LENGTH];

void rfm23bp_init(rfm23bp_params * params)
{
  internal_params = params;
  memset(internal_params,0x00,sizeof *internal_params);
  }

void rfm23bp_temperature_init()
{
  rfm23bp_enable_adc_input();
  rfm23bp_set_adc_reference();
  rfm23bp_set_temperature_range(m64_p64C);
  rfm23bp_enable_adc_reading();
}

void rfm23_reset_rx_fifo()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0x02);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0);
}

void rfm23bp_transmit(uint8_t* data, uint8_t data_length)
{
  rfm23_reset_rx_fifo();
  //while (!rfm23bp_tx_available()){};
  rfm23bp_write_data_to_fifo(data,data_length);
  rfm23bp_enable_tx();
}


uint8_t rfm23bp_received()
{
  uint8_t reg;
  rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_2);
  reg = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
  if(reg=255) return 0;
  if ((BIT_CHECK(reg,4))/* && (BIT_CHECK(reg,4))*/)
    return 1;
  else return 0;
}

void rfm23bp_set_address(uint8_t address)
{
  if (address<=MAX_I2C_ADDRESS)
    internal_params->address = address;
  else internal_params->address = 0;
}

uint8_t rfm23bp_set_register(uint8_t reg,uint8_t value)
{
    uint8_t tx[] = {reg | RH_SPI_WRITE_MASK, value};
	uint8_t rx[] = {0x00, 0x00};
	spi_transfer(fd, tx, rx, 2);

    return 0;
}


uint8_t rfm23bp_read_register(uint8_t reg)
{
    uint8_t tx[] = {reg, 0x00};
    uint8_t rx[] = {0x00, 0x00};
    spi_transfer(fd, tx, rx, 2);

    return rx[1];
}

void rfm23bp_enable_rx()
{
  /*uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  BIT_SET(creg,2);
  BIT_CLEAR(creg,3);*/
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x04);//creg);
  //rfm23bp_set_register(RFM23BP_GPIO0_CONFIGURATION,0xD5);
}


void rfm23bp_enable_tx()
{
  /*uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  BIT_SET(creg,3);
  BIT_CLEAR(creg,2);*/
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x09);//creg);
  //rfm23bp_set_register(RFM23BP_GPIO1_CONFIGURATION,0xD2);
}


void rfm23bp_standby()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0);
}


void rfm23bp_sleep()
{
  uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  BIT_SET(creg,6);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,creg);
}

void rfm23bp_ready()
{
  uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  BIT_SET(creg,0);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,creg);
}


void rfm23bp_mode_spi(uint8_t slave_select)
{
  internal_params->slave_select_pin = slave_select;
  internal_params->spi_or_twi = 1;
  //SS_PIN = internal_params->slave_select_pin;
  fd = spi_init(SPI_DEV_NAME);
  if (fd < 0) {
	  perror("Can't open spi device\n");
	  exit(1);
  }
}

void rfm23bp_reset()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x80);
  delayMicroseconds(1);
}

void rfm23bp_enable_adc_input()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  BIT_CLEAR(reg,4);
  BIT_CLEAR(reg,5);
  BIT_CLEAR(reg,6);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

void rfm23bp_set_adc_reference()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  BIT_CLEAR(reg,2);
  BIT_CLEAR(reg,3);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

void rfm23bp_set_temperature_range(rfm23bp_temperature_range range)
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_TEMPERATURE_SENSOR_CONTROL);
  if(range<3)
  {
    reg &= (range << 6 | 0x3F);
    BIT_SET(reg,5);
  }
  else if(range==3)
  {
    BIT_SET(reg,7);
    BIT_SET(reg,6);
    BIT_SET(reg,5);  }
  else
  {
    BIT_SET(reg,7);
    BIT_CLEAR(reg,6);
    BIT_CLEAR(reg,5);
  }
  
  rfm23bp_set_register(RFM23BP_TEMPERATURE_SENSOR_CONTROL,reg);
}

void rfm23bp_enable_adc_reading()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  BIT_SET(reg,7);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

uint8_t rfm23bp_read_temperature()
{
  rfm23bp_temperature_init();
  uint64_t time = millis();
  while ((!(rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION) & 0x80)) && ((millis()-time)<RFM_TIMEOUT));
  return rfm23bp_read_register(RFM23BP_ADC_VALUE);
}

void rfm23bp_set_power_level(rfm23bp_power_level level)
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_TX_POWER);
  if(BIT_CHECK(level,0)) BIT_SET(reg,0); else BIT_CLEAR(reg,0);
  if(BIT_CHECK(level,1)) BIT_SET(reg,1); else BIT_CLEAR(reg,1);
  if(BIT_CHECK(level,2)) BIT_SET(reg,2); else BIT_CLEAR(reg,2);
  rfm23bp_set_register(RFM23BP_TX_POWER,reg);
}

/*
** Установить параметры для несущей частоты 433МГц (см. калькулятор регистров)
*/
void rfm23bp_set_default_carrier_frequency_433()
{
  rfm23bp_set_register(RFM23BP_FREQUENCY_BAND_SELECT,0x53);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_1,0x4B);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_0,0x00);
}

/*
** Установить параметры для несущей частоты 435МГц (см. калькулятор регистров)
*/
void rfm23bp_set_default_carrier_frequency_435()
{
  rfm23bp_set_register(RFM23BP_FREQUENCY_BAND_SELECT,0x53);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_1,0x7D);
  rfm23bp_set_register(RFM23BP_NOMINAL_CARRIER_FREQUENCY_0,0x00);
}


void rfm23bp_set_default_frequency_deviation()
{
  rfm23bp_set_register(RFM23BP_FREQUENCY_DEVIATION,0x20);
}

void rfm23bp_set_default_modulation()
{
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_1,0x2C);
//  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_2,0x23);
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_2,0x22);
}

void rfm23bp_set_default_baudrate()
{
//  rfm23bp_set_register(RFM23BP_TX_DATA_RATE_1,0x09);
  rfm23bp_set_register(RFM23BP_TX_DATA_RATE_1,0x4E);
  //rfm23bp_set_register(RFM23BP_TX_DATA_RATE_0,0xD5);
  rfm23bp_set_register(RFM23BP_TX_DATA_RATE_0,0xA5);
}

void rfm23bp_set_default_header_settings()
{
  rfm23bp_set_register(RFM23BP_TRANSMIT_PACKET_LENGTH,0x40);
  rfm23bp_set_register(RFM23BP_HEADER_CONTROL_2,0x0A);
  rfm23bp_set_register(RFM23BP_HEADER_CONTROL_1,0x8C);
  rfm23bp_set_register(RFM23BP_DATA_ACCESS_CONTROL,0xA8);
  
  rfm23bp_set_register(RFM23BP_PREAMBLE_DETECTION_CONTROL,0x22);
  
  rfm23bp_set_register(RFM23BP_RX_FIFO_CONTROL,63);//31);//63
}

void rfm23bp_set_default_rx_settings()
{
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OVERSAMPLING_RATIO,0x68);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OFFSET_2,0x01);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OFFSET_1,0x3A);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_OFFSET_0,0x93);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1,0x01);
  rfm23bp_set_register(RFM23BP_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0,0x30);
  rfm23bp_set_register(RFM23BP_AFC_LIMITER,0x1E);
  rfm23bp_set_register(RFM23BP_IF_FILTER_BANDWIDTH,0x2E);

}

void rfm23bp_set_default_radio_settings()
{
//  rfm23bp_set_default_carrier_frequency_435();
  rfm23bp_set_default_carrier_frequency_433();
  rfm23bp_set_default_frequency_deviation();
  rfm23bp_set_default_modulation();
  rfm23bp_set_default_baudrate();
  rfm23bp_set_default_header_settings();
  rfm23bp_set_default_rx_settings();

  
  rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_1,0x14);
  rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_2,0x00);
}

void rfm23bp_set_ook_modulation()
{
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_1,0x24);
  rfm23bp_set_register(RFM23BP_MODULATION_MODE_CONTROL_2,0x21);
}


void rfm23bp_write_byte_to_fifo(uint8_t data)
{
  rfm23bp_set_register(RFM23BP_FIFO_ACCESS,data);
}

uint8_t rfm23bp_read_byte_from_fifo()
{
  return rfm23bp_read_register(RFM23BP_FIFO_ACCESS);
}

void rfm23bp_write_data_to_fifo(uint8_t * data, uint8_t length)
{
  uint8_t i;
  if (length>RFM23_FIFO_LENGTH) return;
  for (i=0;i<length;i++)
    rfm23bp_set_register(RFM23BP_FIFO_ACCESS,data[i]);
}

uint8_t rfm23bp_get_rx_fifo()
{
  uint8_t i;
  
  uint8_t rssi = rfm23bp_read_register(RFM23BP_RECEIVED_SIGNAL_STRENGTH_INDICATOR);

  ///*

  uint8_t data[RFM23_FIFO_LENGTH+3] = "";
    
    for (i=0;i<RFM23_FIFO_LENGTH;i++)
      RFM_DATA[i] = rfm23bp_read_register(RFM23BP_FIFO_ACCESS);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0x02);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0);
  
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x04);
  
  return rssi;
}

uint8_t rfm23bp_read_rx_fifo(uint8_t* data) {
	for (int i=0;i<RFM23_FIFO_LENGTH;i++)
		data[i] = rfm23bp_read_register(RFM23BP_FIFO_ACCESS);
	rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0x02);
	rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0);

	rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x04);

	return 1;
}

uint8_t rfm23bp_transmite(uint8_t* data, uint8_t len) {
	if (len > 64) {
		return 0;
	}

	rfm23bp_write_data_to_fifo(data, len);
	rfm23bp_enable_tx();

	while(!rfm23bp_tx_available()) {
		delayMicroseconds(200);
	}

	return 1;
}

uint8_t rfm23bp_receive(uint8_t* data, int timeout) {
	// Timing for the interrupt loop timeout
	struct timeval start, end;
	gettimeofday(&start, NULL);
	long elapsed = 0;

	int regControl;

	rfm23bp_enable_rx();

	// Loop endlessly on interrupt or timeout
	//	Don't use interrupt registers here as these don't seem to behave consistently
	//	Watch the operating mode register for the device leaving RX mode. This is indicitive
	//	of a valid packet being received
	while (elapsed < timeout) {
		regControl = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
		if (regControl>>1 & 1) break;

		// Determine elapsed time
		gettimeofday(&end, NULL);
		elapsed = (end.tv_usec - start.tv_usec)/1000 + (end.tv_sec - start.tv_sec)*1000;
		delayMicroseconds(200);
	}

	if ( (regControl>>1 & 1) == 0 ) return 0;

	usleep(5000);
	return rfm23bp_read_rx_fifo(data);
}

uint8_t rfm23bp_tx_available()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
  rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_2);
  if (BIT_CHECK(reg,2))
    return 1;
  else return 0;
}

