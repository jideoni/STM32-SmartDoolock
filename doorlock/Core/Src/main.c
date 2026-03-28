/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "temp_sensor.h"
#include "ble.h"
#include "eeprom.h"
#include "RC522_RFID.h"
//Branch testtttttt
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHARED_I2C &hi2c1
#define LOCK_DOOR_TIMEOUT 5000
#define SET_PIN_TIMEOUT 10000

//thread flags
#define BLE_TASK_THREAD_FLAG   (1U << 0)	//set bit 0. 1U = 00000001
#define DISPLAY_TASK_THREAD_FLAG   (1U << 1)	//set bit 1. 1U = 00000010

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim16;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* Definitions for BLE_Task */
osThreadId_t BLE_TaskHandle;
const osThreadAttr_t BLE_Task_attributes = { .name = "BLE_Task", .stack_size =
		128 * 8, .priority = (osPriority_t) osPriorityNormal, };
/* Definitions for Display_Task */
osThreadId_t Display_TaskHandle;
const osThreadAttr_t Display_Task_attributes =
		{ .name = "Display_Task", .stack_size = 128 * 4, .priority =
				(osPriority_t) osPriorityBelowNormal1, };
/* Definitions for TempSensor_Task */
osThreadId_t TempSensor_TaskHandle;
const osThreadAttr_t TempSensor_Task_attributes =
		{ .name = "TempSensor_Task", .stack_size = 128 * 4, .priority =
				(osPriority_t) osPriorityBelowNormal2, };
/* Definitions for Main_TaskHandle */
osThreadId_t Main_TaskHandleHandle;
const osThreadAttr_t Main_TaskHandle_attributes = { .name = "Main_TaskHandle",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityLow, };
/* Definitions for I2C1_Mutex */
osMutexId_t I2C1_MutexHandle;
const osMutexAttr_t I2C1_Mutex_attributes = { .name = "I2C1_Mutex" };
/* USER CODE BEGIN PV */
uint8_t buf[12];		//12 size buffer of type unsigned 8 bit integer
uint16_t passcode = 1010;	//0x03F2
float temp_func;
typedef uint32_t TimeStamp;

extern uint8_t eeprom_data[EEPROM_RW_DATA_SIZE];

TimeStamp timeCheck = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM16_Init(void);
void StartBLE_TaskTask(void *argument);
void StartDisplay_TaskTask(void *argument);
void StartTempSensor_TaskTask(void *argument);
void StartMain_TaskTask(void *argument);

