/*
 * ble.h
 *
 *  Created on: Mar 24, 2026
 *      Author: Jyde
 */

#ifndef INC_BLE_H_
#define INC_BLE_H_

//#include <main.h>
#define STM32F0
//#define STM32F4

#if defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#endif

#include <stdbool.h>
#include "temp_sensor.h"
#include "eeprom.h"

#define RX_SIZE 4	//BLE RX size is 4 bytes
#define BLE_COMMAND_HEADER 4369		//0x1111
#define PIN_ATTEMPT_RETRY_WAIT_TIME 10000
#define PIN_CHANGE_RETRY_WAIT_TIME 30000
#define PIN_CHANGE_COMMAND 10794	//0x2A2A
#define REGISTER_CARD1 65521	//0xFFF1
#define REGISTER_CARD2 65522	//0xFFF2
#define SIZE_IN_BYTES 4
#define LOCK_DOOR_TIMEOUT 5000

extern bool new_pin_signal;
extern bool pin_reset_too_many_attempts;

extern uint8_t eeprom_data[SIZE_IN_BYTES];
extern uint8_t access_request_counter;
extern uint8_t pin_change_counter;
extern uint16_t ble_command;
extern uint16_t current_pin;	//current pin variable

extern TimeStamp pin_reset_attempt_again_timer;
extern TimeStamp attempt_again_timer_previous;
extern TimeStamp set_pin_timeflag;
extern TimeStamp set_pin_timeout;
extern TimeStamp attempt_again_timer;
extern TimeStamp timeOpen;
extern TimeStamp save_new_card_time_flag;

extern char print_buffer[50];
extern uint8_t rx_buf[RX_SIZE];	//holds 1 byte of data

//states
typedef enum {
	INACTIVE, ACTIVE
} SET_PIN_MODE_t;
extern SET_PIN_MODE_t set_pin_mode;

typedef enum {
	NO, YES
} ACC_REQ_t;
extern ACC_REQ_t access_request_too_many_attempts;

typedef enum {
	LOCKED, OPEN
} DOOR_STATUS_t;
extern DOOR_STATUS_t door_status;

typedef enum {
	IDLE1, PROCESSING1,
	REQUESTED1
} SAVE_NEW_CARD1_MODE_t;
extern SAVE_NEW_CARD1_MODE_t save_new_card1_mode;

typedef enum {
	IDLE2, PROCESSING2,
	REQUESTED2
} SAVE_NEW_CARD2_MODE_t;
extern SAVE_NEW_CARD2_MODE_t save_new_card2_mode;

void process_BLE_command(void);
void receive_BLE_command(void);
void unlock_door(void);
void clear_rx_buf(void);
void serial_print(char *msg);
void update_pin(void);
void access_request_attemps_check(void);
void pin_change_attempts_check(void);
void retrieve_ble_command(void);
void retrieve_current_pin(void);
void process_pin_change(void);
void enter_pin_change_mode(void);
void access_denied(void);
void access_request_wait_notif(void);
void lock_door(void);

#endif /* INC_BLE_H_ */
