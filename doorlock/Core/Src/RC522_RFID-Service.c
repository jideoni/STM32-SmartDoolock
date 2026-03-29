/*
 * RC522_RFID-Service.c
 *
 *  Created on: Mar 28, 2026
 *      Author: Jyde
 */

#include "RC522_RFID_Service.h"

static uint8_t status = 0;
static uint8_t str[MAX_LEN]; // Max_LEN = 16
static uint8_t sNum[ID_SIZE];

void RFID_Init(void) {
	MFRC522_Init();
}

void process_RFID_command(void) {
	status = MFRC522_Request(PICC_REQIDL, str);
	status = MFRC522_Anticoll(str);
	memcpy(sNum, str, ID_SIZE);
	//tag
	if ((str[0] == 213) && (str[1] == 230) && (str[2] == 43) && (str[3] == 7)
			&& (str[4] == 31)) {
		if (door_status == LOCKED) {
			unlock_door();
		}else{
			lock_door();
		}
		currentMillis = time_now();
	} else {
		//do nothing
	}
	/*else if ((str[0] == 155) && (str[1] == 153) && (str[2] == 15)
	 && (str[3] == 7) && (str[4] == 10)) {
	 //			lock door
	 //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	 }*/

}
