#include <ssd1306_service.h>
#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <temp_sensor.h>

char b[5];
float t;

extern TIM_HandleTypeDef htim16;

static uint16_t timer_val = 0;

void ssd1306_RectangleFill() {
	ssd1306_FillRectangle(31, 1, 65, 35, White);
	//ssd1306_UpdateScreen();
}

void ssd1306_Circle() {
	uint32_t delta;
	for (delta = 0; delta < 5; delta++) {
		ssd1306_FillCircle(23 * delta + 15, 40, 10, White);
	}
	//ssd1306_UpdateScreen();
	return;
}

void displayTemperatureAtInit(char *heading) {
	t = TMP102_Read_Temp();
	snprintf((char*) b, sizeof(b), "%2d C", ((unsigned int) t / 100));

	ssd1306_Fill(Black);
	ssd1306_FillRectangle(0, 0, 128, 15, White);
	ssd1306_SetCursor((128 - (7 * strlen(heading))) / 2, 3);
	ssd1306_WriteString(heading, Font_7x10, Black);

	ssd1306_SetCursor((128 - (16 * 4)) / 2, 27);

	ssd1306_WriteString(b, Font_16x26, White);
	ssd1306_DrawCircle(71, 27, 2, White);
	ssd1306_UpdateScreen();
}

void displayTemperature(char *heading) {
	if (__HAL_TIM_GET_COUNTER(&htim16) - timer_val >= 3000) {
		t = TMP102_Read_Temp();
		snprintf((char*) b, sizeof(b), "%2d C", ((unsigned int) t / 100));

		ssd1306_Fill(Black);
		ssd1306_FillRectangle(0, 0, 128, 15, White);
		ssd1306_SetCursor((128 - (7 * strlen(heading))) / 2, 3);
		ssd1306_WriteString(heading, Font_7x10, Black);

		ssd1306_SetCursor((128 - (16 * 4)) / 2, 27);

		ssd1306_WriteString(b, Font_16x26, White);
		ssd1306_DrawCircle(71, 27, 2, White);
		//ssd1306_UpdateScreen();

		timer_val = __HAL_TIM_GET_COUNTER(&htim16);
	}
}

void pinResetFeedbacks(int line_count, char *heading, char *line1, char *line2) {
	ssd1306_Fill(Black);
	if (line_count == 1) {
		uint8_t x = (128 - (strlen(line1) * 11)) / 2;
		uint8_t y = ((48 - 18) / 2) + 15;

		uint8_t x2 = (128 - (strlen(line2) * 11)) / 2;
		uint8_t y2 = 20 + 18 + 2;

		ssd1306_FillRectangle(0, 0, 128, 15, White);
		ssd1306_SetCursor((128 - (7 * strlen(heading))) / 2, 3);
		ssd1306_WriteString(heading, Font_7x10, Black);

		ssd1306_SetCursor(x, y);
		ssd1306_WriteString(line1, Font_11x18, White);

		ssd1306_SetCursor(x2, y2);
		ssd1306_WriteString(line2, Font_11x18, White);

	} else if (line_count == 2) {
		uint8_t x = (128 - (strlen(line1) * 11)) / 2;
		uint8_t y = 20;

		uint8_t x2 = (128 - (strlen(line2) * 11)) / 2;
		uint8_t y2 = 20 + 18 + 2;

		ssd1306_FillRectangle(0, 0, 128, 15, White);
		ssd1306_SetCursor((128 - (7 * strlen(heading))) / 2, 3);
		ssd1306_WriteString(heading, Font_7x10, Black);

		ssd1306_SetCursor(x, y);
		ssd1306_WriteString(line1, Font_11x18, White);

		ssd1306_SetCursor(x2, y2);
		ssd1306_WriteString(line2, Font_11x18, White);
	}

	//ssd1306_UpdateScreen();
}

void pinResetFeedbacksNoHeading(char *line1, char *line2) {
	uint8_t x = (128 - (strlen(line1) * 11)) / 2;
	uint8_t y = 20;

	uint8_t x2 = (128 - (strlen(line2) * 11)) / 2;
	uint8_t y2 = 20 + 18 + 2;

	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(line1, Font_11x18, White);

	ssd1306_SetCursor(x2, y2);
	ssd1306_WriteString(line2, Font_11x18, White);

	//ssd1306_UpdateScreen();
}
