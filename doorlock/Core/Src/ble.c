/*
 * ble.c
 *
 *  Created on: Mar 24, 2026
 *      Author: Jyde
 */

#include <stdio.h>
#include <string.h>
#include "ble.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

bool new_pin_signal = 0;
bool pin_reset_too_many_attempts = 0;

uint8_t eeprom_data[SIZE_IN_BYTES];
uint8_t access_request_counter = 0;
uint8_t pin_change_counter = 0;
uint16_t ble_command = 0;
uint16_t current_pin = 0;	//current pin variable

TimeStamp pin_reset_attempt_again_timer = 0;
TimeStamp attempt_again_timer_previous = 0;
TimeStamp set_pin_timeflag = 0;
TimeStamp set_pin_timeout = 0;
TimeStamp attempt_again_timer = 0;
TimeStamp timeOpen = 0;
TimeStamp timeCheck = 0;
TimeStamp save_new_card_time_flag = 0;

char print_buffer[50];
uint8_t rx_buf[RX_SIZE];	//holds 1 byte of data

SET_PIN_MODE_t set_pin_mode = INACTIVE;
DOOR_STATUS_t door_status = LOCKED;
ACC_REQ_t access_request_too_many_attempts = NO;
SAVE_NEW_CARD1_MODE_t save_new_card1_mode = IDLE1;
SAVE_NEW_CARD2_MODE_t save_new_card2_mode = IDLE2;

void process_BLE_command(void) {
	uint16_t header = (rx_buf[0] << 8) | rx_buf[1];
	if (header == BLE_COMMAND_HEADER) {	//confirm if ble command header is valid
		if (access_request_too_many_attempts == NO) {
			retrieve_ble_command();
			retrieve_current_pin();

			// set new pin
			if ((new_pin_signal == 1) && (set_pin_mode == ACTIVE)) {
				update_pin();
			} else if (set_pin_mode == ACTIVE) {
				process_pin_change();
			} else if ((ble_command == current_pin)
					&& (save_new_card1_mode == REQUESTED1)) {
				pinResetFeedbacks(2, "Save New Card", "Scan", "New Card");
				display_temp = DO_NOT_DISPLAY_TEMP;
				save_new_card1_mode = PROCESSING1;
				save_new_card_time_flag = time_now();
			} else if ((ble_command == current_pin)
					&& (save_new_card2_mode == REQUESTED2)) {
				pinResetFeedbacks(2, "Save New Card", "Scan", "New Card");
				display_temp = DO_NOT_DISPLAY_TEMP;
				save_new_card2_mode = PROCESSING2;
				save_new_card_time_flag = time_now();
			}
			//confirm if entered PIN is correct, grant access
			else if (ble_command == current_pin) {
				unlock_door();
			}
			//enter pin change mode
			else if ((ble_command == PIN_CHANGE_COMMAND)
					&& (door_status == LOCKED)) {
				enter_pin_change_mode();
			} else if ((ble_command == REGISTER_CARD1)
					&& (door_status == LOCKED)) {
				pinResetFeedbacks(2, "Save New Card1", "Enter", "Current PIN");
				save_new_card1_mode = REQUESTED1;
			} else if ((ble_command == REGISTER_CARD2)
					&& (door_status == LOCKED)) {
				pinResetFeedbacks(2, "Save New Card2", "Enter", "Current PIN");
				save_new_card2_mode = REQUESTED2;
			}
			//confirm if entered PIN is correct, deny access
			else if ((ble_command != current_pin) && (door_status == LOCKED)) {
				access_denied();
			}
			clear_rx_buf();
		}
		//notify user to wait
		else {
			access_request_wait_notif();
		}
		access_request_attemps_check();
		pin_change_attempts_check();
	}
	currentMillis = time_now();
}

