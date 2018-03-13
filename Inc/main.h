/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
 
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define AMP_SD_Pin GPIO_PIN_14
#define AMP_SD_GPIO_Port GPIOC
#define OLED_RST_Pin GPIO_PIN_15
#define OLED_RST_GPIO_Port GPIOC
#define Batt_ADC_Pin GPIO_PIN_0
#define Batt_ADC_GPIO_Port GPIOA
#define SI_SEL_Pin GPIO_PIN_1
#define SI_SEL_GPIO_Port GPIOA
#define WIR433_TXD_Pin GPIO_PIN_2
#define WIR433_TXD_GPIO_Port GPIOA
#define WIR433_RXD_Pin GPIO_PIN_3
#define WIR433_RXD_GPIO_Port GPIOA
#define SI_SDN_Pin GPIO_PIN_4
#define SI_SDN_GPIO_Port GPIOA
#define KEY_ADC_Pin GPIO_PIN_0
#define KEY_ADC_GPIO_Port GPIOB
#define AIR_ADC_Pin GPIO_PIN_1
#define AIR_ADC_GPIO_Port GPIOB
#define SI_IRQ_Pin GPIO_PIN_2
#define SI_IRQ_GPIO_Port GPIOB
#define BLE_TXD_Pin GPIO_PIN_10
#define BLE_TXD_GPIO_Port GPIOB
#define BLE_RXD_Pin GPIO_PIN_11
#define BLE_RXD_GPIO_Port GPIOB
#define BLE_IRQ_Pin GPIO_PIN_12
#define BLE_IRQ_GPIO_Port GPIOB
#define BLE_IRQ_EXTI_IRQn EXTI15_10_IRQn
#define BLE_WKUP_Pin GPIO_PIN_13
#define BLE_WKUP_GPIO_Port GPIOB
#define BLE_RST_Pin GPIO_PIN_14
#define BLE_RST_GPIO_Port GPIOB
#define DS_DQ_Pin GPIO_PIN_15
#define DS_DQ_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_8
#define LED_BLUE_GPIO_Port GPIOA
#define SPK_DATA_Pin GPIO_PIN_9
#define SPK_DATA_GPIO_Port GPIOA
#define LED_YELLOW_Pin GPIO_PIN_11
#define LED_YELLOW_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_12
#define LED_RED_GPIO_Port GPIOA
#define POWER_EN_Pin GPIO_PIN_3
#define POWER_EN_GPIO_Port GPIOB
#define OLED_D1_Pin GPIO_PIN_4
#define OLED_D1_GPIO_Port GPIOB
#define OLED_D0_Pin GPIO_PIN_5
#define OLED_D0_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_6
#define OLED_DC_GPIO_Port GPIOB
#define WIR433_AUX_Pin GPIO_PIN_7
#define WIR433_AUX_GPIO_Port GPIOB
#define WIR433_M0_Pin GPIO_PIN_8
#define WIR433_M0_GPIO_Port GPIOB
#define WIR433_M1_Pin GPIO_PIN_9
#define WIR433_M1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define KEY_ADC_CH 1
#define BATT_ADC_CH 0
#define AIR_ADC_CH 2
 
#define STATUS_OFF 				0
#define STATUS_ON 				1	
#define STATUS_ERR				2
#define STATUS_IDLE				3	
#define STATUS_SET_SEL		4
#define STATUS_SET_HUD		5
#define STATUS_SET_BLE		6
#define STATUS_SET_DATA		7
#define STATUS_SET_AIRPRESS_CAL	8
#define STATUS_SET_SN	10
#define STATUS_PAIR		9

#define PAGE_RUN_ONE 0
#define PAGE_RUN_TWO 1
#define PAGE_RUN_THREE 2

#define KEY1 0x01
#define KEY2 0x02
#define KEY3 0x04
#define KEY4 0x08
#define KEY5 0x10

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
