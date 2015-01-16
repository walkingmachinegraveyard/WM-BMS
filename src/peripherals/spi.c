/**
 ******************************************************************************
 * File Name          : SPI.c
 * Date               : 12/09/2014 19:33:45
 * Description        : This file provides code for the configuration
 *                      of the SPI instances.
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
#include "spi.h"
#include "Board.h"
#include "gpio.h"

SPI_HandleTypeDef hspi1;

/**
 * Start the Full Duplex Communication process
 *  While the SPI in TransmitReceive process, user can transmit data through
 *  "TxBuffer" buffer & receive data through "RxBuffer"
 *  Timeout is set to 5S
 *  @param ad72
 *  @return
 */
void spi_exchange(ad7280a_t *ad72) {

    ad72->RxData = 0;

    // Fill up the transmit buffers
    ad72->TxBuffer[0] = ad72->TxData >> 24;
    ad72->TxBuffer[1] = ad72->TxData >> 16;
    ad72->TxBuffer[2] = ad72->TxData >> 8;
    ad72->TxBuffer[3] = ad72->TxData;

    // Latch the CS
    HAL_GPIO_WritePin(GPIOC, (1 << GPIOC_SPI1_CS_PIN), GPIO_PIN_RESET);

    // Start the transfer
    switch (HAL_SPI_TransmitReceive(&hspi1,
            ad72->TxBuffer, ad72->RxBuffer, 4,
            250)) {
    case HAL_OK:
        /* Blink Green LED: Transfer in transmission process is correct */
        ad72->transmitting = 1;

        break;

    case HAL_TIMEOUT:
        HAL_GPIO_WritePin(GPIOE, (1 << GPIOE_RED_LED), GPIO_PIN_SET);
        break;

    case HAL_ERROR:
        HAL_GPIO_WritePin(GPIOE, (1 << GPIOE_RED_LED), GPIO_PIN_SET);
        break;

    default:
        break;
    }

    // Latch the CS
    HAL_GPIO_WritePin(GPIOC, (1 << GPIOC_SPI1_CS_PIN), GPIO_PIN_SET);

    // Merge the four bytes received
    ad72->RxData = ad72->RxBuffer[0];
    ad72->RxData = ad72->RxData <<8;
    ad72->RxData |= ad72->RxBuffer[1];
    ad72->RxData = ad72->RxData << 8;
    ad72->RxData |= ad72->RxBuffer[2];
    ad72->RxData = ad72->RxData << 8;
    ad72->RxData |= ad72->RxBuffer[3];

}

/* SPI1 init function */
void MX_SPI1_Init(void) {

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    hspi1.Init.CRCPolynomial = 7;
    HAL_SPI_Init(&hspi1);

    // Latch the CS
    HAL_GPIO_WritePin(GPIOC, (1 << GPIOC_SPI1_CS_PIN), GPIO_PIN_SET);

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {

    GPIO_InitTypeDef GPIO_InitStruct;
    if (hspi->Instance == SPI1) {
        /* Peripheral clock enable */
        __SPI1_CLK_ENABLE();

        /**SPI1 GPIO Configuration
         PA5     ------> SPI1_SCK
         PA6     ------> SPI1_MISO
         PA7     ------> SPI1_MOSI
         */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) {

    if (hspi->Instance == SPI1) {
        /* Peripheral clock disable */
        __SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
         PA5     ------> SPI1_SCK
         PA6     ------> SPI1_MISO
         PA7     ------> SPI1_MOSI
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    }
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
