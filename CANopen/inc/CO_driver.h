/*
 * CAN module object for Microchip STM32F103 microcontroller.
 *
 * @file        CO_driver.h
 * @version     SVN: \$Id: CO_driver.h 42 2013-07-09 11:14:12Z jani22 $
 * @author      Janez Paternoster
 * @author      Ondrej Netik
 * @copyright   2004 - 2013 Janez Paternoster, Ondrej Netik
 *
 * This file is part of CANopenNode, an opensource CANopen Stack.
 * Project home page is <http://canopennode.sourceforge.net>.
 * For more information on CANopen see <http://www.can-cia.org/>.
 *
 * CANopenNode is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CO_DRIVER_H
#define CO_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "stm32f4xx_hal.h"
#include "BMS_tasks.h"
#include "messages.h"

/* Exported define -----------------------------------------------------------*/
#define PACKED_STRUCT               __attribute__((packed))
#define ALIGN_STRUCT_DWORD          __attribute__((aligned(4)))

/* Peripheral addresses */
#define ADDR_CAN1    CAN2
#define TMIDxR_TXRQ  ((uint32_t)0x00000001) /* Transmit mailbox request */

/* Disabling interrupts */
#define CO_DISABLE_INTERRUPTS()  taskENTER_CRITICAL();
#define CO_ENABLE_INTERRUPTS()   taskEXIT_CRITICAL();

#define writeCanOpenOD(objectID, TaskID, data, datasize)  AllocateAndSendMessage(objectID, TaskID, data, datasize)

/* Timeout for initialization */

#define INAK_TIMEOUT        ((uint32_t)0x0000FFFF)
/* Data types */
    typedef float                   float32_t;
    typedef long double             float64_t;
    typedef char                    char_t;
    typedef unsigned char           oChar_t;
    typedef unsigned char           domain_t;

/* Return values */
typedef enum{
    CO_ERROR_NO                 = 0,
    CO_ERROR_ILLEGAL_ARGUMENT   = -1,
    CO_ERROR_OUT_OF_MEMORY      = -2,
    CO_ERROR_TIMEOUT            = -3,
    CO_ERROR_ILLEGAL_BAUDRATE   = -4,
    CO_ERROR_RX_OVERFLOW        = -5,
    CO_ERROR_RX_PDO_OVERFLOW    = -6,
    CO_ERROR_RX_MSG_LENGTH      = -7,
    CO_ERROR_RX_PDO_LENGTH      = -8,
    CO_ERROR_TX_OVERFLOW        = -9,
    CO_ERROR_TX_PDO_WINDOW      = -10,
    CO_ERROR_TX_UNCONFIGURED    = -11,
    CO_ERROR_PARAMETERS         = -12,
    CO_ERROR_DATA_CORRUPT       = -13,
    CO_ERROR_CRC                = -14
}CO_ReturnError_t;

/* CAN receive message structure as aligned in CAN module. */
typedef struct{
    uint32_t StdId;       /*!< Specifies the standard identifier.
                               This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF */

    uint32_t ExtId;       /*!< Specifies the extended identifier.
                               This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF */

    uint32_t IDE;         /*!< Specifies the type of identifier for the message that will be received.
                               This parameter can be a value of @ref CAN_identifier_type */

    uint32_t RTR;         /*!< Specifies the type of frame for the received message.
                               This parameter can be a value of @ref CAN_remote_transmission_request */

    uint32_t DLC;         /*!< Specifies the length of the frame that will be received.
                               This parameter must be a number between Min_Data = 0 and Max_Data = 8 */

    uint32_t Data[8];     /*!< Contains the data to be received.
                               This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF */

    uint32_t FMI;         /*!< Specifies the index of the filter the message stored in the mailbox passes through.
                               This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF */

    uint32_t FIFONumber;  /*!< Specifies the receive FIFO number.
                               This parameter can be CAN_FIFO0 or CAN_FIFO1 */
}CO_CANrxMsg_t;

/* Received message object */
typedef struct{
    uint16_t            ident;
    uint16_t            mask;
    void               *object;
    void              (*pFunct)(void *object, CO_CANrxMsg_t *message);
}CO_CANrx_t;

/* Transmit message object. */
typedef struct{
    uint32_t            ident;
    uint8_t             DLC;
    uint8_t             data[8];
    volatile uint8_t    bufferFull;
    volatile uint8_t    syncFlag;
}CO_CANtx_t;/* ALIGN_STRUCT_DWORD; */

/* CAN module object. */
typedef struct{
    CAN_TypeDef        *CANbaseAddress;         /* STM32F4xx specific */
    CO_CANrx_t         *rxArray;
    uint16_t            rxSize;
    CO_CANtx_t         *txArray;
    uint16_t            txSize;
    volatile uint8_t    useCANrxFilters;
    volatile uint8_t    bufferInhibitFlag;
    volatile uint8_t    firstCANtxMessage;
    volatile uint16_t   CANtxCount;
    uint32_t            errOld;
    void               *em;
}CO_CANmodule_t;

/* Request CAN configuration or normal mode */
//void CO_CANsetConfigurationMode(CAN_TypeDef *CANbaseAddress);
//void CO_CANsetNormalMode(CAN_TypeDef *CANbaseAddress);

/* Initialize CAN module object. */
CO_ReturnError_t CO_CANmodule_init(
        CO_CANmodule_t         *CANmodule,
        CAN_TypeDef            *CANbaseAddress,
        CO_CANrx_t              rxArray[],
        uint16_t                rxSize,
        CO_CANtx_t              txArray[],
        uint16_t                txSize,
        uint16_t                CANbitRate);


/* Switch off CANmodule. */
void CO_CANmodule_disable(CO_CANmodule_t *CANmodule);


/* Read CAN identifier */
//uint16_t CO_CANrxMsg_readIdent(const CO_CANrxMsg_t *rxMsg);


/* Configure CAN message receive buffer. */
CO_ReturnError_t CO_CANrxBufferInit(
        CO_CANmodule_t         *CANmodule,
        uint16_t                index,
        uint16_t                ident,
        uint16_t                mask,
        unsigned char           rtr,
        void                   *object,
        void                  (*pFunct)(void *object, const CO_CANrxMsg_t *message));


/* Configure CAN message transmit buffer. */
CO_CANtx_t *CO_CANtxBufferInit(
        CO_CANmodule_t         *CANmodule,
        uint16_t                index,
        uint16_t                ident,
        int8_t                  rtr,
        uint8_t                 noOfBytes,
        int8_t                  syncFlag);

/* Send CAN message. */
CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer);


/* Clear all synchronous TPDOs from CAN module transmit buffers. */
void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule);


/* Verify all errors of CAN module. */
void CO_CANverifyErrors(CO_CANmodule_t *CANmodule);


/* CAN interrupts receives and transmits CAN messages. */
void CO_CANinterrupt_Rx(CO_CANmodule_t *CANmodule);
void CO_CANinterrupt_Tx(CO_CANmodule_t *CANmodule);
void CO_CANinterrupt_Status(CO_CANmodule_t *CANmodule);

// Read CanOpen Object
void readCanOpenOD(uint32_t objectIndex,void *pData, uint16_t sender, QueueHandle_t xQueue);



#endif    /* CO_DRIVER_H */
