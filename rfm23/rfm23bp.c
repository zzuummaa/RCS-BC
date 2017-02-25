/* This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */


#include <stdio.h>
#include <math.h>

#include "time.h"
#include "rfm23bp.h"

#include "linux_wrapper.h"

#define RFM_TIMEOUT 300

static rfm23bp_params * internal_params;
rfm23bp_params params;
uint8_t RFM_DATA[RFM23_FIFO_LENGTH];

void SC18IS602_speed_max()
{
  uint8_t data1[2] = "";
  data1[0] = 0xF0;
  data1[1] = 0x00;
  twi_writeTo(internal_params->address, data1, 2, 1, 1);
}

/*
** COMMAND: test-rfm23bp
** 
** Test rfm23bps transceiver
*/
void rfm23bp_test_cmd()
{
  uint8_t i,reg;
  char data[20];
  //modem_on();
  
  io_bit_set(P_L,2,io_port);
  io_bit_set(P_L,2,io_ddr);
  

  rfm23bp_init(&params);
  rfm23bp_set_address(0x28);
  #if TEST_TRANSMITTER
  rfm23bp_mode_spi(RFM23_SLAVE_SELECT_PIN);
  #else
  SC18IS602_speed_max();
  #endif
  reg = rfm23bp_read_register(RFM23BP_DEVICE_VERSION);
  
  sail_print_format_uint(data,20,"VERSION=",reg);
  console_println(data);

  rfm23bp_ready();
  console_println("Driver started");
  rfm23bp_reset();
  #if TEST_TRANSMITTER
  console_println("Starting transmission");
  rfm23bp_set_default_radio_settings();
  rfm23bp_set_power_level(level_0);
  uint8_t aaa[65] = {};
  uint8_t k = 1;
  uint8_t iii,jjj;
  while(1){
    /*if (k>0x0F) k = 1;
    for(iii=0;iii<64;iii++)
    {
      aaa[iii] = k | (k << 4);
    }
    k++;*/

    for (iii=0;iii<LINE_COUNT;iii++)
    {
      for (jjj=0;jjj<COLUMN_COUNT;jjj++)
      {
        aaa[jjj] = 0;
        aaa[jjj] = pgm_read_byte(data_to_send[iii] + jjj);//(&(data_to_send[iii][jjj]));// (data_to_send + iii*COLUMN_COUNT + jjj);
      }
      rfm23bp_transmit(aaa,64);
      delay_abs(TEST_TRANSMITTER_INTERVAL);
    }
    //console_println("sent");
    //delay_abs(TEST_TRANSMITTER_INTERVAL);
    k = 1;
    for(iii=0;iii<64;iii++)
    {
      aaa[iii] = k | (k << 4);
      k++;
      if (k>0x0f) {k = 1;}
    }
    rfm23bp_transmit(aaa,64);
    delay_abs(TEST_TRANSMITTER_INTERVAL);
  }
  #else
  for(i=0;i<5;i++){
    reg = rfm23bp_read_temperature();
    float temp = 0.5*reg - 64.0;
    
    sail_print_format_uint(data,20,"REG=",reg);
    console_print(data);
    sail_print_format_double(data,20,", TEMP=",temp);
    console_print(data);
    console_println("C");
    
    delay_abs(300);
  }
  #endif
  console_println("Finished");
}


void rfm23bp_init(rfm23bp_params * params)
{
  internal_params = params;
  sail_memset(internal_params,0x00,sizeof *internal_params);
  sail_memcpy(internal_params->callsign,CALLSIGN,CALLSIGN_LENGTH);
  internal_params->callsign_length = CALLSIGN_LENGTH;
}

void rfm23bp_temperature_init()
{
  rfm23bp_enable_adc_input();
  rfm23bp_set_adc_reference();
  rfm23bp_set_temperature_range(m64_p64C);
  rfm23bp_enable_adc_reading();
}

void rfm23bp_set_callsign_length(uint8_t length)
{
  if (length>CALLSIGN_MAX_LENGTH)
    internal_params->callsign_length = CALLSIGN_MAX_LENGTH;
  else
    internal_params->callsign_length = length;
}

void rfm23bp_reset_callsign_length()
{
  internal_params->callsign_length = sail_strlen((char*)(internal_params->callsign));
//  if((internal_params->callsign_length) > CALLSIGN_MAX_LENGTH) internal_params->callsign_length = CALLSIGN_MAX_LENGTH;
}

void rfm23bp_set_callsign(uint8_t * callsign)
{
  if (sail_strlen((char*)callsign)<=CALLSIGN_MAX_LENGTH)
  {
    sail_strcpy((char*)(internal_params->callsign),(char*)(callsign));
    rfm23bp_reset_callsign_length();
  }
  else
  {
    sail_memcpy(internal_params->callsign,callsign,CALLSIGN_MAX_LENGTH);
    internal_params->callsign_length = CALLSIGN_MAX_LENGTH;
  }
}

void rfm23_reset_rx_fifo()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0x02);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0);
}

void rfm23bp_transmit(uint8_t* data, uint8_t data_length)
{
  rfm23_reset_rx_fifo();
  rfm23bp_write_data_to_fifo(data,data_length);
  rfm23bp_enable_tx();
  unsigned long start_time = millis();
  while(1)
  {
    if(millis() - start_time > RFM_TIMEOUT) break;
    {
      uint8_t STATUS_1;
      STATUS_1 = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
      if (bit_check(&STATUS_1,2)) break;
    }
  }
}


