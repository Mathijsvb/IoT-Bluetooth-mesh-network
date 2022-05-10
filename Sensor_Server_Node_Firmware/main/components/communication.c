/*
 * communication.c
 *
 *  Created on: 6 May 2022
 *      Author: Richard & Tom
 */

#include "communication.h"

esp_err_t process_raw_temp_hum_values(uint8_t *data, size_t data_size, int16_t *temp_ptr, uint16_t *hum_ptr)
/* Process the raw data from the sensor and put them into integers ready to transfer using the wireless protocol */
{
	if(data_size < 5 || data_size > 6)
		return ESP_ERR_INVALID_ARG;

	float temperature = (*data << 8) + *(data+1);
	temperature = -45 + 175.0*temperature/(65536.0 - 1);

	float humidity = (*(data+3) << 8) + *(data+4);
	humidity = 100.0*humidity/(65536.0 - 1);

	*temp_ptr = round(temperature * 100.0);
	*hum_ptr = round(humidity * 100.0);

	return ESP_OK;
}

void print_sensor_values(int16_t *temp_ptr, uint16_t *hum_ptr)
/* Print the sensor values, input should be pointers to 16 bit integers values for temperature and humidity */
{
	printf("Temperature: %.2f\n", *temp_ptr / 100.0);
	printf("Humidity:    %.2f\n", *hum_ptr / 100.0);
}

esp_err_t print_status_register(uint8_t *data, size_t data_size)
/* Prints the status register to the terminal in a convenient format */
{
	if(data_size < 2 || data_size > 3)
		return ESP_ERR_INVALID_ARG;

	_Bool alert_pending = *data & 0b10000000;
	_Bool heater_status = *data & 0b00100000;
	_Bool rh_tracking_alert = *data & 0b00001000;
	_Bool t_tracking_alert = *data & 0b00000100;
	_Bool system_reset_detected = *(data+1) & 0b00010000;
	_Bool command_status = *(data+1) & 0b00000010;
	_Bool write_data_checksum_status = *(data+1) & 0b00000001;

	printf("Alert pending status\t\t- ");
	if(alert_pending)
		printf("'1': at least one pending alert\n");
	else
		printf("'0': no pending alerts\n");

	printf("Heater status\t\t\t- ");
	if(heater_status)
		printf("'1': Heater ON\n");
	else
		printf("'0': Heater OFF\n");

	printf("RH tracking alert\t\t- ");
	if(rh_tracking_alert)
		printf("'1': alert\n");
	else
		printf("'0': no alert\n");

	printf("T tracking alert\t\t- ");
	if(t_tracking_alert)
		printf("'1': alert\n");
	else
		printf("'0': no alert\n");

	printf("System reset detected\t\t- ");
	if(system_reset_detected)
		printf("'1': reset detected (hard reset, soft reset command or supply fail)\n");
	else
		printf("'0': no reset detected since last 'clear status register' command\n");

	printf("Command status\t\t\t- ");
	if(command_status)
		printf("'1': last command not processed. It was either invalid, failed the integrated command checksum\n");
	else
		printf("'0': last command executed successfully\n");

	printf("Write data checksum status\t- ");
	if(write_data_checksum_status)
		printf("'1': checksum of last write transfer failed\n");
	else
		printf("'0': checksum of last write transfer was correct\n");

	return ESP_OK;
}
