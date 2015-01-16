#include <string.h>
#include <assert.h>
#include "stm32f4xx_hal.h"
#include "cmsis_device.h"
#include "BMS_tasks.h"
#include "can.h"
#include "CANopen.h"
#include "CO_OD.h"
#include "CO_driver.h"

#if TRACE_DEBUG
    #define TASK_CAN_OPEN_HEAP (2048/4)
#else
    #define TASK_CAN_OPEN_HEAP (2048/4)
#endif


#define NODE_ID_OFFSET 30
#define CANOPEN_RX_MSG_ITEM_QUEUE_LENGTH 10
#define CANOPEN_OD_MSG_ITEM_QUEUE_LENGTH 25

extern const CO_OD_entry_t CO_OD[CO_OD_NoOfElements];  /* Object Dictionary array */

SemaphoreHandle_t xObjectDictionaryMutex = NULL;

/**
 * This function is application specific, it MUST be modified
 * @return The value of this CanOpen Node ID
 */
uint16_t getNodeID(void){
    uint16_t bms_node_ID = 0;
    HAL_GPIO_ReadPin(GPIOE, GPIOE_ID_1) && bms_node_ID == 0 ? bms_node_ID = 1 : bms_node_ID;
    HAL_GPIO_ReadPin(GPIOE, GPIOE_ID_2) && bms_node_ID == 0 ? bms_node_ID = 2 : bms_node_ID;
    HAL_GPIO_ReadPin(GPIOE, GPIOE_ID_3) && bms_node_ID == 0 ? bms_node_ID = 3 : bms_node_ID;
    HAL_GPIO_ReadPin(GPIOE, GPIOE_ID_4) && bms_node_ID == 0 ? bms_node_ID = 4 : bms_node_ID;
    return bms_node_ID + NODE_ID_OFFSET;
}

/**
 * This function is application specific, it MUST be modified
 * @param msg_queue The queue on which to subscribe all the objects
 */
void subscribeToAllObjects(uint16_t taskID, QueueHandle_t msg_queue){
    SubscribeTo(CELL_VOLTAGES_ID,taskID  ,msg_queue);
    SubscribeTo(CELL_HEALTH_ID,  taskID  ,msg_queue);
    SubscribeTo(CELL_STATUS_ID,  taskID  ,msg_queue);
    SubscribeTo(BATT_STATUS_ID,  taskID  ,msg_queue);
    SubscribeTo(BATT_HEALTH_ID,  taskID  ,msg_queue);
    SubscribeTo(BATT_TEMP_ID,    taskID  ,msg_queue);
    SubscribeTo(BATT_VOLTAGE_ID, taskID  ,msg_queue);
}


/**
 * The thread is responsible to the write requests to the Object Dictionary
 * All objects than will be written need to be subscribed to.
 * @param vCanOpen_OD_Write
 * @param vpParams
 */
portTASK_FUNCTION(vCanOpen_OD_Write,vpParams) {

    __SIZE_TYPE__  size        = 0;
    uint16_t objectIndex = NULL;

    // Converting passed arguments
    threadShared_t* shared;
    shared = (threadShared_t*) vpParams;

    // Messaging Init
    MSG * pMSG;
    xQueueHandle msg_queue = xQueueCreate(CANOPEN_OD_MSG_ITEM_QUEUE_LENGTH , sizeof(MSG*));

    // Subscribe to all objects that can be written to the OD
    subscribeToAllObjects(CAN_OPEN_OD_WRITE_TASK_ID, msg_queue);

    while (xQueueReceive(msg_queue,&pMSG,portMAX_DELAY)==pdTRUE){

        if( xSemaphoreTake( xObjectDictionaryMutex, portMAX_DELAY) == pdTRUE ){

            // Update the Object Dictionary with the new value
            objectIndex = CO_OD_find(CO->SDO,pMSG->MsgType);

            // Calculate size of object
            if(CO_OD[objectIndex].maxSubIndex == 0){
                // Object is not an array
                size = CO_OD[objectIndex].length;
            }
            else{
                // Object is an array
                size = CO_OD[objectIndex].length * CO_OD[objectIndex].maxSubIndex;
            }

            CO_memcpy(CO_OD[objectIndex].pData, pMSG->MsgData, size);

            ConsumeMessage(pMSG);
            xSemaphoreGive(xObjectDictionaryMutex);
        }
    }
    (void)shared; // Remove unused variable warning
}

