/*
 * temp_sensor.c
 *
 *  Created on: Feb 21, 2026
 *      Author: Jyde
 */

#include "temp_sensor.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

DISP_TMP_t display_temp = DISPLAY_TEMP;

TimeStamp currentMillis = 0;
TimeStamp previousMillis = 0;

HAL_StatusTypeDef ret;
int16_t val;
uint8_t temp_buffer[2];		//12 size buffer of type unsigned 8 bit integer
float temp_c;

//float TMP102_Read_Temp(void) {
//	temp_buffer[0] = REG_TEMP;
//	ret = HAL_I2C_Master_Transmit(&hi2c1, TMP102_ADDR, temp_buffer, 1, 10);
//	if (ret != HAL_OK) {
//		strcpy((char*) temp_buffer, "Error Tx\r\n");
//	} else {
//		// Read 2 bytes from the temperature register
//		ret = HAL_I2C_Master_Receive(&hi2c1, TMP102_ADDR, temp_buffer, 1, 10);
//		if (ret != HAL_OK) {
//			strcpy((char*) temp_buffer, "Error Rx\r\n");
//		} else {
//			// Combine the bytes
//			val = ((int16_t) temp_buffer[0] << 4) | (temp_buffer[1] >> 4);
//			//Convert to 2's complement, since temperature can be negative
//			if (val > 0x7FF) {
//				val |= 0xF000;
//			}
//			//Convert to float temperature value (celsius)
//			temp_c = val * 0.0625;
//			// Convert temperature to decimal format
//			temp_c *= 100;
//		}
//	}
//	return temp_c;
//}

float TMP102_Read_Temp(void) {
	//Initialize for next read
	//temp_sensor_Init();

	// Combine the bytes
	val = ((int16_t) temp_buffer[0] << 4) | (temp_buffer[1] >> 4);
	//Convert to 2's complement, since temperature can be negative
	if (val > 0x7FF) {
		val |= 0xF000;
	}
	//Convert to float temperature value (celsius)
	temp_c = val * 0.0625;
	// Convert temperature to decimal format
	temp_c *= 100;
	return temp_c;
}

void temp_sensor_Init(void) {
	HAL_I2C_Mem_Read_DMA(&hi2c1, TMP102_ADDR, REG_TEMP, // temperature register
			I2C_MEMADD_SIZE_8BIT, temp_buffer, AMT_OF_DATA_TO_READ);
}

uint32_t time_now(void) {
	return HAL_GetTick();
}

void temp_Sensor_service(void) {
	previousMillis = time_now();
	if (previousMillis - currentMillis >= TEMPERATURE_DISPLAY_TIME) {
		if (display_temp == DISPLAY_TEMP) {
			displayTemperature("Temperature");
		}
		// update previous time
		previousMillis = currentMillis;
	}
}
