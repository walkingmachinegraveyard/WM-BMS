#ifndef BOARD_H
#define BOARD_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "diag/Trace.h"

// Debug Configurations
#define USART_DEBUG 0
#define TRACE_DEBUG 0
#define SWV_DEBUG   0

/* Definitions for SPIx clock resources */
#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __SPI1_RELEASE_RESET()

/* Definitions for SPIx Pins */
#define SPIx_SCK_PIN                     (1 << GPIOA_SCK)
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_SCK_AF                      GPIO_AF5_SPI1
#define SPIx_MISO_PIN                    (1 << GPIOA_MISO)
#define SPIx_MISO_GPIO_PORT              GPIOA
#define SPIx_MISO_AF                     GPIO_AF5_SPI1
#define SPIx_MOSI_PIN                     (1 << GPIOA_MOSI)
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_MOSI_AF                     GPIO_AF5_SPI1


/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (100)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* Definition for DAC clock resources */
#define DACx_CHANNEL1_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()
#define DMA1_CLK_ENABLE()               __DMA1_CLK_ENABLE()

#define DACx_FORCE_RESET()              __DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __DAC_RELEASE_RESET()

/* Definition for DACx Channel1 Pin */
#define DACx_CHANNEL1_PIN                GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT          GPIOA

/* Definition for DACx's Channel1 */
#define DACx_CHANNEL1                    DAC_CHANNEL_1

/* Definition for DACx's DMA Channel1 */
#define DACx_DMA_CHANNEL1                DMA_CHANNEL_7
#define DACx_DMA_STREAM1                 DMA1_Stream5

/* Definition for DACx's NVIC */
#define DACx_DMA_IRQn1                   DMA1_Stream5_IRQn
#define DACx_DMA_IRQHandler1             DMA1_Stream5_IRQHandler

#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __ADC1_CLK_ENABLE()
#define DMA2_CLK_ENABLE()               __DMA2_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_0
#define ADCx_CHANNEL_GPIO_PORT          GPIOC

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_8

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA_CHANNEL_0
#define ADCx_DMA_STREAM                 DMA2_Stream0

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler


/* Definitions for the BMS GPIO */
#define GPIOA_BUTTON            0
#define GPIOA_CURRFAULT         1
#define GPIOA_LRCK              4
#define GPIOA_FAULT             3
#define GPIOA_VOC               4
#define GPIOA_SCK               5
#define GPIOA_MISO              6
#define GPIOA_MOSI              7
#define GPIOA_VBUS_FS           9
#define GPIOA_SWDIO             13
#define GPIOA_SWCLK             14

#define GPIOB_CAN_TX            6
#define GPIOB_CAN_RX            5
#define GPIOB_SWO               3
#define GPIOB_SDA               9
#define GPIOB_SCK               10
#define GPIOB_CNVST             0
#define GPIOB_PWRDOWN           1

#define GPIOC_CURR_SENS         0
#define GPIOC_VZCR              1
#define GPIOC_DOUT              3
#define GPIOC_MCLK              7
#define GPIOC_SCLK              10
#define GPIOC_SDIN              12
#define GPIOC_SPI1_CS_PIN		4

#define GPIOD_RESET             4
#define GPIOD_OVER_CURRENT      5
#define GPIOD_SELFPWR			6
#define GPIOD_POWERMODULE       14

#define GPIOE_GREEN_LED         4
#define GPIOE_RED_LED           5
#define GPIOE_ID_1		        0
#define GPIOE_ID_2              1
#define GPIOE_ID_3              2
#define GPIOE_ID_4	            3

#define GPIOH_OSC_IN            0
#define GPIOH_OSC_OUT           1

struct bms {
    char *progname;
};

typedef struct therm {
    uint8_t therm_id;
    uint8_t present;
    uint32_t temperature;
} therm_t;

typedef struct ad7280a {
    uint8_t delay_ms;
    uint32_t TxData;
    uint32_t RxData;
    uint32_t cellbalance;
    uint32_t on_off;
    uint8_t TxBuffer[4];
    uint8_t RxBuffer[4];
    uint8_t transmitting;

} ad7280a_t;

typedef struct bms bms_t;

void BMS_Init();

#endif // BOARD_H
