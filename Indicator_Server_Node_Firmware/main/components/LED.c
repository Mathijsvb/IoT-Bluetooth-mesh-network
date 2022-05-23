#include "stdio.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"
#include "LED.h"

#define TAG "LED"
led_strip_t *led;

void LED_init(){
	//Configure the RMT channel and set the counter clock to 40MHz
	rmt_config_t config = RMT_DEFAULT_CONFIG_TX(2, 0);
	config.clk_div = 2;

	//Install the rmt driver
	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

	// install ws2812 driver
	led_strip_config_t led_config = LED_STRIP_DEFAULT_CONFIG(1, (led_strip_dev_t)config.channel);
	led = led_strip_new_rmt_ws2812(&led_config);
	if (!led) {
	 	ESP_LOGE(TAG, "install WS2812 driver failed");
	}

	// Clear LED strip (turn off all LEDs)
	ESP_ERROR_CHECK(led->set_pixel(led, 0, 0, 0, 0));
	ESP_ERROR_CHECK(led->refresh(led, 100));
}

void LED_setcolor(uint8_t red, uint8_t green, uint8_t blue){
	ESP_ERROR_CHECK(led->set_pixel(led, 0, red, green, blue));
	ESP_ERROR_CHECK(led->refresh(led, 100));
}
