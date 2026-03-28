#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__

//#include "main.h"
#include <_ansi.h>

_BEGIN_STD_C

//extern TIM_HandleTypeDef htim16;

void ssd1306_RectangleFill(void);
void ssd1306_Circle(void);
void displayTemperature(char *heading);
void displayTemperatureAtInit(char *heading);
void pinResetFeedbacks(int line_count, char *heading, char *line1, char *line2);
void pinResetFeedbacksNoHeading(char *line1, char *line2);

_END_STD_C

#endif // __SSD1306_TEST_H__
