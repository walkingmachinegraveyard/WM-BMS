/*
 * messages.h
 *
 *  Created on: Sep 15, 2014
 *      Author: Frank
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <FreeRTOS.h>
#include <semphr.h>
#include <string.h>

#define ConsumeMessage( pMsg ) ( ReleaseMessage( pMsg ) )



struct xMSG {
    int MsgType;    //The type of message sent
    int SenderID;   //The ID of the task that sent the message
    void *MsgData;  //A pointer to the data
    uint8_t NbToken; //Number of tasks this message was sent to. (Used to delete the message data)
    xSemaphoreHandle SemHandle;
};
typedef struct xMSG MSG;

// Messages
#define CAN_MSG_ID                      200
#define CAN_MSG_RX_ID                   300
#define CAN_OPEN_READ_REQUEST           400
// CAN OPEN OD Messages (Must be the proper Object Index)
#define CELL_VOLTAGES_ID                0x2112
#define CELL_HEALTH_ID                  0x2120
#define CELL_STATUS_ID                  0x2121
#define BATT_STATUS_ID                  0x2111
#define BATT_HEALTH_ID                  0x2110
#define BATT_TEMP_ID                    0x2108
#define BATT_VOLTAGE_ID                 0x2109


void AllocateAndSendMessage(int MSGType, int SenderID, void *pData, __SIZE_TYPE__ datasize);
void SendMessage(int MSGType, int SenderID, void *pData);
void SubscribeTo(int MSGType, int SenderID, void *pMsgQueue);
void ReleaseMessage(void *pMsg);

#endif /* MESSAGES_H_ */
