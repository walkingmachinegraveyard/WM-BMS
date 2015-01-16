/**
  ******************************************************************************
  * File Name          : CAN.h
  * Date               : 12/09/2014 19:33:44
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
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
#ifndef __can_H
#define __can_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <FreeRTOS.h>
#include <semphr.h>
#include "stm32f4xx_hal.h"

/*Définition des GPIOs pour le CAN BUS*/
#define CAN_BUS                     CAN2
#define CAN_CLK                     (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
#define CAN_RX_PIN                  GPIO_Pin_5
#define CAN_TX_PIN                  GPIO_Pin_6
#define CAN_GPIO_PORT               GPIOB
#define CAN_GPIO_CLK                RCC_AHB1Periph_GPIOB
#define CAN_AF_PORT                 GPIO_AF_CAN2
#define CAN_RX_SOURCE               GPIO_PinSource5
#define CAN_TX_SOURCE               GPIO_PinSource6

#define byte uint8_t

CAN_HandleTypeDef hcan2;

void MX_CAN2_Init(void);
void startCANTasks(void);



#ifdef __cplusplus
}
#endif
#endif /*__ can_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
