/**
 ******************************************************************************
 * File Name          : ADC.c
 * Date               : 16/09/2014 22:32:34
 * Description        : This file provides code for the configuration
 *                      of the ADC instances.
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
#include "adc.h"

#include "gpio.h"

#define ADC_AVERAGE_AMMOUNT 5

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

void ADC_BeginConvertions() {
    HAL_ADC_Start_IT(&hadc1);
    HAL_ADC_Start_IT(&hadc2);
}

/**
 * @brief  Conversion complete callback in non blocking mode
 * @param  AdcHandle : AdcHandle handle
 * @note   This example shows a simple way to report end of conversion, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
    static uint16_t i1 = 0;
    static uint16_t i2 = 0;
    static uint32_t samples_averages_1 = 0;
    static uint32_t samples_averages_2 = 0;

    if (AdcHandle->Instance == ADC1) {

        if (i1 < ADC_AVERAGE_AMMOUNT) {
            samples_averages_1 += HAL_ADC_GetValue(AdcHandle);
            ++i1;
        } else {
            uhADCxConvertedValue1 = samples_averages_1
                    / ADC_AVERAGE_AMMOUNT;
            i1 = 0;
            samples_averages_1 = 0;
        }
    }

    if (AdcHandle->Instance == ADC2) {

        if (i2 < ADC_AVERAGE_AMMOUNT) {
            samples_averages_2 += HAL_ADC_GetValue(AdcHandle);
            ++i2;
        } else {
            uhADCxConvertedValue2 = samples_averages_2
                    / ADC_AVERAGE_AMMOUNT;
            i2 = 0;
            samples_averages_2 = 0;
        }
    }


}

void ADC_IRQHandler(void) {
    HAL_ADC_IRQHandler(&hadc1);
    HAL_ADC_IRQHandler(&hadc2);
}
/* ADC1 init function */
void MX_ADC1_Init(void) {

    ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION12b;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfDiscConversion = 1;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = EOC_SINGLE_CONV;
    HAL_ADC_Init(&hadc1);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}
/* ADC2 init function */
void MX_ADC2_Init(void) {

    ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc2.Instance = ADC2;
    hadc2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hadc2.Init.Resolution = ADC_RESOLUTION12b;
    hadc2.Init.ScanConvMode = DISABLE;
    hadc2.Init.ContinuousConvMode = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.NbrOfDiscConversion = 1;
    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion = 1;
    hadc2.Init.DMAContinuousRequests = DISABLE;
    hadc2.Init.EOCSelection = EOC_SINGLE_CONV;
    HAL_ADC_Init(&hadc2);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    HAL_ADC_ConfigChannel(&hadc2, &sConfig);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) {

    GPIO_InitTypeDef GPIO_InitStruct;
    if (hadc->Instance == ADC1) {
        /* Peripheral clock enable */
        __ADC1_CLK_ENABLE();

        /**ADC1 GPIO Configuration
         PC1     ------> ADC1_IN11
         */
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* Peripheral interrupt init*/
        /* Sets the priority grouping field */
        HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
    } else if (hadc->Instance == ADC2) {
        /* Peripheral clock enable */
        __ADC2_CLK_ENABLE();

        /**ADC2 GPIO Configuration
         PC0     ------> ADC2_IN10
         */
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* Peripheral interrupt init*/
        /* Sets the priority grouping field */
        HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc) {

    if (hadc->Instance == ADC1) {
        /* Peripheral clock disable */
        __ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
         PC1     ------> ADC1_IN11
         */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(ADC_IRQn);
    } else if (hadc->Instance == ADC2) {
        /* Peripheral clock disable */
        __ADC2_CLK_DISABLE();

        /**ADC2 GPIO Configuration
         PC0     ------> ADC2_IN10
         */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(ADC_IRQn);
    }
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
