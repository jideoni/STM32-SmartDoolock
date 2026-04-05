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
#include "eeprom.h"

void RFID_Init(void);
void process_RFID_command(void);
uint8_t confirm_read_card_buf_is_full(uint8_t *ID_data);
void clear_read_card_buf(void);

#endif /* INC_RC522_RFID_SERVICE_H_ */
