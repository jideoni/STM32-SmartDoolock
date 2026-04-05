/*
 * eeprom.c
 *
 *  Created on: Mar 24, 2026
 *      Author: Jyde
 */

#include "eeprom.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t card1ID[5];
uint8_t card2ID[5];
uint8_t read_card_buf[MAX_LEN]; // Max_LEN = 5

HAL_StatusTypeDef read_current_PIN_from_eeporm(uint8_t *pin) {
	return HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, EEPROM_PIN_PAGE,
			EEPROM_PIN_MEM_ADDR_SIZE, pin, EEPROM_PIN_DATA_SIZE,
			EEPROM_RW_TIMEOUT);
}

void write_new_PIN_to_eeprom(uint8_t ble_rx[]) {
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, EEPROM_PIN_PAGE,
			EEPROM_PIN_MEM_ADDR_SIZE, ble_rx, EEPROM_PIN_DATA_SIZE,
			EEPROM_RW_TIMEOUT);
	HAL_Delay(5);
}

void read_card1ID_from_eeprom(void) {
	HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, (CARD1_PAGE << 6),
			EEPROM_CARD_ID_MEM_ADDR_SIZE, card1ID, AMT_OF_DATA_TO_SEND, 1000);
}
void read_card2ID_from_eeprom(void) {
	HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, (CARD2_PAGE << 6),
			EEPROM_CARD_ID_MEM_ADDR_SIZE, card2ID, AMT_OF_DATA_TO_SEND, 1000);
}
void save_card1ID_to_eeprom(void) {
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, EEPROM_CARD1_ID_PAGE,
			EEPROM_CARD_ID_MEM_ADDR_SIZE, read_card_buf, AMT_OF_DATA_TO_SEND,
			1000);
	HAL_Delay(5);
	save_new_card1_mode = IDLE1;
	display_temp = DISPLAY_TEMP;
	pinResetFeedbacks(2, "Save New Card", "Card 1", "Saved");
}
void save_card2ID_to_eeprom(void) {
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, EEPROM_CARD2_ID_PAGE,
			EEPROM_CARD_ID_MEM_ADDR_SIZE, read_card_buf, AMT_OF_DATA_TO_SEND,
			1000);
	HAL_Delay(5);
	save_new_card2_mode = IDLE2;
	display_temp = DISPLAY_TEMP;
	pinResetFeedbacks(2, "Save New Card", "Card 2", "Saved");
}
