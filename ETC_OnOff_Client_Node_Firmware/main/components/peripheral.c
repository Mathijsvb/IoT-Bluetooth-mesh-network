
/* ########################################################
 *
 * Author: Mathijs van Binnendijk
 * Purpose: This file includes functions taking care of the
 * indicator/ actuator node functionalities
 * Date: 07/06/2022 (dd/mm/yyyy)
 *
 * ########################################################
 */

#include "peripheral.h"

#define TAG "PERIPHERAL"

#define relay_pin 0
#define buzzer_pin 0
#define button_pins[2] = {0, 0}

/* max RGB value */
const uint8_t max_val = 255;

/* timing */
const uint8_t count_max = 100;
const uint8_t smallest_delay_time_ms = 20;

/* buzzer */
const uint8_t times_vib = 3;

/*
 * Function:  indicator_init
 * -------------------------
 * initialises the gpio pins used for the buzzer and relay
 */
void node_init(node_type node) {
	//ToDo: add init
}

void set_relay(bool pys_mute_state) {
  //ToDo: add pin functions
  if (pys_mute_state) {

  } else {

  }
}

void set_vib(bool vib_state) {
  //ToDo: add pin functions
  if (vib_state) {

  } else {

  }
}

/*
 * Function:  convert_code_to_RGB
 * ------------------------------
 *  takes a colour code and converts it to RGB values
 *
 *  indicator_colour: colour of the LED
 *  red, green, blue: RGB values from 0 to 255
 */
void convert_code_to_RGB(colour colour_used, uint8_t * red, uint8_t * green, uint8_t * blue) {
  switch (colour_used) {
  case RED:
    * red = max_val;
    * green = 0;
    * blue = 0;
    break;
  case ORANGE:
    * red = max_val;
    * green = max_val / 3;
    * blue = 0;
    break;
  case YELLOW:
    * red = max_val;
    * green = max_val;
    * blue = 0;
    break;
  case GREEN:
    * red = 0;
    * green = max_val;
    * blue = 0;
    break;
  case CYAN:
    * red = 0;
    * green = max_val;
    * blue = max_val;
    break;
  case BLUE:
    * red = 0;
    * green = 0;
    * blue = max_val;
    break;
  case PURPLE:
    * red = max_val;
    * green = 0;
    * blue = max_val;
    break;
  case WHITE:
    * red = max_val;
    * green = max_val;
    * blue = max_val;
    break;
  default:
    ESP_LOGW(TAG, "unsupported colour type");
    break;
  }
}

/*
 * Function:  get_indicator_code
 * -----------------------------
 *  generates an indicator code used to communicate indicator
 *  states over the network
 *
 *  indicator_colour: colour of the LED
 *  indicator_effect: effect used to display this colour
 *  vib_state: ON for turning on vibrating motor, OFF for leaving off
 *
 *  returns: indicator code containing led and buzzer information
 */
uint8_t get_indicator_code(colour indicator_colour, effect indicator_effect, uint8_t vib_state) {
  uint8_t code;

  code = (INDICATOR_OP_CODE << 6) | (vib_state << 5) | (indicator_effect << 3) | indicator_colour;

  return code;
}

/*
 * Function:  get_control_code
 * ---------------------------
 *  generates an control code used to communicate control
 *  states over the network. can control muting online and physical
 *
 *  use_online_mute: if 1: apply online_mute_state, if 0: don't
 *  online_mute_state: if 1: mute, if 0: unmute
 *  use_phys_mute: if 1: apply online_mute_state, if 0: don't
 *  pys_mute_state: if 1: mute, if 0: unmute
 *
 *  returns: control code containing relay information
 */
uint8_t get_control_code(uint8_t use_online_mute, uint8_t online_mute_state, uint8_t use_phys_mute, uint8_t pys_mute_state) {
  uint8_t code;

  code = (CONTROL_OP_CODE << 6)| (use_online_mute << 3) | (online_mute_state << 2) | (use_phys_mute << 1) | pys_mute_state;

  return code;
}

/*
 * Function:  run_lights
 * ---------------------
 *  Runs the lighting effects on the LED based on a counter
 *
 *  effect_used: LED effect displayed (LED_OFF, STATIC, BLINKING, BREATHING)
 *  indicator_colour: colour of the LED (RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE)
 *  count: counter used for the effect. Needs to be incremented on a regular interval outside of function
 */
