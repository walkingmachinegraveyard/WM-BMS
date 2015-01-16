/**
 ******************************************************************************
 * File Name          : CAN.c
 * Date               : 21/11/2014 10:48:29
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

/* Includes ------------------------------------------------------------------*/
#include "can.h"

#include "gpio.h"

#define _125KB
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
CAN_HandleTypeDef hcan2;

/* CAN2 init function */
void MX_CAN2_Init(void) {
    static CanRxMsgTypeDef rxMSG;
    static CanTxMsgTypeDef txMSG;

    CAN_FilterConfTypeDef sFilterConfig;

    hcan2.pTxMsg = &txMSG;
    hcan2.pRxMsg = &rxMSG;

    hcan2.Instance = CAN2;
    hcan2.Init.Mode = CAN_MODE_NORMAL;
    hcan2.Init.SJW = CAN_SJW_1TQ;
    hcan2.Init.TTCM = DISABLE;
    hcan2.Init.ABOM = DISABLE;
    hcan2.Init.AWUM = DISABLE;
    hcan2.Init.NART = DISABLE;
    hcan2.Init.RFLM = DISABLE;
    hcan2.Init.TXFP = ENABLE;
    hcan2.pRxMsg = &rxMSG;
    hcan2.pTxMsg = &txMSG;

    #ifdef  _125KB
    hcan2.Init.Prescaler = 16;
    hcan2.Init.BS1       = CAN_BS1_6TQ;
    hcan2.Init.BS2       = CAN_BS2_8TQ;
    #endif
    #ifdef  _250KB
    hcan2.Init.Prescaler = 8;
    hcan2.Init.BS1       = CAN_BS1_8TQ;
    hcan2.Init.BS2       = CAN_BS2_7TQ;
    #endif
    #ifdef  _500KB
    hcan2.Init.Prescaler = 4;
    hcan2.Init.BS1       = 8;
    hcan2.Init.BS2       = CAN_BS2_7TQ;
    #endif
    #ifdef  _1000KB
    hcan2.Init.Prescaler = 2;
    hcan2.Init.BS1       = CAN_BS1_8TQ;
    hcan2.Init.BS2       = CAN_BS2_7TQ;
    #endif

    HAL_CAN_Init(&hcan2);


    //Configure the CAN Filter
    sFilterConfig.FilterNumber = 1;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 1;

    if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) {
        while(1); //TODO Make an error handler
    }

    __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {

    GPIO_InitTypeDef GPIO_InitStruct;
    if (hcan->Instance == CAN2) {
        /* Peripheral clock enable */
        __CAN2_CLK_ENABLE();
        __CAN1_CLK_ENABLE();

        /**CAN2 GPIO Configuration
         PB5     ------> CAN2_RX
         PB6     ------> CAN2_TX
         */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral interrupt init*/
        /* Sets the priority grouping field */
        HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
        HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
        /* Sets the priority grouping field */
        HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
        HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan) {

    if (hcan->Instance == CAN2) {
        /* Peripheral clock disable */
        __CAN2_CLK_DISABLE();
        __CAN1_CLK_DISABLE();

        /**CAN2 GPIO Configuration
         PB5     ------> CAN2_RX
         PB6     ------> CAN2_TX
         */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5 | GPIO_PIN_6);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
        HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);

    }
}

void CAN2_RX0_IRQHandler(void) {
    HAL_CAN_IRQHandler(&hcan2);
}

void CAN2_RX1_IRQHandler(void) {
    HAL_CAN_IRQHandler(&hcan2);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
