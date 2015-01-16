/*
 * BMS_tasks.h
 *
 *  Created on: Sep 13, 2014
 *      Author: François Killeen
 */

#ifndef BMS_TASKS_H_
#define BMS_TASKS_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "messages.h"
#include "timers.h"
#include "Board.h"

#define BMS_IDLE_SLEEP 0

// Task Priorities
#define MESSAGING_PRIORITY          configMAX_PRIORITIES - 1 // Blocking Thread Event Driven
#define CAN_IN_PRIORITY             configMAX_PRIORITIES - 1 // Blocking Thread Event Driven
#define CAN_OUT_PRIORITY            configMAX_PRIORITIES - 1 // Blocking Thread Event Driven
#define CAN_OPEN_OD_READ_PRIORITY   configMAX_PRIORITIES - 2 // Blocking Thread Event Driven
#define CAN_OPEN_OD_WRITE_PRIORITY  configMAX_PRIORITIES - 2 // Blocking Thread Event Driven
#define CAN_OPEN_RX_PRIORITY        configMAX_PRIORITIES - 2 // Blocking Thread Event Driven
#define LED_PRIORITY                configMAX_PRIORITIES - 3 // Blocking Thread Time Driven
#define MONITOR_PRIORITY            configMAX_PRIORITIES - 3 // Blocking Thread Time Driven
#define CAN_OPEN_PROCESS_PRIORITY   configMAX_PRIORITIES - 4 // Non-Blocking Thread (Idle)


// Task Identifications numbers
#define CAN_OPEN_PROCESS_TASK_ID    1
#define CAN_OPEN_OD_READ_TASK_ID    2
#define CAN_OPEN_OD_WRITE_TASK_ID   3
#define CAN_OPEN_RX_TASK_ID         4
#define CAN_IN_TASK_ID              5
#define CAN_OUT_TASK_ID             6
#define MONITOR_TASK_ID             7
#define STATUS_LED_TASK_ID          8
#define MESSAGING_TASK_ID           9


typedef struct threadShared {
    uint8_t sharedByte;
} threadShared_t;


void createMonitorTask(threadShared_t *critical);
void createStatusLEDTask(threadShared_t *critical);
void createCanTask(threadShared_t *critical);
void createCanOpenTasks(threadShared_t *critical);
void createMessageCenterTask(threadShared_t *critical);

#endif /* BMS_TASKS_H_ */
