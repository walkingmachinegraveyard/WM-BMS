/* CAN module object for Microchip STM32F103 microcontroller.
 *
 * @file        CO_driver.c
 * @version     SVN: \$Id: co_driver.c 42 2013-07-09 11:14:12Z jani22 $
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

/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_hal.h"
#include "CO_driver.h"
#include "CO_Emergency.h"
#include <string.h>

/* Private macro -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/
static void CO_CANClkSetting (void);
static void CO_CANconfigGPIO (void);
static void CO_CANsendToModule(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer, uint8_t transmit_mailbox);

/*******************************************************************************
   Macro and Constants - CAN module registers
 *******************************************************************************/


/******************************************************************************/
CO_ReturnError_t CO_CANmodule_init(
        CO_CANmodule_t *CANmodule,
        CAN_TypeDef *CANbaseAddress,
        CO_CANrx_t *rxArray,
        uint16_t rxSize,
        CO_CANtx_t *txArray,
        uint16_t txSize,
        uint16_t CANbitRate)
{

    return CO_ERROR_NO;
}

/******************************************************************************/
void CO_CANmodule_disable(CO_CANmodule_t *CANmodule)
{
}

/******************************************************************************/
CO_ReturnError_t CO_CANrxBufferInit(
        CO_CANmodule_t         *CANmodule,
        uint16_t                index,
        uint16_t                ident,
        uint16_t                mask,
        unsigned char           rtr,
        void                   *object,
        void                  (*pFunct)(void *object, const CO_CANrxMsg_t *message))
{
    CO_ReturnError_t ret = CO_ERROR_NO;

    if((CANmodule!=NULL) && (object!=NULL) && (pFunct!=NULL) && (index < CANmodule->rxSize)){
        /* buffer, which will be configured */
        CO_CANrx_t *rxBuffer;
        rxBuffer = CANmodule->rxArray + index;
        /* Configure object variables */
        rxBuffer->object = object;
        rxBuffer->pFunct = pFunct;

        /* CAN identifier and CAN mask, bit aligned with CAN module. Different on different microcontrollers. */
        rxBuffer->ident = ident & 0x07FFU;
        if(rtr){
            rxBuffer->ident |= 0x0800U;
        }
        rxBuffer->mask = (mask & 0x07FFU) | 0x0800U;

        /* Set CAN hardware module filter and mask. */
        if(CANmodule->useCANrxFilters){

        }
    }
    else{
        ret = CO_ERROR_ILLEGAL_ARGUMENT;
    }

    return ret;
}

/******************************************************************************/
CO_CANtx_t *CO_CANtxBufferInit(
        CO_CANmodule_t         *CANmodule,
        uint16_t                index,
        uint16_t                ident,
        int8_t                  rtr,
        uint8_t                 noOfBytes,
        int8_t                  syncFlag)
{
    uint32_t TXF;
    CO_CANtx_t *buffer;

    //safety
    if (!CANmodule || CANmodule->txSize <= index) return 0;

    //get specific buffer
    buffer = &CANmodule->txArray[index];

    //CAN identifier, bit aligned with CAN module registers

    TXF = ident << 21;
    TXF &= 0xFFE00000;
    if (rtr) TXF |= 0x02;

    //write to buffer
    buffer->ident = TXF;
    buffer->DLC = noOfBytes;
    buffer->bufferFull = 0;
    buffer->syncFlag = syncFlag ? 1 : 0;

    return buffer;
}

int8_t getFreeTxBuff(CO_CANmodule_t *CANmodule)
{
    uint8_t txBuff = 0;
    for (txBuff = 0; txBuff <= 3; txBuff++)
        //if (CAN_TransmitStatus(CANmodule->CANbaseAddress, txBuff) == CAN_TxStatus_Ok)
        switch (txBuff)
        {
        case (CAN_TXMAILBOX_0 ):
            if (CANmodule->CANbaseAddress->TSR & CAN_TSR_TME0 )
                return txBuff;
            else
                break;
        case (CAN_TXMAILBOX_1 ):
            if (CANmodule->CANbaseAddress->TSR & CAN_TSR_TME1 )
                return txBuff;
            else
                break;
        case (CAN_TXMAILBOX_2 ):
            if (CANmodule->CANbaseAddress->TSR & CAN_TSR_TME2 )
                return txBuff;
            else
                break;
        }
    return -1;
}

/******************************************************************************/
CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer)
{
    CO_ReturnError_t err = CO_ERROR_NO;
    int8_t txBuff;

    CANmodule->bufferInhibitFlag = buffer->syncFlag;
    CO_CANsendToModule(CANmodule, buffer, txBuff);

    CO_ENABLE_INTERRUPTS();

    return err;
}