/**
 * This thread is responsible for read requests.
 * When an object from the Object dictionary needs to be read,
 * this thread will send the read value as a message with the object ID as
 * the message type.
 * @param vCanOpen_OD_Read
 * @param vpParams
 */
portTASK_FUNCTION(vCanOpen_OD_Read,vpParams) {

    uint16_t objectIndex,ODIndex;
    __SIZE_TYPE__  size        = 0;

    // Converting passed arguments
    threadShared_t* shared;
    shared = (threadShared_t*) vpParams;

    // Messaging Init
    MSG * pMSG;
    xQueueHandle msg_queue = xQueueCreate(CANOPEN_OD_MSG_ITEM_QUEUE_LENGTH , sizeof(MSG*));

    // Subscribe to the read requests
    SubscribeTo(CAN_OPEN_READ_REQUEST,CAN_OPEN_OD_READ_TASK_ID,msg_queue);

    while (xQueueReceive(msg_queue,&pMSG,portMAX_DELAY)==pdTRUE){

        if( xSemaphoreTake( xObjectDictionaryMutex, portMAX_DELAY) == pdTRUE ){

            // Find the appropriate object
            objectIndex = *((uint32_t*)pMSG->MsgData);
            ODIndex = CO_OD_find(CO->SDO,objectIndex);

            // Calculate size of object
            if(CO_OD[ODIndex].maxSubIndex == 0)
                // Object is not an array
                size = CO_OD[ODIndex].length;
            else
                // Object is an array
                size = CO_OD[ODIndex].length * CO_OD[ODIndex].maxSubIndex;

            // Send a message with the requested object
            AllocateAndSendMessage(pMSG->SenderID, CAN_OPEN_OD_READ_TASK_ID, CO_OD[ODIndex].pData,size);

            ConsumeMessage(pMSG);
            xSemaphoreGive(xObjectDictionaryMutex);
        }
    }
    (void)shared; // Remove unused variable warning
}

/**
 * This thread is responsible to receive messages from the CAN thread
 * @param vCanOpenRXManager
 * @param vpParams
 */
portTASK_FUNCTION(vCanOpenRXManager,vpParams) {

    // Converting passed arguments
    threadShared_t* shared;
    shared = (threadShared_t*) vpParams;

    // Messaging Init
    MSG * pMSG;
    xQueueHandle msg_queue = xQueueCreate(CANOPEN_RX_MSG_ITEM_QUEUE_LENGTH , sizeof(MSG*));

    // Subscribe to the CAN messages
    SubscribeTo(CAN_MSG_RX_ID,CAN_OPEN_RX_TASK_ID,msg_queue);

    while (xQueueReceive(msg_queue,&pMSG,portMAX_DELAY)==pdTRUE){
        if( xSemaphoreTake( xObjectDictionaryMutex, portMAX_DELAY ) == pdTRUE ){


            uint16_t index;
            uint8_t msgMatched = 0;
            CO_CANrx_t *msgBuff = CO->CANmodule[0]->rxArray;

            for (index = 0; index < CO->CANmodule[0]->rxSize; index++)
            {
                uint16_t msg = (((CanRxMsgTypeDef*)pMSG->MsgData)->StdId << 2) | (((CanRxMsgTypeDef*)pMSG->MsgData)->RTR ? 2 : 0);
                if (((msg ^ msgBuff->ident) & msgBuff->mask) == 0)
                {
                    msgMatched = 1;
                    break;
                }
                msgBuff++;
            }

            //Call specific function, which will process the message
            if (msgMatched && msgBuff->pFunct)
            msgBuff->pFunct(msgBuff->object, pMSG->MsgData);

            xSemaphoreGive(xObjectDictionaryMutex);
            ConsumeMessage(pMSG);
        }

    }

    (void)shared; // Remove unused variable warning
}

/**
 * Read from the Object Dictionary
 * @param objectIndex The index of the Object that will be read
 * @param pData A pointer to the data where we want to write the read data
 * @param xQueue The queue used to read that specific object (MUST BE SUBSCRIBED TO THE OBJECT)
 */