void access_request_wait_notif(void) {
	pinResetFeedbacks(2, "Too Many Attempts", "Try Again", "Later");
	attempt_again_timer = time_now();
}
void access_denied(void) {
	serial_print("Access Denied");
	pinResetFeedbacks(2, "Try Again", "Access", "Denied");
	access_request_counter++;
}
void enter_pin_change_mode(void) {
	if (pin_reset_too_many_attempts == 0) {
		set_pin_mode = ACTIVE;
		display_temp = DO_NOT_DISPLAY_TEMP;
		set_pin_timeflag = time_now();
		serial_print("Enter Current Pin...");
		pinResetFeedbacks(2, "Pin Change Menu", "Enter", "Current Pin");
	} else {
		pinResetFeedbacks(2, "Change On Hold", "Try Again", "Later");
		pin_reset_attempt_again_timer = time_now();
	}
}
void process_pin_change(void) {
	if (ble_command == current_pin) {	//if you get the current pin right
		serial_print("Enter New Pin");
		pinResetFeedbacks(2, "Pin Change Menu", "Enter", "New Pin");
		set_pin_timeflag = time_now();	//restart timeout
		new_pin_signal = 1;
	} else {
		//Incorrect current pin entered
		serial_print("Incorrect, try again.");
		pinResetFeedbacks(2, "Pin Change Menu", "Incorrect", "Try Again");
		set_pin_timeflag = time_now();	//restart timeout
		currentMillis = time_now();
		pin_change_counter++;
	}
}
void retrieve_ble_command(void) {
	ble_command = (rx_buf[2] << 8) | rx_buf[3];
	//print received command
	sprintf(print_buffer, "Received: %d\r\n", ble_command);
	HAL_UART_Transmit(&huart2, (uint8_t*) print_buffer, strlen(print_buffer),
			10);
}

void retrieve_current_pin(void) {
	//retrieve current PIN
	if (read_current_PIN_from_eeporm(eeprom_data) == HAL_OK) {
		current_pin = (eeprom_data[0] << 8) | eeprom_data[1];
	}
	//print current PIN
	sprintf(print_buffer, "PIN: %d\r\n", current_pin);
	HAL_UART_Transmit(&huart2, (uint8_t*) print_buffer, strlen(print_buffer),
			20);

}
void access_request_attemps_check(void) {
	if (access_request_counter == 5) {
		access_request_too_many_attempts = YES;
		pinResetFeedbacks(2, "Too Many Attempts", "Try Again", "Later");
		attempt_again_timer = time_now();
	}
}
void pin_change_attempts_check(void) {
	if (pin_change_counter >= 5) {
		pin_reset_too_many_attempts = 1;
		pinResetFeedbacks(2, "Change On Hold", "Try Again", "Later");
		pin_reset_attempt_again_timer = time_now();

	}
}

void receive_BLE_command(void) {
	HAL_UART_Receive_IT(&huart1, (uint8_t*) rx_buf, RX_SIZE);
}

void lock_door(void) {
	timeCheck = time_now();
	if (timeCheck - timeOpen >= LOCK_DOOR_TIMEOUT) {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
		door_status = LOCKED;	//door locked
		pinResetFeedbacks(2, "Goodbye", "Door", "Locked");
	}
}

void unlock_door() {
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	timeOpen = time_now();
	serial_print("Access Granted");
	pinResetFeedbacks(2, "Welcome Home!", "Access", "Granted");
	attempt_again_timer = time_now();
	access_request_counter = 0;		//reset counter
	pin_change_counter = 0;
	display_temp = DISPLAY_TEMP;
	door_status = OPEN;	//door unlocked
}

void clear_rx_buf(void) {
	for (int i = 0; i < RX_SIZE; i++) {
		rx_buf[i] = 0;
	}
}

void serial_print(char *msg) {
	static char b[50];
	sprintf(b, "%s\r\n", msg);
	HAL_UART_Transmit(&huart2, (uint8_t*) b, strlen(b), 10);
}

void update_pin(void) {
	uint8_t new_pin_buf[] = { rx_buf[2], rx_buf[3] };
	if ((ble_command == PIN_CHANGE_COMMAND)
			&& (pin_reset_too_many_attempts == 0)) {
		serial_print("Try a different PIN");
		pinResetFeedbacks(1, "Pin Change Menu", "Try Again", "");
	} else {
		write_new_PIN_to_eeprom(new_pin_buf);
		pinResetFeedbacks(2, "Pin Change Menu", "Pin Change", "Successful");
		sprintf(print_buffer, "New pin is %d\r\n", ble_command);
		HAL_UART_Transmit(&huart2, (uint8_t*) print_buffer,
				strlen(print_buffer), 20);
//clear flags
		set_pin_mode = INACTIVE;
		new_pin_signal = 0;
		display_temp = DISPLAY_TEMP;
//clear rx buffer
		clear_rx_buf();
	}
}
