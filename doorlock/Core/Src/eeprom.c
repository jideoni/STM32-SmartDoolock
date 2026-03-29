/*
 * eeprom.c
 *
 *  Created on: Mar 24, 2026
 *      Author: Jyde
 */

#include "eeprom.h"

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef Read_Current_PIN(uint8_t *pin) {
	return HAL_I2C_Mem_Read(&hi2c1,
	EEPROM_ADDR,
	EEPROM_PAGE,
	EEPROM_ADDR_MEM_SIZE, pin,
	EEPROM_RW_DATA_SIZE,
	EEPROM_RW_TIMEOUT);
}

void write_PIN_to_eeprom(uint8_t ble_rx[]) {
	HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, EEPROM_PAGE,
	EEPROM_ADDR_MEM_SIZE, ble_rx, EEPROM_RW_DATA_SIZE,
	EEPROM_RW_TIMEOUT);
	HAL_Delay(5);
}
