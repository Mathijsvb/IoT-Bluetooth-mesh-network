/*
 * commands.h
 *
 *  Created on: 3 May 2022
 *      Author: Richard & Tom
 */

#ifndef MAIN_COMMANDS_H_
#define MAIN_COMMANDS_H_

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "communication.h"

#define I2C_MASTER_SCL_IO			18     				       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO			19      				   /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM				0          				   /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ			100000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE	0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE	0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS		1000

#define SHT35_SENSOR_ADDR			0x45

#define POLYNOMIAL					0x31

// Measurement repeatability
typedef enum{
	HIGH_REPEATABILITY,
	MEDIUM_REPEATABILITY,
	LOW_REPEATABILITY,
}etRepeatability;

// Measurement frequency
typedef enum{
	FREQUENCY_HZ5,	//  0.5 measurements per seconds
	FREQUENCY_1HZ,	//  1.0 measurements per seconds
	FREQUENCY_2HZ,	//  2.0 measurements per seconds
	FREQUENCY_4HZ,	//  4.0 measurements per seconds
	FREQUENCY_10HZ,	// 10.0 measurements per seconds
}etFrequency;

esp_err_t i2c_master_init(void);
esp_err_t SHT35_single_measurement(int16_t *temp_ptr, uint16_t *hum_ptr);
static int8_t SHT35_calculate_crc(uint8_t data[], uint8_t number_of_bytes);
static esp_err_t SHT35_check_crc(uint8_t data[], uint8_t number_of_bytes, uint8_t checksum);
esp_err_t SHT35_read_out_status_register(uint8_t *data, size_t read_size);
esp_err_t SHT35_read_and_print_status_register(void);
esp_err_t SHT35_single_shot_data_acquisition(uint8_t *data, size_t read_size, _Bool clock_stretching, etRepeatability repeatability);
esp_err_t SHT35_read_measurements_periodic_mode(uint8_t *data, size_t read_size);
esp_err_t SHT35_heater(_Bool heater_enabled);
esp_err_t SHT35_periodic_data_acquisition(etFrequency frequency, etRepeatability repeatability);
esp_err_t SHT35_soft_reset(void);
esp_err_t SHT35_break_command(void);
esp_err_t SHT35_ART_command(void);
esp_err_t SHT35_clear_status_register(void);

#endif /* MAIN_COMMANDS_H_ */