/* USER CODE BEGIN PFP */
void lock_door(void);
void pin_reset_timeout(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	MX_TIM16_Init();
	/* USER CODE BEGIN 2 */
	ssd1306_Init();
	receive_BLE_command();
	displayTemperatureAtInit("Temperature");

	//Update_PIN(dataw1);
	//HAL_I2C_Mem_Write(SHARED_I2C, EEPROM_ADDR, (1 << 6), 2, dataw1, 4, 50);
	//HAL_Delay(5);

	//Start Timer 16
	HAL_TIM_Base_Start(&htim16);

	if (Read_Current_PIN(eeprom_data) == HAL_OK) {
		current_pin = (eeprom_data[0] << 8) | eeprom_data[1];
		sprintf(print_buffer, "Current Pin EEPROM: %d\r\n", current_pin);
		HAL_UART_Transmit(&huart2, (uint8_t*) print_buffer,
				strlen(print_buffer), 20);
	}

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();
	/* Create the mutex(es) */
	/* creation of I2C1_Mutex */
	I2C1_MutexHandle = osMutexNew(&I2C1_Mutex_attributes);

	/* Create the thread(s) */
	/* creation of BLE_Task */
	BLE_TaskHandle = osThreadNew(StartBLE_TaskTask, NULL, &BLE_Task_attributes);

	/* creation of Display_Task */
	Display_TaskHandle = osThreadNew(StartDisplay_TaskTask, NULL,
			&Display_Task_attributes);

	/* creation of TempSensor_Task */
	TempSensor_TaskHandle = osThreadNew(StartTempSensor_TaskTask, NULL,
			&TempSensor_Task_attributes);

	/* creation of Main_TaskHandle */
	Main_TaskHandleHandle = osThreadNew(StartMain_TaskTask, NULL,
			&Main_TaskHandle_attributes);

	/* Start scheduler */
	osKernelStart();
	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1
			| RCC_PERIPHCLK_I2C1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x0000020B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM16 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM16_Init(void) {

	/* USER CODE BEGIN TIM16_Init 0 */

	/* USER CODE END TIM16_Init 0 */

	/* USER CODE BEGIN TIM16_Init 1 */

	/* USER CODE END TIM16_Init 1 */
	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 16000 - 1;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = 65535;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM16_Init 2 */

	/* USER CODE END TIM16_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 38400;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel2_3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD2_Pin */
	GPIO_InitStruct.Pin = LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void lock_door() {
	timeCheck = time_now();
	if (timeCheck - timeOpen >= LOCK_DOOR_TIMEOUT) {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
		door_status = 0;	//door locked
	}
}

void pin_reset_timeout() {
	set_pin_timeout = time_now();
	if ((set_pin_timeout - set_pin_timeflag >= SET_PIN_TIMEOUT)
			&& set_pin_mode == ACTIVE) {
		serial_print("PIN Reset Timeout");
		pinResetFeedbacks(1, "Pin Reset Menu", "Timeout", "");
		set_pin_mode = INACTIVE;
		new_pin_signal = 0;
		display_temp = DISPLAY_TEMP;
		currentMillis = time_now();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		osThreadFlagsSet(BLE_TaskHandle, BLE_TASK_THREAD_FLAG);
	}
}
/* USER CODE END 4 */

/* USER CODE END Header_StartBLE_TaskTask */
void StartBLE_TaskTask(void *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osThreadFlagsWait(BLE_TASK_THREAD_FLAG, osFlagsWaitAny, osWaitForever);
		receive_BLE_command();
		osMutexAcquire(I2C1_MutexHandle, osWaitForever);
		process_BLE_command();
		osThreadFlagsSet(Display_TaskHandle, DISPLAY_TASK_THREAD_FLAG);
		osMutexRelease(I2C1_MutexHandle);
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartDisplay_TaskTask */
/**
 * @brief Function implementing the Display_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDisplay_TaskTask */
void StartDisplay_TaskTask(void *argument) {
	/* USER CODE BEGIN StartDisplay_TaskTask */
	/* Infinite loop */
	for (;;) {
		osThreadFlagsWait(DISPLAY_TASK_THREAD_FLAG, osFlagsWaitAny,
		osWaitForever);
		osMutexAcquire(I2C1_MutexHandle, osWaitForever);
		ssd1306_UpdateScreen();
		osMutexRelease(I2C1_MutexHandle);
		osDelay(1);
	}
	/* USER CODE END StartDisplay_TaskTask */
}

/* USER CODE BEGIN Header_StartTempSensor_TaskTask */
/**
 * @brief Function implementing the TempSensor_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTempSensor_TaskTask */
void StartTempSensor_TaskTask(void *argument) {
	/* USER CODE BEGIN StartTempSensor_TaskTask */
	/* Infinite loop */
	for (;;) {
		osMutexAcquire(I2C1_MutexHandle, osWaitForever);
		temp_Sensor_service();
		osMutexRelease(I2C1_MutexHandle);
		osThreadFlagsSet(Display_TaskHandle, DISPLAY_TASK_THREAD_FLAG);
		osDelay(3000);
	}
	/* USER CODE END StartTempSensor_TaskTask */
}

/* USER CODE BEGIN Header_StartMain_TaskTask */
/**
 * @brief Function implementing the Main_TaskHandle thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMain_TaskTask */
void StartMain_TaskTask(void *argument) {
	/* USER CODE BEGIN StartMain_TaskTask */
	/* Infinite loop */
	for (;;) {
		if (access_request_too_many_attempts
				== YES&& (time_now() - attempt_again_timer) >= PIN_ATTEMPT_RETRY_WAIT_TIME) {
			access_request_counter = 0;
			access_request_too_many_attempts = NO;
			attempt_again_timer = 0;
		}

		if (pin_reset_too_many_attempts == 1
				&& (time_now() - pin_reset_attempt_again_timer
						>= PIN_CHANGE_RETRY_WAIT_TIME)) {
			pin_change_counter = 0;
			pin_reset_too_many_attempts = 0;
			pin_reset_attempt_again_timer = 0;
		}
		lock_door();
		pin_reset_timeout();
		osDelay(1);
	}
	/* USER CODE END StartMain_TaskTask */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
