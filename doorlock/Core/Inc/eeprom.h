/*
 * eeprom.h
 *
 *  Created on: Mar 24, 2026
 *      Author: Jyde
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "ble.h"

#define EEPROM_I2C &hi2c1
#define EEPROM_ADDR 0xA0
#define EEPROM_RW_TIMEOUT 100

#define EEPROM_PIN_PAGE 			(1 << 6)	//page 6 of 256
#define EEPROM_PIN_MEM_ADDR_SIZE 	2			//2 bytes
#define EEPROM_PIN_DATA_SIZE 		4			//4 bytes

#define EEPROM_CARD1_ID_PAGE 			(10 << 6)	//page 10 of 256
#define EEPROM_CARD2_ID_PAGE 			(11 << 6)	//page 11 of 256
#define EEPROM_CARD_ID_MEM_ADDR_SIZE 	2			//2 bytes
#define EEPROM_CARD_DATA_SIZE 			5			//5 bytes

void write_new_PIN_to_eeprom(uint8_t ble_rx[]);
HAL_StatusTypeDef read_current_PIN_from_eeporm(uint8_t *pin);

void write_card_ID_to_eeprom(uint8_t card_ID[]);
HAL_StatusTypeDef read_card_ID_from_eeprom(uint8_t *card);

#endif /* INC_EEPROM_H_ */
