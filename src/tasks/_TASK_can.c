#include <string.h>
#include <assert.h>
#include "stm32f4xx_hal.h"
#include "cmsis_device.h"
#include "BMS_tasks.h"
#include "can.h"
#include "messages.h"

#define CAN_MSG_ITEM_QUEUE_LENGTH 10
#define CAN_ITEM_QUEUE_LENGTH 10
#define CAN_NB_RETRIES 10
#define CAN_TX_TIMEOUT 10
#define CAN_USE_SOFT_FILTERS
#define CAN_SOFT_FILTER_MIN 0x100
#define CAN_SOFT_FILTER_MAX 0x7FF
#define CAN_RECEIVE_IRQ_MAX_TIMEOUT 100

#if TRACE_DEBUG
    #define TASK_CAN_HEAP (4096/4)
#else
    #define TASK_CAN_HEAP (2048/4)
#endif

//needed for the interruptxQueueHandle can_queue;
/**
 * CanBus Reception Thread
 * @param vCANManagerIN
 * @param vpParams
 */
portTASK_FUNCTION(vCANManagerIN,vpParams) {
    can_queue = xQueueCreate(CAN_ITEM_QUEUE_LENGTH, sizeof(CanRxMsgTypeDef));
    HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
    while (xQueueReceive(can_queue,hcan2.pRxMsg,portMAX_DELAY) == pdTRUE) {
        if ((hcan2.pRxMsg->StdId >= CAN_SOFT_FILTER_MIN)
                && (hcan2.pRxMsg->StdId <= CAN_SOFT_FILTER_MAX)) {
            AllocateAndSendMessage(CAN_MSG_RX_ID, CAN_MSG_ID, hcan2.pRxMsg, sizeof(CanRxMsgTypeDef));
        }
    }
    while (1) {
        //TODO: Error
    }
    (void) vpParams; // Remove a warning for unused variable
}

/**
 * CanBus Transmission Thread
 * @param vCANManagerOUT
 * @param vpParams
 */
portTASK_FUNCTION(vCANManagerOUT,vpParams) {

    MSG * pMSG;
    uint8_t mailbox;
    uint8_t can_retries = 0, can_timeout = 0;
    CanTxMsgTypeDef txMSG;
    hcan2.pTxMsg = &txMSG;

    xQueueHandle msg_queue = xQueueCreate(CAN_MSG_ITEM_QUEUE_LENGTH,
            sizeof(MSG*));

    hcan2.pTxMsg->DLC = 4;
    hcan2.pTxMsg->StdId = 100;
    memcpy(hcan2.pTxMsg->Data, "hell", 4);
    hcan2.pTxMsg->RTR = CAN_RTR_DATA;
    hcan2.pTxMsg->IDE = CAN_ID_STD;

    //Subscribe to the messaging
    SubscribeTo(CAN_MSG_ID, CAN_MSG_ID, msg_queue);
    //messaging to CAN
    while (xQueueReceive(msg_queue,&pMSG,portMAX_DELAY) == pdTRUE) {
        if (pMSG->MsgType == CAN_MSG_ID) {
            //message preparation
            hcan2.pTxMsg->DLC = ((CanTxMsgTypeDef*) pMSG->MsgData)->DLC;
            hcan2.pTxMsg->StdId = ((CanTxMsgTypeDef*) pMSG->MsgData)->StdId;
            memcpy(hcan2.pTxMsg->Data, ((CanTxMsgTypeDef*) pMSG->MsgData)->Data,
                    hcan2.pTxMsg->DLC);
            hcan2.pTxMsg->IDE = CAN_ID_STD;
            hcan2.pTxMsg->RTR = CAN_RTR_DATA;
            //message sending
            can_retries = 0;
            mailbox = HAL_CAN_Transmit(&hcan2, can_timeout);
            while (mailbox == CAN_TXSTATUS_NOMAILBOX) {
                vTaskDelay(5 / portTICK_RATE_MS);
                mailbox = HAL_CAN_Transmit(&hcan2, can_timeout);
                can_retries++;
                if (can_retries >= CAN_NB_RETRIES) {
                    __HAL_CAN_CANCEL_TRANSMIT(&hcan2, CAN_TXMAILBOX_0);
                    __HAL_CAN_CANCEL_TRANSMIT(&hcan2, CAN_TXMAILBOX_1);
                    __HAL_CAN_CANCEL_TRANSMIT(&hcan2, CAN_TXMAILBOX_2);

                    //TODO RE-INIT CAN
//                    HAL_CAN_MspDeInit(&hcan2);
//                    HAL_CAN_MspInit(&hcan2);
                    can_retries = 0;
                }
            }
            while ((__HAL_CAN_TRANSMIT_STATUS(&hcan2, mailbox)
                    != CAN_TXSTATUS_OK) && (can_timeout != CAN_TX_TIMEOUT)) {
                can_timeout++;
                vTaskDelay(1 / portTICK_RATE_MS);
            }
            ConsumeMessage(pMSG);
            //TODO  dispose Message...
        }
    }
    while (1) {
        //TODO: Error Handler
        trace_puts("Can Task Crashed!");
    }
    (void) vpParams; // Remove a warning for unused variable
}

/**
 * @brief  Transmission  complete callback in non blocking mode
 * @param  CanHandle: pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle) {
    __HAL_CAN_DBG_FREEZE(&hcan2, DISABLE);
    if (__HAL_CAN_MSG_PENDING(&hcan2, CAN_FIFO0) != 0) {
        HAL_CAN_Receive(&hcan2, CAN_FIFO0, CAN_RECEIVE_IRQ_MAX_TIMEOUT);
        xQueueSend(can_queue, hcan2.pRxMsg, CAN_RECEIVE_IRQ_MAX_TIMEOUT);
    }
    /* Receive */
    HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
}


/**
 *
 * Initiate the CanBus Periph + Can RX and TX tasks
 *
 * @param shared  threadShare_t Type that allow certain parameters to be passed
 * to the thread when initiated.
 */
void createCanTask(threadShared_t *shared) {

    // Periph Init
    MX_CAN2_Init();

    xTaskCreate(vCANManagerIN, "CAN IN", TASK_CAN_HEAP, (void * ) shared,
            CAN_IN_PRIORITY, NULL);
    xTaskCreate(vCANManagerOUT, "CAN OUT", TASK_CAN_HEAP, (void * ) shared,
            CAN_OUT_PRIORITY, NULL);
}
