/*
 * temp_sensor.h
 *
 *  Created on: Feb 21, 2026
 *      Author: Jyde
 */

#ifndef INC_TEMP_SENSOR_H_
#define INC_TEMP_SENSOR_H_

#include <main.h>
#include <string.h>
#include <stdio.h>
#include "ssd1306_service.h"

#define TEMPERATURE_DISPLAY_TIME 3000
#define AMT_OF_DATA_TO_READ 2

typedef uint32_t TimeStamp;
typedef enum {
	DO_NOT_DISPLAY_TEMP, DISPLAY_TEMP
} DISP_TMP_t;
extern DISP_TMP_t display_temp;

//extern uint8_t temp_buffer[2];		//12 size buffer of type unsigned 8 bit integer
static const uint8_t TMP102_ADDR = 0x48 << 1; //Use 8-bit address
static const uint8_t REG_TEMP = 0x00;

extern TimeStamp currentMillis;
extern TimeStamp previousMillis;

float TMP102_Read_Temp(void);
void temp_Sensor_service(void);
uint32_t time_now(void);
void temp_sensor_Init(void);

#endif /* INC_TEMP_SENSOR_H_ */
