#include "BMS_tasks.h"
#include "mlist.h"
#include "messages.h"
#include "can.h"

#if TRACE_DEBUG
    #define MSG_CENTER_HEAP (8192/4)
#else
    #define MSG_CENTER_HEAP (4096/4)
#endif

xQueueHandle SysQueue; //Messages that are sent to the system are passed through this queue
//This variable is set in the global scope so it can be accessed by SendMessage/ReceiveMessage

portTASK_FUNCTION(vMessageManager,vpParams) {

    MList *lMessageTubes = NULL; //This list contains the messages values and a list of the subscribed tasks to those messages.
    MSG *pMSG = NULL;                 //

    //SysQueue initialisation
    SysQueue = xQueueCreate(10, sizeof(MSG*));
    if (SysQueue) {
        //Messages tubes initialisation
        InitMList(&lMessageTubes);

        //Message loop
        while (xQueueReceive(SysQueue, &pMSG, portMAX_DELAY)) {
            //Subscription to the message queue.
            if (pMSG->MsgType == 0) {
                MList *lNewTube = NULL;
                int ReachedNodeValue = GoToNodeValue(lMessageTubes,
                        (int) ((void**) pMSG->MsgData)[0]);
                if (ReachedNodeValue == 0
                        || ReachedNodeValue
                                != (int) ((void**) pMSG->MsgData)[0]) {
                    InitMList(&lNewTube);
                    AddPackage(lMessageTubes, (int) ((void**) pMSG->MsgData)[0],
                            lNewTube);
                } else
                    lNewTube = ReturnThisPackage(lMessageTubes);
                AddPackage(lNewTube, pMSG->SenderID,
                        ((void**) pMSG->MsgData)[1]); //TODO : SHOULD NOT ADD SAME PACKET TWICE.
                vPortFree(pMSG->MsgData);
                vPortFree(pMSG);
            } else {
                //Get node with same value
                int ReachedNodeValue = GoToNodeValue(lMessageTubes,
                        pMSG->MsgType);
                if (ReachedNodeValue == pMSG->MsgType) {
                    //Add message to each queues in the list
                    MList *SuscribedList = ReturnThisPackage(lMessageTubes);

                    if (MListLength(SuscribedList) != 0) {
                        //Goes to the first item
                        GoToNodeValue(SuscribedList, 0);

                        //First item must be done manually
                        xQueueHandle *MsgTube = ReturnThisPackage(
                                SuscribedList);
                        if (xQueueSend(MsgTube, (&pMSG), portMAX_DELAY) == pdPASS) {
                            pMSG->NbToken++;
                        }

                        //Send message to the rest of the list
                        if (MListLength(SuscribedList) > 1) {
                            int i;
                            for (i = MListLength(SuscribedList) - 1; i != 0;
                                    i--) {
                                MsgTube = ReturnNextPackage(SuscribedList);
                                if (MsgTube != NULL) {
                                    if (xQueueSend(MsgTube, (&pMSG),
                                            portMAX_DELAY) == pdPASS) {
                                        pMSG->NbToken++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    while (1) {
    }
    (void) vpParams;
}

void SendMessage(int MSGType, int SenderID, void *pData) {
    //Packaging the message
    MSG *NewMsg = pvPortMalloc(sizeof(MSG));
    NewMsg->MsgType = MSGType;
    NewMsg->SenderID = SenderID;
    NewMsg->MsgData = pData;
    NewMsg->NbToken = 0;

    //Adding the message to the queue
    xQueueSend(SysQueue, (&NewMsg), 0);
}

void AllocateAndSendMessage(int MSGType, int SenderID, void *pData, __SIZE_TYPE__ datasize){
    taskENTER_CRITICAL();
    void* copiedData;
    copiedData = pvPortMalloc(datasize);
    memcpy(copiedData, pData, datasize);
    taskEXIT_CRITICAL();
    SendMessage(MSGType, SenderID, copiedData);
}


void SubscribeTo(int MSGType, int SenderID, void *pMsgQueue) {
    //Pack the Data so that it can be received in the Message tubes.
    void **pSusbscription = pvPortMalloc(2 * sizeof(void *));
    pSusbscription[0] = (void *) MSGType;
    pSusbscription[1] = (void *) pMsgQueue;
    //Sends the message to the Message manager... where the subscription takes place
    SendMessage(0, SenderID, pSusbscription);
}

void ManageSubscription(void) {
}

void ReleaseMessage(void *pMsg) {
    taskENTER_CRITICAL();
    if (pMsg != NULL) {
        ((MSG*) pMsg)->NbToken--;
        if (((MSG*) pMsg)->NbToken == 0) {
            if (((MSG*) pMsg)->MsgData != NULL)
                vPortFree(((MSG*) pMsg)->MsgData);
            vPortFree(pMsg);
        }
    }
    taskEXIT_CRITICAL();
}

void createMessageCenterTask(threadShared_t *critical) {
    xTaskCreate(vMessageManager, "Msg Ctr", MSG_CENTER_HEAP, (void * ) critical,
            MESSAGING_PRIORITY, NULL);
}
