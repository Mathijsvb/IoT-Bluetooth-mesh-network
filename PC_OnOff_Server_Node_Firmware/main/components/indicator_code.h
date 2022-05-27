#ifndef _indicator_code_H
#define _indicator_code_H

#define COLOUR_MASK 0b00000111
#define EFFECT_MASK 0b00011000
#define RELAY_MASK 0b01000000
#define BUZZER_MASK 0b10000000

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

void indicator_init();

void set_relay(bool relay_state);

void set_buzzer(bool buzzer_state);

void convert_code_to_RGB(uint8_t code, uint8_t* red, uint8_t* green, uint8_t* blue);

uint8_t get_indicator_code(colour indicator_colour, effect indicator_effect, uint8_t buzzer_state, uint8_t relay_state);

void run_indicator(uint8_t code, uint8_t use_buzzer, uint8_t use_relay, uint8_t* current_count);


#endif
