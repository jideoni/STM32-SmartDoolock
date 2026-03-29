/*
 * Rc522_RFID_Service.h
 *
 *  Created on: Mar 28, 2026
 *      Author: Jyde
 */

#ifndef INC_RC522_RFID_SERVICE_H_
#define INC_RC522_RFID_SERVICE_H_

#include <string.h>
#include "ble.h"
#include "RC522_RFID.h"

#define ID_SIZE 5
//extern uint8_t status;
//extern uint8_t str[MAX_LEN]; // Max_LEN = 16
//extern uint8_t sNum[5];

void RFID_Init(void);
void process_RFID_command(void);

#endif /* INC_RC522_RFID_SERVICE_H_ */
