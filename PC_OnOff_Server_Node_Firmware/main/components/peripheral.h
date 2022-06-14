#ifndef _peripheral_H
#define _peripheral_H


#include <stdio.h>
#include <stdbool.h>

#include "esp_log.h"
#include "LED.h"

#include "esp_ble_mesh_common_api.h"

#define COLOUR_MASK 0b00000111
#define EFFECT_MASK 0b00011000
#define BUZZER_MASK 0b00100000

#define PHYS_MUTE_STATE_MASK 	0b00000001
#define USE_PHYS_MUTE_MASK 		0b00000010
#define ONLINE_MUTE_STATE_MASK 	0b00000100
#define USE_ONLINE_MUTE_MASK 	0b00001000

#define NOTHING_OP_CODE 	0b00
#define INDICATOR_OP_CODE 	0b01
#define CONTROL_OP_CODE 	0b10
#define RESERVED_OP_CODE 	0b11

#define ON 1
#define OFF 0

typedef enum {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	PURPLE,
	WHITE
} colour;

typedef enum {
	LED_OFF,
	STATIC,
	BLINKING,
	BREATHING
} effect;

typedef enum {
	BUTTONS_VIB_NODE,
	LED_NODE,
	RELAY_NODE
} node_type;

void node_init(node_type node);

void set_relay(bool relay_state);

void set_vib(bool buzzer_state);

uint8_t get_indicator_code(colour indicator_colour, effect indicator_effect, uint8_t buzzer_state);

uint8_t get_control_code(uint8_t use_online_mute, uint8_t online_mute_state, uint8_t use_phys_mute, uint8_t pys_mute_state);

void run_lights(effect effect_used, colour colour_used, uint8_t count);

void run_vib(uint8_t* buzzer_count, uint8_t count);

uint8_t check_if_code_type(uint8_t code, uint8_t old_code, uint8_t code_type);

void run_indicator_client(uint8_t* code, node_type node, uint8_t* current_count,  uint8_t* buzzer_count);

void run_control_client(uint8_t * control_code, node_type node);

void run_client_as_delay(uint8_t* code, node_type node, uint8_t* current_count,  uint8_t* buzzer_count, uint16_t delay_repititions);

void run_light_as_delay(effect effect_used, colour colour_used, uint8_t* current_count, uint16_t delay_repititions);

void display_code(uint8_t code);

void publish_msg(uint8_t code);

void peripheral_init(node_type node);

void set_AppKey();

#endif