void readCanOpenOD(uint32_t objectID,void *pData, uint16_t sender, QueueHandle_t xQueue){

    __SIZE_TYPE__ size = 0;
    MSG * pMSG;

    // If there is no data available in the queue, ask for an update
    if(xQueuePeek(xQueue,&pMSG,0)==pdFALSE){
        AllocateAndSendMessage(CAN_OPEN_READ_REQUEST, sender, &objectID, sizeof(uint32_t));
    }

    // Wait for the read request task to reply with the object
    if(xQueueReceive(xQueue,&pMSG,portMAX_DELAY)==pdTRUE){
            if( xSemaphoreTake( xObjectDictionaryMutex, portMAX_DELAY ) == pdTRUE ){

                // Calculate size of object:
                uint16_t ODIndex = CO_OD_find(CO->SDO,objectID);
                if(CO_OD[ODIndex].maxSubIndex == 0)
                    // Object is not an array
                    size = CO_OD[ODIndex].length;
                else
                    // Object is an array
                    size = CO_OD[ODIndex].length * CO_OD[ODIndex].maxSubIndex;
                xSemaphoreGive(xObjectDictionaryMutex);
            }

            // Copy the data received
            CO_memcpy(pData,pMSG->MsgData,size);

            ConsumeMessage(pMSG);
    }
}


/**
 * This task take care of most of the CanOpen processes.
 * It must run whenever there is spare time with the scheduler.
 * Ideally we should hook it to the IDLE task but either way it is not
 * a blocking thread so it needs to be a very low priority but must still
 * run often. (A few times per second)
 * @param vCanOpenProcessManager
 * @param vpParams
 */
portTASK_FUNCTION(vCanOpenProcessManager,vpParams) {


    // Converting passed arguments
    threadShared_t* shared;
    shared = (threadShared_t*) vpParams;

    xObjectDictionaryMutex = xSemaphoreCreateBinary();
    xSemaphoreGive(xObjectDictionaryMutex);

    uint32_t timer1msCopy, timer1msDiff;
    uint32_t timer1msPrevious = HAL_GetTick();
    CO_ReturnError_t err;

    // *******Application specific!******
    CO_OD_ROM.CANNodeID = getNodeID();
    // **********************************

    /* Verify, if OD structures have proper alignment of initial values */
    if (CO_OD_RAM.FirstWord != CO_OD_RAM.LastWord)
        {
            while(1){}
        }
    if (CO_OD_EEPROM.FirstWord != CO_OD_EEPROM.LastWord)
        {
            while(1){}
        }
    if (CO_OD_ROM.FirstWord != CO_OD_ROM.LastWord)
        {
            while(1){}
        }

    // Initialize CANopen
    err = CO_init();
    if(err){
       while(1);
       // TODO: Handle Init Error
    }

    // Init is done, we can now start the other CAN OPEN tasks
    xTaskCreate(vCanOpenRXManager, "CAN_OPEN_RX", TASK_CAN_OPEN_HEAP, (void * ) shared,
               CAN_OPEN_RX_PRIORITY, NULL);

   xTaskCreate(vCanOpen_OD_Read, "CAN_OPEN_READ_OBJECT_DICTIONARY", TASK_CAN_OPEN_HEAP, (void * ) shared,
           CAN_OPEN_OD_READ_PRIORITY, NULL);

   xTaskCreate(vCanOpen_OD_Write, "CAN_OPEN_WRITE_OBJECT_DICTIONARY", TASK_CAN_OPEN_HEAP, (void * ) shared,
           CAN_OPEN_OD_WRITE_PRIORITY, NULL);

    while (1) {

        if( xSemaphoreTake( xObjectDictionaryMutex, portMAX_DELAY ) == pdTRUE ){

            timer1msCopy = HAL_GetTick();
            timer1msDiff = timer1msCopy - timer1msPrevious;
            timer1msPrevious = timer1msCopy;

            CO_process(CO, timer1msDiff);
            CO_process_RPDO(CO);
            CO_process_TPDO(CO);

            // Need to sleep at least 1ms
            vTaskDelay(1/portTICK_RATE_MS);

            xSemaphoreGive(xObjectDictionaryMutex);
        }

    }
    (void)shared; // Remove unused variable warning
}


/**
 * Create the required threads for the CanOpen to run properly
 * @param shared
 */
void createCanOpenTasks(threadShared_t *shared) {
    xTaskCreate(vCanOpenProcessManager, "CAN_OPEN_PROCESS", TASK_CAN_OPEN_HEAP, (void * ) shared,
            CAN_OPEN_PROCESS_PRIORITY, NULL);
}
