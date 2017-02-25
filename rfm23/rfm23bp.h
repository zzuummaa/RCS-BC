#ifndef rfm23bp_H
#define rfm23bp_H

#include <inttypes.h>

uint8_t rfm23bp_tx_available();
uint8_t rfm23bp_get_rx_fifo();
void rfm23bp_transmit_callsign();
uint8_t rfm23bp_read_byte_from_fifo();
void rfm23bp_write_byte_to_fifo(uint8_t data);
void rfm23bp_set_ook_modulation();
void rfm23bp_set_default_rx_settings();
void rfm23bp_set_default_header_settings();
void rfm23bp_set_default_baudrate();
void rfm23bp_set_default_modulation();
void rfm23bp_set_default_frequency_deviation();
void rfm23bp_set_default_carrier_frequency_435();
void rfm23bp_set_default_carrier_frequency_433();
void rfm23bp_sleep();
void rfm23bp_standby();
void rfm23bp_enable_rx();
uint8_t rfm23bp_received();
void rfm23bp_enable_tx();
void rfm23bp_write_data_to_fifo(uint8_t *data,uint8_t length);
uint8_t rfm23bp_set_register(uint8_t reg,uint8_t value);
void rfm23_reset_rx_fifo();
void rfm23bp_set_callsign(uint8_t *callsign);
void rfm23bp_reset_callsign_length();
void rfm23bp_set_callsign_length(uint8_t length);
void rfm23bp_enable_adc_reading();
typedef enum {
  m64_p64C,
  m64_p192C,
  m0_p128C,
  m40_p216F,
  p0_p341K
}rfm23bp_temperature_range;
void rfm23bp_set_temperature_range(rfm23bp_temperature_range range);
void rfm23bp_set_adc_reference();
void rfm23bp_enable_adc_input();
void rfm23bp_temperature_init();
uint8_t rfm23bp_read_temperature();
void rfm23bp_transmit(uint8_t* data, uint8_t data_length);
typedef enum {
  level_0=0, // -8dBm
  level_1, // -5dBm
  level_2, // -2dBm
  level_3, // 1dBm
  level_4, // 4dBm
  level_5, // 28dBm
  level_6, // 29dBm
  level_7, // 30dBm
}rfm23bp_power_level;
void rfm23bp_set_power_level(rfm23bp_power_level level);
void rfm23bp_set_default_radio_settings();
void rfm23bp_reset();
void rfm23bp_ready();
void console_println(const char *text);
uint8_t rfm23bp_read_register(uint8_t reg);
void rfm23bp_mode_spi(uint8_t slave_select);
void rfm23bp_set_address(uint8_t address);
#define CALLSIGN_MAX_LENGTH                                20
typedef struct {
  uint8_t callsign[CALLSIGN_MAX_LENGTH];
  uint8_t callsign_length;
  uint8_t address;
  uint8_t spi_or_twi;
  uint8_t slave_select_pin;
}rfm23bp_params;
extern rfm23bp_params params;
void rfm23bp_init(rfm23bp_params *params);
void rfm23bp_test_cmd();
void SC18IS602_speed_max();
#define RFM23_FIFO_LENGTH                                  64
extern uint8_t RFM_DATA[RFM23_FIFO_LENGTH];
#define TEST_TRANSMITTER                                   0
#if TEST_TRANSMITTER
#define LINE_COUNT   47
#define COLUMN_COUNT 64
extern const char data_to_send[LINE_COUNT][COLUMN_COUNT]PROGMEM;
#endif
#define RFM23_SLAVE_SELECT_PIN                             51
#define MAX_I2C_ADDRESS                                    0x7F
#define FUNCTION_ID                                        0x01
#define RFM23BP_FIFO_ACCESS                                0x7F
#define RFM23BP_RX_FIFO_CONTROL                            0x7E
#define RFM23BP_TX_FIFO_CONTROL_2                          0x7D
#define RFM23BP_TX_FIFO_CONTROL_1                          0x7C
#define RFM23BP_FREQUENCY_HOPING_STEP_SIZE                 0x7A
#define RFM23BP_FREQUENCY_HOPING_CHANNEL_SELECT            0x79
#define RFM23BP_NOMINAL_CARRIER_FREQUENCY_0                0x77
#define RFM23BP_NOMINAL_CARRIER_FREQUENCY_1                0x76
#define RFM23BP_FREQUENCY_BAND_SELECT                      0x75
#define RFM23BP_FREQUENCY_OFFSET_2                         0x74
#define RFM23BP_FREQUENCY_OFFSET_1                         0x73
#define RFM23BP_FREQUENCY_DEVIATION                        0x72
#define RFM23BP_MODULATION_MODE_CONTROL_2                  0x71
#define RFM23BP_MODULATION_MODE_CONTROL_1                  0x70
#define RFM23BP_TX_DATA_RATE_0                             0x6F
#define RFM23BP_TX_DATA_RATE_1                             0x6E
#define RFM23BP_TX_POWER                                   0x6D
#define RFM23BP_AGC_OVERRIDE_1                             0x69
#define RFM23BP_CRYSTAL_OSCILLATOR_CONTROL_TEST            0x62
#define RFM23BP_CHANNEL_FILTER_COEFFICIENT_ADDRESS         0x60
#define RFM23BP_ADC8_CONTROL                               0x4F
#define RFM23BP_RECEIVED_PACKET_LENGTH                     0x4B
#define RFM23BP_RECEIVED_HEADER_0                          0x4A
#define RFM23BP_RECEIVED_HEADER_1                          0x49
#define RFM23BP_RECEIVED_HEADER_2                          0x48
#define RFM23BP_RECEIVED_HEADER_3                          0x47
#define RFM23BP_HEADER_ENABLE_0                            0x46
#define RFM23BP_HEADER_ENABLE_1                            0x45
#define RFM23BP_HEADER_ENABLE_2                            0x44
#define RFM23BP_HEADER_ENABLE_3                            0x43
#define RFM23BP_CHECK_HEADER_0                             0x42
#define RFM23BP_CHECK_HEADER_1                             0x41
#define RFM23BP_CHECK_HEADER_2                             0x40
#define RFM23BP_CHECK_HEADER_3                             0x3F
#define RFM23BP_TRANSMIT_PACKET_LENGTH                     0x3E
#define RFM23BP_TRANSMIT_HEADER_0                          0x3D
#define RFM23BP_TRANSMIT_HEADER_1                          0x3C
#define RFM23BP_TRANSMIT_HEADER_2                          0x3B
#define RFM23BP_TRANSMIT_HEADER_3                          0x3A
#define RFM23BP_SYNC_WORD_0                                0x39
#define RFM23BP_SYNC_WORD_1                                0x38
#define RFM23BP_SYNC_WORD_2                                0x37
#define RFM23BP_SYNC_WORD_3                                0x36
#define RFM23BP_PREAMBLE_DETECTION_CONTROL                 0x35
#define RFM23BP_PREAMBLE_LENGTH                            0x34
#define RFM23BP_HEADER_CONTROL_2                           0x33
#define RFM23BP_HEADER_CONTROL_1                           0x32
#define RFM23BP_EZMAC_STATUS                               0x31
#define RFM23BP_DATA_ACCESS_CONTROL                        0x30
#define RFM23BP_SLICER_PEAK_HOLD                           0x2E
#define RFM23BP_OOK_COUNTER_VALUE_2                        0x2D
#define RFM23BP_OOK_COUNTER_VALUE_1                        0x2C
#define RFM23BP_AFC_CORRECTION_READ                        0x2B
#define RFM23BP_AFC_LIMITER                                0x2A
#define RFM23BP_ANTENNA_DIVERSITY_REGISTER_2               0x29
#define RFM23BP_ANTENNA_DIVERSITY_REGISTER_1               0x28
#define RFM23BP_RSSI_THRESHOLD_FOR_CLEAR_CHANNEL_INDICATOR 0x27
#define RFM23BP_RECEIVED_SIGNAL_STRENGTH_INDICATOR         0x26
#define RFM23BP_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0          0x25
#define RFM23BP_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1          0x24
#define RFM23BP_CLOCK_RECOVERY_OFFSET_0                    0x23
#define RFM23BP_CLOCK_RECOVERY_OFFSET_1                    0x22
#define RFM23BP_CLOCK_RECOVERY_OFFSET_2                    0x21
#define RFM23BP_CLOCK_RECOVERY_OVERSAMPLING_RATIO          0x20
#define RFM23BP_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE          0x1F
#define RFM23BP_AFC_TIMING_CONTROL                         0x1E
#define RFM23BP_AFC_LOOP_GEARSHIFT_OVERRIDE                0x1D
#define RFM23BP_IF_FILTER_BANDWIDTH                        0x1C
#define RFM23BP_BATTERY_VOLTAGE_LEVEL                      0x1B
#define RFM23BP_LOW_BATTERY_DETECTOR_THRESHOLD             0x1A
#define RFM23BP_LOW_DUTY_CYCLE_MODE_DURATION               0x19
#define RFM23BP_WAKE_UP_TIMER_VALUE_2                      0x18
#define RFM23BP_WAKE_UP_TIMER_VALUE_1                      0x17
#define RFM23BP_WAKE_UP_TIMER_PERIOD_3                     0x16
#define RFM23BP_WAKE_UP_TIMER_PERIOD_2                     0x15
#define RFM23BP_WAKE_UP_TIMER_PERIOD_1                     0x14
#define RFM23BP_TEMPERATURE_VALUE_OFFSET                   0x13
#define RFM23BP_TEMPERATURE_SENSOR_CONTROL                 0x12
#define RFM23BP_ADC_VALUE                                  0x11
#define RFM23BP_ADC_SENSOR_AMPLIFIER_OFFSET                0x10
#define RFM23BP_ADC_CONFIGURATION                          0x0F
#define RFM23BP_IO_PORT_CONFIGURATION                      0x0E
#define RFM23BP_GPIO2_CONFIGURATION                        0x0D
#define RFM23BP_GPIO1_CONFIGURATION                        0x0C
#define RFM23BP_GPIO0_CONFIGURATION                        0x0B
#define RFM23BP_MICROCONTROLLER_OUTPUT_CLOCK               0x0A
#define RFM23BP_CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE        0x09
#define RFM23BP_OPERATING_AND_FUNCTION_CONTROL_2           0x08
#define RFM23BP_OPERATING_AND_FUNCTION_CONTROL_1           0x07
#define RFM23BP_INTERRUPT_ENABLE_2                         0x06
#define RFM23BP_INTERRUPT_ENABLE_1                         0x05
#define RFM23BP_INTERRUPT_STATUS_2                         0x04
#define RFM23BP_INTERRUPT_STATUS_1                         0x03
#define RFM23BP_DEVICE_STATUS                              0x02
#define RFM23BP_DEVICE_VERSION                             0x01
#define RFM23BP_DEVICE_TYPE                                0x00
#define RFM23BP_TIMEOUT                                    100
#define RH_SPI_WRITE_MASK                                  0x80
#define CALLSIGN_LENGTH                                    10
#define CALLSIGN                                           "CQ DE RS2S"
#define TEST_TRANSMITTER_INTERVAL                          1200

#endif
