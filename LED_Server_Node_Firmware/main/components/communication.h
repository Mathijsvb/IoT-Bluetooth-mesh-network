/*
 * communication.h
 *
 *  Created on: 6 May 2022
 *      Author: Richard & Tom
 */

#ifndef MAIN_COMMUNICATION_H_
#define MAIN_COMMUNICATION_H_

#include "esp_err.h"
#include <math.h>

esp_err_t process_raw_temp_hum_values(uint8_t *data, size_t data_size, int16_t *temp_ptr, uint16_t *hum_ptr);
void print_sensor_values(int16_t *temp_ptr, uint16_t *hum_ptr);
esp_err_t print_status_register(uint8_t *data, size_t data_size);

#endif /* MAIN_COMMUNICATION_H_ */
