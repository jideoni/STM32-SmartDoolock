/*
 * eeprom.h
 *
 *  Created on: Mar 24, 2026
 *      Author: Jyde
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "ble.h"

#define EEPROM_PAGE (1 << 6)
#define EEPROM_ADDR_MEM_SIZE 2
#define EEPROM_RW_DATA_SIZE 4		//4 bytes
#define EEPROM_RW_TIMEOUT 100
#define EEPROM_ADDR 0xA0

void update_pin(void);
void write_PIN_to_eeprom(uint8_t ble_rx[]);
HAL_StatusTypeDef Read_Current_PIN(uint8_t *pin);

#endif /* INC_EEPROM_H_ */
