/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
regulator PID[4];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define disk_to_real	0.4743
#define freq			0.01
#define arm_count		2870.0
#define moving_count	770.0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM7_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Base_Start_IT(&htim6);

  init_pid();

  set_voltage(0, 0.0);
  set_voltage(1, 0.0);
  set_voltage(2, 0.0);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void get_encoder_data(void)
{
	int16_t Enc[3] = {0.0, 0.0};

	Enc[0] = TIM8->CNT;
	Enc[1] = TIM2->CNT;
	Enc[2] = TIM3->CNT;

	// speed
	float tmp_dist = 0.0;
	tmp_dist = Enc[0] * (disk_to_real / moving_count);
	PID[0].in = tmp_dist / freq;

	tmp_dist = Enc[1] * (disk_to_real / moving_count);
	PID[1].in = tmp_dist / freq;

	tmp_dist = Enc[2] * (disk_to_real / 1340.0);
	PID[2].in = tmp_dist / freq;

	// distanse
	tmp_dist = Enc[2] * (disk_to_real / 1340.0) ;
	PID[3].in += tmp_dist * 360.0;

	TIM8->CNT = 0;
	TIM2->CNT = 0;
	TIM3->CNT = 0;
}

void calc_pid(void)
{
	PID[0].error = PID[0].target - PID[0].in;
	PID[0].sum_error += PID[0].error;
	if(PID[0].sum_error > PID[0].max_sum_error) PID[0].sum_error = PID[0].max_sum_error;
	if(PID[0].sum_error < -PID[0].max_sum_error) PID[0].sum_error = -PID[0].max_sum_error;

	PID[0].out = (PID[0].p_k * PID[0].error) + (PID[0].i_k * PID[0].sum_error) + (PID[0].d_k * PID[0].prev_error);
	if(PID[0].out > PID[0].max_out) PID[0].out = PID[0].max_out;
	if(PID[0].out < -PID[0].max_out) PID[0].out = -PID[0].max_out;
	PID[0].prev_error = PID[0].error;

	PID[1].error = PID[1].in - PID[1].target;
	PID[1].sum_error += PID[1].error;
	if(PID[1].sum_error > PID[1].max_sum_error) PID[1].sum_error = PID[1].max_sum_error;
	if(PID[1].sum_error < -PID[1].max_sum_error) PID[1].sum_error = -PID[1].max_sum_error;

	PID[1].out = (PID[1].p_k * PID[1].error) + (PID[1].i_k * PID[1].sum_error) + (PID[1].d_k * PID[1].prev_error);
	if(PID[1].out > PID[1].max_out) PID[1].out = PID[1].max_out;
	if(PID[1].out < -PID[1].max_out) PID[1].out = -PID[1].max_out;
	PID[1].prev_error = PID[1].error;

	PID[2].error = PID[2].in - PID[2].target;
	PID[2].sum_error += PID[2].error;
	if(PID[2].sum_error > PID[2].max_sum_error) PID[2].sum_error = PID[2].max_sum_error;
	if(PID[2].sum_error < -PID[2].max_sum_error) PID[2].sum_error = -PID[2].max_sum_error;

	PID[2].out = (PID[2].p_k * PID[2].error) + (PID[2].i_k * PID[2].sum_error) + (PID[2].d_k * PID[2].prev_error);
	if(PID[2].out > PID[2].max_out) PID[2].out = PID[2].max_out;
	if(PID[2].out < -PID[2].max_out) PID[2].out = -PID[2].max_out;
	PID[2].prev_error = PID[2].error;

	PID[3].error = PID[3].target - PID[3].in;
	PID[3].sum_error += PID[3].error;
	if(PID[3].sum_error > PID[3].max_sum_error) PID[3].sum_error = PID[3].max_sum_error;
	if(PID[3].sum_error < -PID[3].max_sum_error) PID[3].sum_error = -PID[3].max_sum_error;

	PID[3].out = (PID[3].p_k * PID[3].error) + (PID[3].i_k * PID[3].sum_error) + (PID[3].d_k * PID[3].prev_error);
	if(PID[3].out > PID[3].max_out) PID[3].out = PID[3].max_out;
	if(PID[3].out < -PID[3].max_out) PID[3].out = -PID[3].max_out;
	PID[3].prev_error = PID[3].error;
}

