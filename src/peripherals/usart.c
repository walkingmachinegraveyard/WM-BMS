/**
  ******************************************************************************
  * File Name          : USART.c
  * Date               : 12/09/2014 19:33:45
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#include "usart.h"

#include "gpio.h"
#include "string.h"

#include <stdint.h>

/**
 * This function initialise the console
 * @param c The address of the console that needs to be initialised
 */

UART_HandleTypeDef huart1;

#ifdef TODO
int16_t console_readline(struct console *c, uint8_t *buffer, uint16_t count) {
    (void) c;
    (void) buffer;
    (void) count;
    return 0;
}

int16_t console_read(struct console *c, uint8_t *buffer, uint16_t count) {
    (void) c;
    (void) buffer;
    (void) count;
    return 0;
}
#endif

void console_writeline(uint8_t *buffer) {

    console_write(buffer);
    console_write((uint8_t*)"\r\n");

}

void console_write( uint8_t *buffer) {

    HAL_UART_Transmit(&(huart1), buffer, strlen(buffer), 100);

}

char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i = -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void consolePrintStatus(cell_t cells[], acs_t *acs, therm_t therms[],
                           battery_t *batt) {

  char buffer[50];

  // Screen Clear
  console_write((uint8_t*)"\033[2J\033[1;1H");

  // Cell 1
  console_write((uint8_t*)"cell1: ");
  console_write((uint8_t*)itoa(cells[0].voltage, buffer));

  console_write((uint8_t*)" bal: ");
  console_write((uint8_t*)itoa(cells[0].is_balancing, buffer));

  console_write((uint8_t*)" HEALTH: ");
  switch(cells[0].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write((uint8_t*)" UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write((uint8_t*)" GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write((uint8_t*)" OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write((uint8_t*)" OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write((uint8_t*)" UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write((uint8_t*)" UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write((uint8_t*)" DEAD ");
  }
  console_writeline((uint8_t*)"");


  // Cell 2
  console_write((uint8_t*)"cell2: ");
  console_write((uint8_t*)itoa(cells[1].voltage, buffer));

  console_write((uint8_t*)" bal: ");
  console_write((uint8_t*)itoa(cells[1].is_balancing, buffer));

  console_write((uint8_t*)" HEALTH: ");
  switch(cells[1].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write((uint8_t*)" UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write((uint8_t*)" GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write((uint8_t*)" OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write((uint8_t*)" OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write((uint8_t*)" UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write((uint8_t*)" UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write((uint8_t*)" DEAD ");
  }
  console_writeline((uint8_t*)"");

  // Cell 3
  console_write((uint8_t*)"cell3: ");
  console_write((uint8_t*)itoa(cells[2].voltage, buffer));

  console_write((uint8_t*)" bal: ");
  console_write((uint8_t*)itoa(cells[2].is_balancing, buffer));

  console_write((uint8_t*)" HEALTH: ");
  switch(cells[2].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write((uint8_t*)" UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write((uint8_t*)" GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write((uint8_t*)" OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write((uint8_t*)" OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write((uint8_t*)" UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write((uint8_t*)" UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write((uint8_t*)" DEAD ");
  }
  console_writeline((uint8_t*)"");

  // Cell 4
  console_write((uint8_t*)"cell4: ");
  console_write((uint8_t*)itoa(cells[3].voltage, buffer));

  console_write((uint8_t*)" bal: ");
  console_write((uint8_t*)itoa(cells[3].is_balancing, buffer));

  console_write((uint8_t*)" HEALTH: ");
  switch(cells[3].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write((uint8_t*)" UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write((uint8_t*)" GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write((uint8_t*)" OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write((uint8_t*)" OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write((uint8_t*)" UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write((uint8_t*)" UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write((uint8_t*)" DEAD ");
  }
  console_writeline((uint8_t*)"");

  // Cell 5
  console_write((uint8_t*)"cell5: ");
  console_write((uint8_t*)itoa(cells[4].voltage, buffer));

  console_write((uint8_t*)" bal: ");
  console_write((uint8_t*)itoa(cells[4].is_balancing, buffer));

  console_write((uint8_t*)" HEALTH: ");
  switch(cells[4].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write((uint8_t*)" UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write((uint8_t*)" GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write((uint8_t*)" OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write((uint8_t*)" OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write((uint8_t*)" UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write((uint8_t*)" UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write((uint8_t*)" DEAD ");
  }
  console_writeline((uint8_t*)"");

  // Cell 6
  console_write((uint8_t*)"cell6: ");
  console_write((uint8_t*)itoa(cells[5].voltage, buffer));

  console_write((uint8_t*)" bal: ");
  console_write((uint8_t*)itoa(cells[5].is_balancing, buffer));

  console_write((uint8_t*)" HEALTH: ");
  switch(cells[5].health) {
  case CELL_HEALTH_UNKNOWN:
    console_write((uint8_t*)" UNKNOWN ");
    break;
  case CELL_HEALTH_GOOD:
    console_write((uint8_t*)" GOOD ");
    break;
  case CELL_HEALTH_OVERHEAT:
    console_write((uint8_t*)" OVERHEAT ");
    break;
  case CELL_HEALTH_OVER_VOLTAGE:
    console_write((uint8_t*)" OVERVOLTAGE ");
    break;
  case CELL_HEALTH_UNDER_VOLTAGE:
    console_write((uint8_t*)" UNDERVOLTAGE ");
    break;
  case CELL_HEALTH_UNSPECIFIED_FAILURE:
    console_write((uint8_t*) " UNSPECIFIED_FAILURE ");
    break;
  case CELL_HEALTH_DEAD:
    console_write((uint8_t*)" DEAD ");
  }
  console_writeline((uint8_t*)"");

  // Total Voltage
  console_write((uint8_t*)"Total: ");
  console_writeline((uint8_t*)itoa(batt->voltage, buffer));

  // Output Current
  console_write((uint8_t*)"OutCur: ");
  if (acs->current_direction == CURRENT_IS_NEGATIVE)
    console_write((uint8_t*)"-");
    itoa(acs_read_currsens(acs), buffer);
  console_writeline((uint8_t*)buffer);

  //Temperature Therm 1/2
  console_writeline((uint8_t*)"Thermistor #1");
  console_writeline((uint8_t*)itoa(therms[0].temperature, buffer));
  //Temperature Therm 2/2
  console_writeline((uint8_t*)"Thermistor #2");
  console_writeline((uint8_t*)itoa(therms[1].temperature, buffer));


}

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
    /* Peripheral clock enable */
    __USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART1)
  {
    /* Peripheral clock disable */
    __USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  }
} 

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