void run_lights(effect effect_used, colour colour_used, uint8_t count) {

  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;

  float breathe_amount = 0;

  convert_code_to_RGB(colour_used, & red, & green, & blue);

  switch (effect_used) {
  case LED_OFF:
    LED_setcolor(0, 0, 0);
    break;
  case STATIC:
    LED_setcolor(red, green, blue);
    break;
  case BREATHING:
    if (count < (count_max / 2)) {
      breathe_amount = ((float) count / (float) count_max) * 2;
      //ESP_LOGI(TAG, "UP, count: %d; breathe_amount: %f", count, breathe_amount);
    } else {
      breathe_amount = (1 - ((float) count / (float) count_max)) * 2;
      //ESP_LOGI(TAG, "DOWN, count: %d; breathe_amount: %f", count, breathe_amount);
    }

    LED_setcolor(breathe_amount *red, breathe_amount *green, breathe_amount *blue);

    break;
  case BLINKING:
    if (count < (count_max / 2)) {
      LED_setcolor(red, green, blue);
    } else {
      LED_setcolor(0, 0, 0);
    }
    break;
  }
}

/*
 * Function:  run_vib
 * ------------------
 *  Runs the vibration motor based on the count and vib_cout
 *
 *  vib_count: times the vibration motor will still vibrate (counts down from times_vib)
 *  count:	counter used for vibrating pulse effect
 */
void run_vib(uint8_t * vib_count, uint8_t count) {

  //ESP_LOGI(TAG, "vib_count %d, count %d", *vib_count,  count);

  if ( * vib_count == 0) {

    return;
  }

  /* this makes sure the buzzer doesn't run continuously */
  if (count < (count_max / 2)) {
    set_vib(ON);
    //ESP_LOGI(TAG, "BUZZ ON, count %d", count);
  } else if (count == (count_max - 1)) { // subtract one of buzzer count end of count
    ( * vib_count) --;
    ESP_LOGI(TAG, "Buzz %d times", * vib_count);
    set_vib(OFF);
  } else {
    set_vib(OFF);
    //ESP_LOGI(TAG, "BUZZ OFF, count %d", count);
  }
}

/*
 * Function:  check_if_code_type
 * -----------------------------
 *  Can be used to check if a code is a control or indicator code
 *
 *  code: code that has to be checked
 *  old_code: indicator or control code the device uses to perform its role
 *  node: type of node
 *
 *  returns: the code if the code was meant for the node, else the old code
 */
uint8_t check_if_code_type(uint8_t code, uint8_t old_code, uint8_t node) {
  uint8_t msg_OP_CODE = (code >> 6);

  if (node == LED_NODE || node == BUTTONS_VIB_NODE) {
	  if (msg_OP_CODE == INDICATOR_OP_CODE) {
		display_code(code);
	    return code;
	  } else {
	    return old_code;
	    ESP_LOGI(TAG, "Code not meant for this node");

	  }
  } else if (node == RELAY_NODE) {
	  if (msg_OP_CODE == CONTROL_OP_CODE) {
		display_code(code);
	    return code;
	  } else {
		  ESP_LOGI(TAG, "Code not meant for this node");
	    return old_code;
	  }
  } else {
	  ESP_LOGE(TAG, "Non supported node type");
	  return 0;
  }

}

bool check_if_button_pressed(uint8_t button_number) {
	return true;
}

/*
 * Function:  run_indicator_client
 * -------------------------------
 *  This function will perform all tasks for an indicator node like the LED and vibrating motor
 *
 *  indicator_code: the code containing the information for the indicator operation
 *  node:	type of node
 *  current_count: used for the effects
 *  vib_count: used for the vibration motor
 */
void run_indicator_client(uint8_t * indicator_code, node_type node, uint8_t * current_count, uint8_t * vib_count) {

  uint8_t msg_OP_CODE = (*indicator_code >> 6);

  if (msg_OP_CODE != INDICATOR_OP_CODE) {
    ESP_LOGE(TAG, "Attempting to run indicator client with non indicator client code");
    return;
  }

  if (node != LED_NODE && node != BUTTONS_VIB_NODE) { // is this the node the message is meant for?
    ESP_LOGE(TAG, "Attempting to run indicator client on non indicator client node");
    return;
  }

  effect effect_used_from_msg = (*indicator_code & EFFECT_MASK) >> 3;
  colour colour_used_from_msg = (*indicator_code & COLOUR_MASK);

  run_lights(effect_used_from_msg, colour_used_from_msg, *current_count);

  if (node == BUTTONS_VIB_NODE) {
    uint8_t vib_state_from_msg = (*indicator_code & BUZZER_MASK) >> 5;

    if (vib_state_from_msg) {
      *indicator_code = (*indicator_code) & (~BUZZER_MASK); // set buzzer on bit 0 in code send
      *vib_count = times_vib; // vibration motor will vibrate times_vib amount
      *current_count = 0; // count restart at 0 so vibrating pulses are consistent
      ESP_LOGI(TAG, "Buzzing started, buzz %d times", * vib_count);
    }

    run_vib(vib_count, * current_count);
  }

  if ( * current_count >= count_max)
    *
    current_count = 0;
  else {
    ( * current_count) ++;
  }
}

