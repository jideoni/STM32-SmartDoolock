/*
 * RC522_RFID-Service.c
 *
 *  Created on: Mar 28, 2026
 *      Author: Jyde
 */

#include "RC522_RFID_Service.h"

static uint8_t status = 0;
static uint8_t read_card_buf[MAX_LEN]; // Max_LEN = 5
static uint8_t sNum[ID_SIZE];
uint8_t card1ID[5];
uint8_t card2ID[5];

extern I2C_HandleTypeDef hi2c1;

void RFID_Init(void) {
	MFRC522_Init();
}

void process_RFID_command(void) {
	status = MFRC522_Request(PICC_REQIDL, read_card_buf);
	status = MFRC522_Anticoll(read_card_buf);
	memcpy(sNum, read_card_buf, ID_SIZE);

	if (confirm_read_card_buf_is_full(read_card_buf)) {
		if (save_new_card1_mode == PROCESSING1) {
			//read card2 page
			HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, (CARD2_PAGE << 6),
			EEPROM_CARD_ID_MEM_ADDR_SIZE, card2ID,
			AMT_OF_DATA_TO_SEND, 1000);
			if (memcmp(read_card_buf, card2ID, 5) == 0) {
				pinResetFeedbacks(2, "Save New Card", "Try Another", "Card");
				save_new_card_time_flag = time_now();
			} else {
				HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, EEPROM_CARD1_ID_PAGE,
				EEPROM_CARD_ID_MEM_ADDR_SIZE, read_card_buf,
				AMT_OF_DATA_TO_SEND, 1000);
				HAL_Delay(5);
				save_new_card1_mode = IDLE1;
				display_temp = DISPLAY_TEMP;
				pinResetFeedbacks(2, "Save New Card", "Card 1", "Saved");
				//currentMillis = time_now();
			}
		} else if (save_new_card2_mode == PROCESSING2) {
			//read card1 page
			HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, (CARD1_PAGE << 6),
			EEPROM_CARD_ID_MEM_ADDR_SIZE, card1ID,
			AMT_OF_DATA_TO_SEND, 1000);
			if (memcmp(read_card_buf, card1ID, 5) == 0) {
				pinResetFeedbacks(2, "Save New Card", "Try Another", "Card");
				save_new_card_time_flag = time_now();
			} else {
				HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, EEPROM_CARD2_ID_PAGE,
				EEPROM_CARD_ID_MEM_ADDR_SIZE, read_card_buf,
				AMT_OF_DATA_TO_SEND, 1000);
				HAL_Delay(5);
				save_new_card2_mode = IDLE2;
				display_temp = DISPLAY_TEMP;
				pinResetFeedbacks(2, "Save New Card", "Card 2", "Saved");
				//currentMillis = time_now();
			}
		} else {
			//read card1 page
			HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, (CARD1_PAGE << 6),
			EEPROM_CARD_ID_MEM_ADDR_SIZE, card1ID,
			AMT_OF_DATA_TO_SEND, 1000);

			//read card2 page
			HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, (CARD2_PAGE << 6),
			EEPROM_CARD_ID_MEM_ADDR_SIZE, card2ID,
			AMT_OF_DATA_TO_SEND, 1000);

			if ((memcmp(read_card_buf, card1ID, 5) == 0)
					|| (memcmp(read_card_buf, card2ID, 5) == 0)) {
				if (door_status == LOCKED) {
					unlock_door();
				} else {
					lock_door();
				}
				//currentMillis = time_now();
			} else {
				access_denied();
				clear_read_card_buf();
			}
			//currentMillis = time_now();
		}
		clear_read_card_buf();
		currentMillis = time_now();
	}
}

uint8_t confirm_read_card_buf_is_full(uint8_t *ID_data) {
	for (int i = 0; i < sizeof(read_card_buf); i++) {
		if (ID_data[i] == 0) {
			return 0;
		}
	}
	return 1;
}

void clear_read_card_buf(void) {
	for (int i = 0; i < sizeof(read_card_buf); i++) {
		read_card_buf[i] = 0;
	}
}
