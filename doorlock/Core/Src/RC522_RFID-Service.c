/*
 * RC522_RFID-Service.c
 *
 *  Created on: Mar 28, 2026
 *      Author: Jyde
 */

#include "RC522_RFID_Service.h"

static uint8_t status = 0;
static uint8_t sNum[ID_SIZE];
extern uint8_t read_card_buf[MAX_LEN];

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
			//read card2ID from eeprom
			read_card2ID_from_eeprom();
			if (memcmp(read_card_buf, card2ID, 5) == 0) {
				pinResetFeedbacks(2, "Save New Card", "Try Another", "Card");
				save_new_card_time_flag = time_now();
			} else {
				//Save new card1 to eeprom
				save_card1ID_to_eeprom();
			}
		} else if (save_new_card2_mode == PROCESSING2) {
			//read card1ID from eeprom
			read_card1ID_from_eeprom();
			if (memcmp(read_card_buf, card1ID, 5) == 0) {
				pinResetFeedbacks(2, "Save New Card", "Try Another", "Card");
				save_new_card_time_flag = time_now();
			} else {
				//Save new card2 to eeprom
				save_card2ID_to_eeprom();
				//currentMillis = time_now();
			}
		} else {
			//read card1ID from eeprom
			read_card1ID_from_eeprom();
			//read card2ID from eeprom
			read_card2ID_from_eeprom();

			if ((memcmp(read_card_buf, card1ID, 5) == 0)
					|| (memcmp(read_card_buf, card2ID, 5) == 0)) {
				if (door_status == LOCKED) {
					unlock_door();
				} else {
					lock_door();
				}
			} else {
				access_denied();
				clear_read_card_buf();
			}
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