void set_voltage(uint8_t motor, float duty)
{
	if(duty > 1.0) duty = 1.0;
	if(duty < -1.0) duty = -1.0;
	switch(motor)
	{
	case 0:
		if(duty >= 0.0)
		{
			TIM4->CCR1 = ((int32_t)(TIM4->ARR * duty));
			HAL_GPIO_WritePin(BTN1_DIR_GPIO_Port, BTN1_DIR_Pin, GPIO_PIN_RESET);
		} else {
			TIM4->CCR1 = ((int32_t)(TIM4->ARR + (TIM4->ARR * duty)));
			HAL_GPIO_WritePin(BTN1_DIR_GPIO_Port, BTN1_DIR_Pin, GPIO_PIN_SET);
		}
		break;
	case 1:
		if(duty >= 0.0)
		{
			TIM4->CCR2 = ((int32_t)(TIM4->ARR * duty));
			HAL_GPIO_WritePin(BTN2_DIR_GPIO_Port, BTN2_DIR_Pin, GPIO_PIN_RESET);
		} else {
			TIM4->CCR2 = ((int32_t)(TIM4->ARR + (TIM4->ARR * duty)));
			HAL_GPIO_WritePin(BTN2_DIR_GPIO_Port, BTN2_DIR_Pin, GPIO_PIN_SET);
		}
		break;
	case 2:
		if(duty >= 0.0)
		{
			TIM4->CCR3 = ((int32_t)(TIM4->ARR * duty));
			HAL_GPIO_WritePin(BTN3_DIR_GPIO_Port, BTN3_DIR_Pin, GPIO_PIN_RESET);
		} else {
			TIM4->CCR3 = ((int32_t)(TIM4->ARR + (TIM4->ARR * duty)));
			HAL_GPIO_WritePin(BTN3_DIR_GPIO_Port, BTN3_DIR_Pin, GPIO_PIN_SET);
		}
		break;
	}
}

void init_pid(void)
{
	// Speed regulators
	PID[0].p_k = 0.050;
	PID[0].i_k = 0.20;
	PID[0].d_k = 0.0;
	PID[0].error = 0.0;
	PID[0].sum_error = 0.0;
	PID[0].max_sum_error = 10.0;
	PID[0].min_out = 0.1;
	PID[0].max_out = 1.0;
	PID[0].out = 0.0;
	PID[0].target = 0.0;

	PID[1].p_k = 0.050;
	PID[1].i_k = 0.200;
	PID[1].d_k = 0.0;
	PID[1].error = 0.0;
	PID[1].sum_error = 0.0;
	PID[1].max_sum_error = 10.0;
	PID[1].min_out = 0.1;
	PID[1].max_out = 1.0;
	PID[1].out = 0.0;
	PID[1].target = 0.0;

	PID[2].p_k = 0.40;
	PID[2].i_k = 0.0780;
	PID[2].d_k = 0.0;
	PID[2].error = 0.0;
	PID[2].sum_error = 0.0;
	PID[2].max_sum_error = 5.0;
	PID[2].min_out = 0.1;
	PID[2].max_out = 1.0;
	PID[2].out = 0.0;
	PID[2].target = 0.0;

	// angle regulator
	PID[3].p_k = 1.0;
	PID[3].i_k = 0.0;
	PID[3].d_k = 0.0;
	PID[3].error = 0.0;
	PID[3].sum_error = 0.0;
	PID[3].max_sum_error = 10.0;
	PID[3].min_out = 0.1;
	PID[3].max_out = 0.25;
	PID[3].out = 0.0;
	PID[3].target = 0.0;
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