/*
 * Function:  run_control_client
 * -----------------------------
 *  This function will perform all tasks for an control node
 *
 *  control_code: the code containing the information for the control operation
 *  node:	type of node
 */
void run_control_client(uint8_t * control_code, node_type node) {
  uint8_t msg_OP_CODE = ( * control_code >> 6);

  if (msg_OP_CODE != CONTROL_OP_CODE) {
    ESP_LOGE(TAG, "Attempting to run control client with non control client code");
    return;
  }
  if (node != RELAY_NODE) {
    ESP_LOGE(TAG, "Attempting to run control client on non control client node");
    return;
  }

  uint8_t phys_mute_state_from_msg = (*control_code & PHYS_MUTE_STATE_MASK);
  uint8_t use_phys_mute_from_msg = (*control_code & USE_PHYS_MUTE_MASK) >> 1;

  if (use_phys_mute_from_msg) {
    set_relay(phys_mute_state_from_msg);
  }

}

/*
 * Function:  run_indicator_client
 * -------------------------------
 *  This function will perform all tasks for an indicator or control node and is meant to be used as a delay function
 *
 *  code: the code containing the information for the indicator or control node operation
 *  node:	type of node
 *  current_count: used for the effects
 *  vib_count: used for the vibration motor
 *  delay_repititions: determines delay (delay_repititions times smallest_delay_time_ms is the delay in ms)
 */
void run_client_as_delay(uint8_t * code, node_type node, uint8_t * current_count, uint8_t * vib_count, uint16_t delay_repititions) {

  if (node == LED_NODE || node == BUTTONS_VIB_NODE) {
    for (int i = 0; i < delay_repititions; i++) {
      run_indicator_client(code, node, current_count, vib_count);
      vTaskDelay(pdMS_TO_TICKS(smallest_delay_time_ms));
    }
  } else if (node == RELAY_NODE) {
    for (int i = 0; i < delay_repititions; i++) {
      run_control_client(code, node);
      vTaskDelay(pdMS_TO_TICKS(smallest_delay_time_ms));
    }
  }
}

/*
 * Function:  run_light_as_delay
 * -----------------------------
 *  This function is meant for other nodes that do not use indicator codes but still want to display LED effects
 *
 *  effect_used: LED effect displayed (LED_OFF, STATIC, BLINKING, BREATHING)
 *  indicator_colour: colour of the LED (RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE)
 *  current_count: used for the effects
 *  delay_repititions: determines delay (delay_repititions times smallest_delay_time_ms is the delay in ms)
 */
void run_light_as_delay(effect effect_used, colour colour_used, uint8_t * current_count, uint16_t delay_repititions) {

  for (int i = 0; i < delay_repititions; i++) {
    run_lights(effect_used, colour_used, * current_count);

    if ( * current_count >= count_max)
      *current_count = 0;
    else {
      (*current_count) ++;
    }
    vTaskDelay(pdMS_TO_TICKS(smallest_delay_time_ms));
  }
}


/*
 * Function:  display_code
 * -----------------------
 * Debug function displaying all properties of a indicator or control code
 */
void display_code(uint8_t code) {
  if ((code >> 6) == INDICATOR_OP_CODE) {
    ESP_LOGI(TAG, "Indicator Code: %d%d Buzzer: %d Effect: %d%d Colour: %d%d%d", (code & 0b10000000) >> 7, (code & 0b01000000) >> 6, (code & 0b00100000) >> 5, (code & 0b00010000) >> 4, (code & 0b00001000) >> 3, (code & 0b00000100) >> 2, (code & 0b00000010) >> 1, (code & 0b00000001));
  } else if ((code >> 6) == CONTROL_OP_CODE) {
    ESP_LOGI(TAG, "Control Code: %d%d ND: %d%d%d%d Use relay: %d Relay state: %d", (code & 0b10000000) >> 7, (code & 0b01000000) >> 6, (code & 0b00100000) >> 5, (code & 0b00010000) >> 4, (code & 0b00001000) >> 3, (code & 0b00000100) >> 2, (code & 0b00000010) >> 1, (code & 0b00000001));
  } else {
    ESP_LOGI(TAG, "Undefined Code: %d%d%d%d%d%d%d%d", (code & 0b10000000) >> 7, (code & 0b01000000) >> 6, (code & 0b00100000) >> 5, (code & 0b00010000) >> 4, (code & 0b00001000) >> 3, (code & 0b00000100) >> 2, (code & 0b00000010) >> 1, (code & 0b00000001));
  }

}
