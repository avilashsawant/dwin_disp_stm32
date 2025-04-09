/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_output_Pin GPIO_PIN_13
#define led_output_GPIO_Port GPIOC
#define En_A1_Pin GPIO_PIN_0
#define En_A1_GPIO_Port GPIOA
#define EN_A2_Pin GPIO_PIN_1
#define EN_A2_GPIO_Port GPIOA
#define EN_A3_Pin GPIO_PIN_2
#define EN_A3_GPIO_Port GPIOA
#define EN_A4_Pin GPIO_PIN_3
#define EN_A4_GPIO_Port GPIOA
#define EN_A5_Pin GPIO_PIN_4
#define EN_A5_GPIO_Port GPIOA
#define EN_A6_Pin GPIO_PIN_5
#define EN_A6_GPIO_Port GPIOA
#define EN_A7_Pin GPIO_PIN_6
#define EN_A7_GPIO_Port GPIOA
#define EN_A8_Pin GPIO_PIN_7
#define EN_A8_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_5
#define Buzzer_GPIO_Port GPIOC
#define SOL_1_Pin GPIO_PIN_8
#define SOL_1_GPIO_Port GPIOE
#define SOL_2_Pin GPIO_PIN_9
#define SOL_2_GPIO_Port GPIOE
#define SOL_3_Pin GPIO_PIN_10
#define SOL_3_GPIO_Port GPIOE
#define SOL_4_Pin GPIO_PIN_11
#define SOL_4_GPIO_Port GPIOE
#define SOL_5_Pin GPIO_PIN_12
#define SOL_5_GPIO_Port GPIOE
#define SOL_6_Pin GPIO_PIN_13
#define SOL_6_GPIO_Port GPIOE
#define SOL_7_Pin GPIO_PIN_14
#define SOL_7_GPIO_Port GPIOE
#define SOL_8_Pin GPIO_PIN_15
#define SOL_8_GPIO_Port GPIOE
#define OE_A_Pin GPIO_PIN_10
#define OE_A_GPIO_Port GPIOD
#define Stepper_clock_A_Pin GPIO_PIN_8
#define Stepper_clock_A_GPIO_Port GPIOA
#define H_F_B_Pin GPIO_PIN_11
#define H_F_B_GPIO_Port GPIOA
#define Stepper_dir_Pin GPIO_PIN_12
#define Stepper_dir_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
