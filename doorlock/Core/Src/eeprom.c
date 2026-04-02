/*
 * eeprom.c
 *
 *  Created on: Mar 24, 2026
 *      Author: Jyde
 */

#include "eeprom.h"

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef read_current_PIN_from_eeporm(uint8_t *pin) {
	return HAL_I2C_Mem_Read(EEPROM_I2C,
	EEPROM_ADDR,
	EEPROM_PIN_PAGE, EEPROM_PIN_MEM_ADDR_SIZE, pin,
	EEPROM_PIN_DATA_SIZE,
	EEPROM_RW_TIMEOUT);
}

void write_new_PIN_to_eeprom(uint8_t ble_rx[]) {
	HAL_I2C_Mem_Write(EEPROM_I2C,
	EEPROM_ADDR,
	EEPROM_PIN_PAGE,
	EEPROM_PIN_MEM_ADDR_SIZE, ble_rx,
	EEPROM_PIN_DATA_SIZE,
	EEPROM_RW_TIMEOUT);
	HAL_Delay(5);
}

HAL_StatusTypeDef read_card_ID_from_eeprom(uint8_t *card) {
	return HAL_I2C_Mem_Read(EEPROM_I2C,
	EEPROM_ADDR,
	EEPROM_CARD1_ID_PAGE, EEPROM_CARD_ID_MEM_ADDR_SIZE, card,
	EEPROM_CARD_DATA_SIZE,
	EEPROM_RW_TIMEOUT);
}
void write_card_ID_to_eeprom(uint8_t card_ID[]) {
	HAL_I2C_Mem_Write(EEPROM_I2C,
	EEPROM_ADDR, EEPROM_CARD2_ID_PAGE, EEPROM_CARD_ID_MEM_ADDR_SIZE, card_ID,
	EEPROM_CARD_DATA_SIZE,
	EEPROM_RW_TIMEOUT);
	HAL_Delay(5);
}
