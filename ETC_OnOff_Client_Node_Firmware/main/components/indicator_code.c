#include <stdio.h>
#include <stdbool.h>

#include "esp_log.h"
#include "indicator_code.h"
#include "LED.h"

#define TAG "indicator_code"

const uint8_t max_val = 255;
const uint8_t count_max = 100;

#define relay_pin 0
#define buzzer_pin 0


void indicator_init() {

}

void set_relay(bool relay_state) {
	if(relay_state) {
		ESP_LOGI(TAG, "Turning on relay");
	} else {
		ESP_LOGI(TAG, "Turning off relay");
	}
}

void set_buzzer(bool buzzer_state) {
	if(buzzer_state) {
		ESP_LOGI(TAG, "Turning on buzzer");
	} else {
		ESP_LOGI(TAG, "Turning off buzzer");
	}
}


uint8_t get_indicator_code(colour indicator_colour, effect indicator_effect, uint8_t buzzer_state, uint8_t relay_state) {
	uint8_t code;
	code = (buzzer_state << 7) | (relay_state << 6) | (indicator_effect << 3) | indicator_colour;
	//ESP_LOGI(TAG, "Generated colour code 0x%x", code);
	return code;
}

void convert_code_to_RGB(uint8_t code, uint8_t* red, uint8_t* green, uint8_t* blue) {
	colour colour_code = code & COLOUR_MASK;
	switch (colour_code) {
	case RED:
		*red = max_val;
		*green = 0;
		*blue = 0;
		break;
	case ORANGE:
		*red = max_val;
		*green = max_val/3;
		*blue = 0;
		break;
	case YELLOW:
		*red = max_val;
		*green = max_val;
		*blue = 0;
		break;
	case GREEN:
		*red = 0;
		*green = max_val;
		*blue = 0;
		break;
	case CYAN:
		*red = 0;
		*green = max_val;
		*blue = max_val;
		break;
	case BLUE:
		*red = 0;
		*green = 0;
		*blue = max_val;
		break;
	case PURPLE:
		*red = max_val;
		*green = 0;
		*blue = max_val;
		break;
	case WHITE:
		*red = max_val;
		*green = max_val;
		*blue = max_val;
		break;
	default:
		ESP_LOGW(TAG, "unsupported colour type");
	break;
	}
}

void run_indicator(uint8_t code, uint8_t use_buzzer, uint8_t use_relay, uint8_t* current_count) {

	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t count = *current_count;

	effect effect_used = (code & EFFECT_MASK) >> 3;
	uint8_t relay = (code & RELAY_MASK) >> 6;
	uint8_t buzzer = (code & BUZZER_MASK) >> 7;

    float breathe_amount = 0;

    convert_code_to_RGB(code, &red, &green, &blue);

	switch (effect_used) {
	        	case LED_OFF:
	        		LED_setcolor(0, 0, 0);
	        	break;
	        	case STATIC:
	        		LED_setcolor(red, green, blue);
	        	break;
	        	case BREATHING:
	        		if(count < (count_max/2)) {
	        			breathe_amount = ((float) count /(float) count_max) * 2;
	        			//ESP_LOGI(TAG, "UP, count: %d; breathe_amount: %f", count, breathe_amount);
	        		}
	        		else {
	        			breathe_amount = (1 - ((float) count /(float) count_max)) * 2;
	        			//ESP_LOGI(TAG, "DOWN, count: %d; breathe_amount: %f", count, breathe_amount);
	        		}

	        		LED_setcolor(breathe_amount * red, breathe_amount * green, breathe_amount * blue);

	        	break;
	        	case BLINKING:
	        		if(count < (count_max/2)) {
	        			LED_setcolor(red, green, blue);
	        		}
	        		else {
	        			LED_setcolor(0, 0, 0);
	        		}
	        	break;
	}

	if (use_buzzer > 0 && use_relay > 0) {
		ESP_LOGW(TAG, "Can't use buzzer and relay on the same device");
		return;
	}

	if (use_buzzer > 0) {
		if(count < (count_max/2) && buzzer > 0) {
			set_buzzer(ON);
		}
		else {
			set_buzzer(OFF);
		}
	}

	if (use_relay > 0) {
		if(relay > 0) {
		set_relay(ON);
		} else {
		set_relay(OFF);
		}
	}

	 if (*current_count >= count_max)
		 *current_count = 0;
	 else {
		 (*current_count)++;
	 }
}
