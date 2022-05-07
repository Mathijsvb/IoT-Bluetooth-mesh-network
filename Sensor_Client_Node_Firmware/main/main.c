#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_sensor_model_api.h"

#include "ble_mesh_example_init.h"
#include "components/BLE_Mesh.h"

#define TAG "MAIN"



void app_main(void)
{
	esp_err_t err = ESP_OK;
	ESP_LOGI(TAG, "Initializing...");

	err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
	    ESP_ERROR_CHECK(nvs_flash_erase());
	    err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);

	err = bluetooth_init();
	if (err != ESP_OK) {
	    ESP_LOGE(TAG, "esp32_bluetooth_init failed (err %d)", err);
	    return;
	}

    err = ble_mesh_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Bluetooth mesh init failed (err %d)", err);
    }
}