uint8_t rfm23bp_received()
{
  uint8_t reg;
  rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_2);
  reg = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
  if (bit_check(&reg,4))
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
  if(internal_params->spi_or_twi==0){
    uint8_t data[3] = "";
    data[0] = FUNCTION_ID;
    data[1] = reg | RH_SPI_WRITE_MASK;
    data[2] = value;
    return twi_writeTo(internal_params->address, data, 3, 1, 1);
  }
  else
  {
    io_bit_clear(P_C,1,io_port);//bit_clear(PORTE,7,io_port);
    SPI_transfer(reg | RH_SPI_WRITE_MASK);
    SPI_transfer(value);
    io_bit_set(P_C,1,io_port);//bit_set(PORTE,7,io_port);
    return 0;
  }
}


uint8_t rfm23bp_read_register(uint8_t reg)
{
  if(internal_params->spi_or_twi==0){
    uint8_t data[3] = "";
    uint8_t value;
    data[0] = FUNCTION_ID;
    data[1] = reg;
    data[2] = 0xFF;
    twi_writeTo(internal_params->address, data, 3, 1, 1);
    data[0] = 0;
    data[1] = 0;
    twi_readFrom(internal_params->address, data, 2, 1);
    value = data[1];
    return value;
  }
  else
  {
    io_bit_clear(P_C,1,io_port);//bit_clear(PORTE,7,io_port);
    SPI_transfer(reg);
    uint8_t value = SPI_transfer(0);
    io_bit_set(P_C,1,io_port);//bit_set(PORTE,7,io_port);
    return value;
  }
}

void rfm23bp_enable_rx()
{
  /*uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  bit_set(creg,2);
  bit_clear(creg,3);*/
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x04);//creg);
  //rfm23bp_set_register(RFM23BP_GPIO0_CONFIGURATION,0xD5);
}


void rfm23bp_enable_tx()
{
  /*uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  bit_set(creg,3);
  bit_clear(creg,2);*/
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
  bit_set(&creg,6);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,creg);
}

void rfm23bp_ready()
{
  uint8_t creg = rfm23bp_read_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1);
  bit_set(&creg,0);
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,creg);
}


void rfm23bp_mode_spi(uint8_t slave_select)
{
  internal_params->slave_select_pin = slave_select;
  internal_params->spi_or_twi = 1;
  SS_PIN = internal_params->slave_select_pin;
  SPI_begin();
  io_bit_set(P_C,1,io_ddr);//bit_set(DDRE,7,io_ddr);
  io_bit_set(P_C,1,io_port);//bit_set(PORTE,7,io_port);
}

void rfm23bp_reset()
{
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x80);
  delay_abs(1);
}

void rfm23bp_enable_adc_input()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  bit_clear(&reg,4);
  bit_clear(&reg,5);
  bit_clear(&reg,6);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

void rfm23bp_set_adc_reference()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  bit_clear(&reg,2);
  bit_clear(&reg,3);
  rfm23bp_set_register(RFM23BP_ADC_CONFIGURATION,reg);
}

void rfm23bp_set_temperature_range(rfm23bp_temperature_range range)
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_TEMPERATURE_SENSOR_CONTROL);
  if(range<3)
  {
    reg &= (range << 6 | 0x3F);
    bit_set(&reg,5);
  }
  else if(range==3)
  {
    bit_set(&reg,7);
    bit_set(&reg,6);
    bit_set(&reg,5);  }
  else
  {/* Отключено предупреждение PVS V525, так как здесь идет
    * последовательное выставление битов */
    bit_set(&reg,7);//-V525
    bit_clear(&reg,6);//-V525
    bit_clear(&reg,5);//-V525
  }
  
  rfm23bp_set_register(RFM23BP_TEMPERATURE_SENSOR_CONTROL,reg);
}

void rfm23bp_enable_adc_reading()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_ADC_CONFIGURATION);
  bit_set(&reg,7);
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
  if(bit_check((uint8_t*)&level,0)) bit_set(&reg,0); else bit_clear(&reg,0);
  if(bit_check((uint8_t*)&level,1)) bit_set(&reg,1); else bit_clear(&reg,1);
  if(bit_check((uint8_t*)&level,2)) bit_set(&reg,2); else bit_clear(&reg,2);
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

void rfm23bp_transmit_callsign()
{
  rfm23bp_set_ook_modulation();
  rfm23bp_write_data_to_fifo(internal_params->callsign,internal_params->callsign_length);
}

uint8_t rfm23bp_get_rx_fifo()
{
  uint8_t i;
  
  uint8_t rssi = rfm23bp_read_register(RFM23BP_RECEIVED_SIGNAL_STRENGTH_INDICATOR);

  for (i=0;i<RFM23_FIFO_LENGTH;i++)
    RFM_DATA[i] = rfm23bp_read_register(RFM23BP_FIFO_ACCESS);
  /**
   * Отключено предупреждение PVS V525, так как нужно устанавливать
   * регистры в данном порядке с данными значениями,
   * согласно даташиту
   */
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0x02);//-V525
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2, 0);//-V525
  rfm23bp_set_register(RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1,0x04);//-V525
  
  return rssi;
}

uint8_t rfm23bp_tx_available()
{
  uint8_t reg = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
  rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_2);
  if (bit_check(&reg,2))
    return 1;
  else return 0;
}


