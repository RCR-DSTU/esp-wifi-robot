/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
	float in;
	float target;
	float out;
	float error;
	float sum_error;
	float prev_error;
	float max_out;
	float min_out;
	float max_sum_error;
	uint16_t count;
	float p_k;
	float i_k;
	float d_k;
}regulator;

extern regulator PID[4];
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern char sync;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void calc_pid(void);
void set_voltage(uint8_t motor, float duty);
void get_encoder_data(void);
void init_pid(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN6_DIR_Pin GPIO_PIN_2
#define BTN6_DIR_GPIO_Port GPIOE
#define EXTI5_Pin GPIO_PIN_4
#define EXTI5_GPIO_Port GPIOE
#define EXTI5_EXTI_IRQn EXTI4_IRQn
#define BTN8_PWM_Pin GPIO_PIN_5
#define BTN8_PWM_GPIO_Port GPIOE
#define BTN7_PWM_Pin GPIO_PIN_6
#define BTN7_PWM_GPIO_Port GPIOE
#define BTN1_DIR_Pin GPIO_PIN_14
#define BTN1_DIR_GPIO_Port GPIOC
#define BTN2_DIR_Pin GPIO_PIN_15
#define BTN2_DIR_GPIO_Port GPIOC
#define ADC7_Pin GPIO_PIN_1
#define ADC7_GPIO_Port GPIOC
#define ADC8_Pin GPIO_PIN_2
#define ADC8_GPIO_Port GPIOC
#define ADC1_Pin GPIO_PIN_0
#define ADC1_GPIO_Port GPIOA
#define ADC2_Pin GPIO_PIN_1
#define ADC2_GPIO_Port GPIOA
#define ADC3_Pin GPIO_PIN_2
#define ADC3_GPIO_Port GPIOA
#define ADC4_Pin GPIO_PIN_3
#define ADC4_GPIO_Port GPIOA
#define PIN3_12V_Pin GPIO_PIN_5
#define PIN3_12V_GPIO_Port GPIOA
#define ENC3A_Pin GPIO_PIN_6
#define ENC3A_GPIO_Port GPIOA
#define ENC3B_Pin GPIO_PIN_7
#define ENC3B_GPIO_Port GPIOA
#define ADC9_Pin GPIO_PIN_4
#define ADC9_GPIO_Port GPIOC
#define ADC10_Pin GPIO_PIN_5
#define ADC10_GPIO_Port GPIOC
#define ADC5_Pin GPIO_PIN_0
#define ADC5_GPIO_Port GPIOB
#define ADC6_Pin GPIO_PIN_1
#define ADC6_GPIO_Port GPIOB
#define BTN5_DIR_Pin GPIO_PIN_7
#define BTN5_DIR_GPIO_Port GPIOE
#define BTN7_DIR_Pin GPIO_PIN_8
#define BTN7_DIR_GPIO_Port GPIOE
#define BTN3_DIR_Pin GPIO_PIN_10
#define BTN3_DIR_GPIO_Port GPIOE
#define BTN9_DIR_Pin GPIO_PIN_12
#define BTN9_DIR_GPIO_Port GPIOE
#define BTN8_DIR_Pin GPIO_PIN_13
#define BTN8_DIR_GPIO_Port GPIOE
#define BTN4_DIR_Pin GPIO_PIN_14
#define BTN4_DIR_GPIO_Port GPIOE
#define I2C_SCL_Pin GPIO_PIN_10
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_11
#define I2C_SDA_GPIO_Port GPIOB
#define BTN10_DIR_Pin GPIO_PIN_13
#define BTN10_DIR_GPIO_Port GPIOB
#define BTN9_PWM_Pin GPIO_PIN_14
#define BTN9_PWM_GPIO_Port GPIOB
#define BTN10_PWM_Pin GPIO_PIN_15
#define BTN10_PWM_GPIO_Port GPIOB
#define EXTI8_Pin GPIO_PIN_8
#define EXTI8_GPIO_Port GPIOD
#define EXTI8_EXTI_IRQn EXTI9_5_IRQn
#define EXTI9_Pin GPIO_PIN_9
#define EXTI9_GPIO_Port GPIOD
#define EXTI9_EXTI_IRQn EXTI9_5_IRQn
#define EXTI10_Pin GPIO_PIN_10
#define EXTI10_GPIO_Port GPIOD
#define EXTI10_EXTI_IRQn EXTI15_10_IRQn
#define PIN4_12V_Pin GPIO_PIN_11
#define PIN4_12V_GPIO_Port GPIOD
#define BTN1_PWM_Pin GPIO_PIN_12
#define BTN1_PWM_GPIO_Port GPIOD
#define BTN2_PWM_Pin GPIO_PIN_13
#define BTN2_PWM_GPIO_Port GPIOD
#define BTN3_PWM_Pin GPIO_PIN_14
#define BTN3_PWM_GPIO_Port GPIOD
#define BTN4_PWM_Pin GPIO_PIN_15
#define BTN4_PWM_GPIO_Port GPIOD
#define ENC1A_Pin GPIO_PIN_6
#define ENC1A_GPIO_Port GPIOC
#define ENC1B_Pin GPIO_PIN_7
#define ENC1B_GPIO_Port GPIOC
#define PIN1_12V_Pin GPIO_PIN_8
#define PIN1_12V_GPIO_Port GPIOC
#define PIN2_12V_Pin GPIO_PIN_9
#define PIN2_12V_GPIO_Port GPIOC
#define ENC2A_Pin GPIO_PIN_15
#define ENC2A_GPIO_Port GPIOA
#define TX3_Pin GPIO_PIN_10
#define TX3_GPIO_Port GPIOC
#define RX3_Pin GPIO_PIN_11
#define RX3_GPIO_Port GPIOC
#define EXTI1_Pin GPIO_PIN_0
#define EXTI1_GPIO_Port GPIOD
#define EXTI1_EXTI_IRQn EXTI0_IRQn
#define EXTI2_Pin GPIO_PIN_1
#define EXTI2_GPIO_Port GPIOD
#define EXTI2_EXTI_IRQn EXTI1_IRQn
#define EXTI3_Pin GPIO_PIN_2
#define EXTI3_GPIO_Port GPIOD
#define EXTI3_EXTI_IRQn EXTI2_IRQn
#define EXTI4_Pin GPIO_PIN_3
#define EXTI4_GPIO_Port GPIOD
#define EXTI4_EXTI_IRQn EXTI3_IRQn
#define EXTI6_Pin GPIO_PIN_6
#define EXTI6_GPIO_Port GPIOD
#define EXTI6_EXTI_IRQn EXTI9_5_IRQn
#define EXTI7_Pin GPIO_PIN_7
#define EXTI7_GPIO_Port GPIOD
#define EXTI7_EXTI_IRQn EXTI9_5_IRQn
#define ENC2B_Pin GPIO_PIN_3
#define ENC2B_GPIO_Port GPIOB
#define PIN6_12V_Pin GPIO_PIN_4
#define PIN6_12V_GPIO_Port GPIOB
#define PIN5_12V_Pin GPIO_PIN_5
#define PIN5_12V_GPIO_Port GPIOB
#define TX1_Pin GPIO_PIN_6
#define TX1_GPIO_Port GPIOB
#define RX1_Pin GPIO_PIN_7
#define RX1_GPIO_Port GPIOB
#define BTN6_PWM_Pin GPIO_PIN_8
#define BTN6_PWM_GPIO_Port GPIOB
#define BTN5_PWM_Pin GPIO_PIN_9
#define BTN5_PWM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