/******************************************************************************/
void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule)
{
    uint32_t tpdoDeleted = 0U;

    CO_DISABLE_INTERRUPTS();
    /* Abort message from CAN module, if there is synchronous TPDO.
     * Take special care with this functionality. */
    if(/*messageIsOnCanBuffer && */CANmodule->bufferInhibitFlag){
        /* clear TXREQ */
        CANmodule->bufferInhibitFlag = 0;
        tpdoDeleted = 1U;
    }
    /* delete also pending synchronous TPDOs in TX buffers */
    if(CANmodule->CANtxCount != 0U){
        uint16_t i;
        CO_CANtx_t *buffer = &CANmodule->txArray[0];
        for(i = CANmodule->txSize; i > 0U; i--){
            if(buffer->bufferFull){
                if(buffer->syncFlag){
                    buffer->bufferFull = 0;
                    CANmodule->CANtxCount--;
                    tpdoDeleted = 2U;
                }
            }
            buffer++;
        }
    }
    CO_ENABLE_INTERRUPTS();


    if(tpdoDeleted != 0U){
        CO_errorReport((CO_EM_t*)CANmodule->em, CO_EM_TPDO_OUTSIDE_WINDOW, CO_EMC_COMMUNICATION, tpdoDeleted);
    }
}

/******************************************************************************/
void CO_CANverifyErrors(CO_CANmodule_t *CANmodule)
{
   uint32_t err;
   CO_EM_t* em = (CO_EM_t*)CANmodule->em;

   err = CANmodule->CANbaseAddress->ESR;
   // if(CAN_REG(CANmodule->CANbaseAddress, C_INTF) & 4) err |= 0x80;

   if(CANmodule->errOld != err)
   {
      CANmodule->errOld = err;

      //CAN RX bus overflow
      if(CANmodule->CANbaseAddress->RF0R & 0x08)
      {
         CO_errorReport(em, CO_EM_CAN_RXB_OVERFLOW, CO_EMC_CAN_OVERRUN, err);
         CANmodule->CANbaseAddress->RF0R &=~0x08;//clear bits
      }

      //CAN TX bus off
      if(err & 0x04) CO_errorReport(em, CO_EM_CAN_TX_BUS_OFF, CO_EMC_BUS_OFF_RECOVERED, err);
      else           CO_errorReset(em, CO_EM_CAN_TX_BUS_OFF, err);

      //CAN TX or RX bus passive
      if(err & 0x02)
      {
         if(!CANmodule->firstCANtxMessage) CO_errorReport(em, CO_EM_CAN_TX_BUS_PASSIVE, CO_EMC_CAN_PASSIVE, err);
      }
      else
      {
        // int16_t wasCleared;
        /* wasCleared = */CO_errorReset(em, CO_EM_CAN_TX_BUS_PASSIVE, err);
        /* if(wasCleared == 1) */CO_errorReset(em, CO_EM_CAN_TX_OVERFLOW, err);
      }


      //CAN TX or RX bus warning
      if(err & 0x01)
      {
         CO_errorReport(em, CO_EM_CAN_BUS_WARNING, CO_EMC_NO_ERROR, err);
      }
      else
      {
         CO_errorReset(em, CO_EM_CAN_BUS_WARNING, err);
      }
   }
}

/******************************************************************************/
// Interrupt from Receiver
void CO_CANinterrupt_Rx(CO_CANmodule_t *CANmodule)
{
    // Implemented in CAN_OPEN_RX_THREAD
}

static void CO_CANsendToModule(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer, uint8_t transmit_mailbox)
{

    CanTxMsgTypeDef      CAN1_TxMsg;
    int i;

    /*Test code, VJ using Drivers Start*/
    CAN1_TxMsg.IDE = CAN_ID_STD;
    CAN1_TxMsg.DLC = buffer->DLC;
    for (i = 0; i < 8; i++) CAN1_TxMsg.Data[i] = buffer->data[i];
    CAN1_TxMsg.StdId = ((buffer->ident) >> 21);
    CAN1_TxMsg.RTR = CAN_RTR_DATA;

//    AllocateAndSendMessage(CAN_MSG_ID, CAN_OPEN_PROCESS_TASK_ID, &CAN1_TxMsg, sizeof(CanTxMsgTypeDef));

    (void)transmit_mailbox; // Removing a warning
    (void)CANmodule;        // Removing a warning
}
/******************************************************************************/



