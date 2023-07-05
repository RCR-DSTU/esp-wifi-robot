/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static struct {
	uint8_t modbus[17];
	bool flag;
	float speed;
	uint8_t direction;
	float arm_angle;
	uint8_t crc_check[2];
}robot;

char sync = 0;
/* USER CODE END Variables */
/* Definitions for startTask */
osThreadId_t startTaskHandle;
uint32_t startTaskBuffer[ 128 ];
osStaticThreadDef_t startTaskControlBlock;
const osThreadAttr_t startTask_attributes = {
  .name = "startTask",
  .cb_mem = &startTaskControlBlock,
  .cb_size = sizeof(startTaskControlBlock),
  .stack_mem = &startTaskBuffer[0],
  .stack_size = sizeof(startTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Modbus */
osThreadId_t ModbusHandle;
uint32_t ModbusBuffer[ 128 ];
osStaticThreadDef_t ModbusControlBlock;
const osThreadAttr_t Modbus_attributes = {
  .name = "Modbus",
  .cb_mem = &ModbusControlBlock,
  .cb_size = sizeof(ModbusControlBlock),
  .stack_mem = &ModbusBuffer[0],
  .stack_size = sizeof(ModbusBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static unsigned char CRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static char CRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static void reset_options(void)
{
	// speed
	PID[0].target = 0.0;
	PID[1].target = 0.0;

	PID[0].sum_error = 0.0;
	PID[1].sum_error = 0.0;
	// angle pos
	PID[3].target = 0.0;
	PID[2].sum_error = 0.0;
	// stat parameters
	robot.arm_angle = 0.0;
	robot.direction = 0;
	robot.flag = false;
	robot.speed = 0.0;
	memset(robot.modbus, 0, sizeof(robot.modbus));
}

static void check_modbus_crc(unsigned char *puchmsg, unsigned short usDataLen)
{
	// low high crc bytes initialized
	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;

	unsigned index;
	while(usDataLen--) {
		index = uchCRCHi ^ *puchmsg++;
		uchCRCHi = uchCRCLo ^ CRCHi[index];
		uchCRCLo = CRCLo[index];
	}
	robot.crc_check[0] = uchCRCLo;
	robot.crc_check[1] = uchCRCHi;
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void ModBus_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of startTask */
  startTaskHandle = osThreadNew(StartDefaultTask, NULL, &startTask_attributes);

  /* creation of Modbus */
  ModbusHandle = osThreadNew(ModBus_Task, NULL, &Modbus_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the startTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	reset_options();
  /* Infinite loop */
  for(;;)
  {
	  if(!robot.flag) xTaskNotifyGive(ModbusHandle);
	  else
	  {
		  robot.flag = false;
		  if(robot.arm_angle >= 0.0 && robot.arm_angle <= 180.0)
		  {
			  PID[3].target = -robot.arm_angle;
		  } else PID[3].target = 0.0;
		  if(robot.speed <= 0.25)
		  {
			  switch(robot.direction)
			  {
			  case 0:
				  PID[0].target = 0.0;
				  PID[1].target = 0.0;
				  break;

			  case 1:
				  PID[0].target = -robot.speed;
				  PID[1].target = robot.speed;
				  break;

			  case 2:
				  PID[0].target = robot.speed;
				  PID[1].target = robot.speed;
				  break;

			  case 3:
				  PID[0].target = -robot.speed;
				  PID[1].target = -robot.speed;
				  break;

			  case 4:
				  PID[0].target = robot.speed;
				  PID[1].target = -robot.speed;
				  break;
			  }
		  }
		  robot.flag = false;
	  }
	  sync = 0;
	  osDelay(10 / portTICK_RATE_MS);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_ModBus_Task */
/**
* @brief Function implementing the Modbus thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ModBus_Task */
void ModBus_Task(void *argument)
{
  /* USER CODE BEGIN ModBus_Task */
	uint32_t prev_time = HAL_GetTick();
	uint32_t time = 0;
  /* Infinite loop */
  for(;;)
  {
    ulTaskNotifyTake(0, portMAX_DELAY);
    if(!robot.flag) HAL_UART_Receive_DMA(&huart1, robot.modbus, sizeof(robot.modbus));
    time = HAL_GetTick();
    if(sync == 1)
    {
    	prev_time = time;
    	robot.flag = true;
    	check_modbus_crc(robot.modbus, sizeof(robot.modbus) - 4);
    	if(robot.crc_check[0] == robot.modbus[13] && robot.crc_check[1] == robot.modbus[14])
    	{
    		uint32_t tmp_speed_uint32_t = 0;
    		tmp_speed_uint32_t += robot.modbus[7] << 24;
    		tmp_speed_uint32_t += robot.modbus[8] << 16;
    		tmp_speed_uint32_t += robot.modbus[9] << 8;
    		tmp_speed_uint32_t += robot.modbus[10];

    		memcpy(&robot.speed, &tmp_speed_uint32_t, sizeof(tmp_speed_uint32_t));

    		robot.arm_angle = robot.modbus[12];
    		robot.direction = robot.modbus[11];
    	}
    	xTaskNotifyGive(startTaskHandle);
    } else
    {
    	if(time - prev_time >= 2000)
    	{
    		reset_options();
    	}
    }
    osDelay(10 / portTICK_RATE_MS);
  }
  /* USER CODE END ModBus_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

