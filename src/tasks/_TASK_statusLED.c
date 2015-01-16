#include "BMS_tasks.h"
#include "CO_driver.h"
#include "battery.h"
#include "monitor.h"
#include "messages.h"
#include "cell.h"
#include "rtc.h"
#include "gpio.h"

#define LED_READ_OD_QUEUE_LENGTH 1

#if TRACE_DEBUG
    #define LED_STACK_SIZE (2048/4)
#else
    #define LED_STACK_SIZE (2048/4)
#endif

// Task to blink LED depending on state the state of the BMS
extern void vStatusLEDTask(void *vParameters) {

    // Initial Standby
    vTaskDelay(1000/portTICK_RATE_MS);

    static uint8_t LedStatus = 1;

    // CanOpen Variables
    cell_health_t cellHealth[6];
    battery_health_t battHealth = BATTERY_HEALTH_UNKNOWN;

    // Converting passed arguments
    threadShared_t* shared;
    shared = (threadShared_t*) vParameters;

    // OD Object reading Queue
    xQueueHandle ReadODQueue = xQueueCreate(LED_READ_OD_QUEUE_LENGTH , sizeof(MSG*));
    SubscribeTo(STATUS_LED_TASK_ID,STATUS_LED_TASK_ID,ReadODQueue);


    while (1) {

#if SWV_DEBUG
        trace_puts("Status LED Task");
#endif

        // Read Battery Health From the Object Dictionary
        readCanOpenOD(BATT_HEALTH_ID,&battHealth,STATUS_LED_TASK_ID,ReadODQueue);
        // Read Cell Health
        readCanOpenOD(CELL_HEALTH_ID,cellHealth,STATUS_LED_TASK_ID,ReadODQueue);

        if(battHealth == BATTERY_HEALTH_GOOD){
            HAL_GPIO_WritePin(GPIOE, (1 << GPIOE_GREEN_LED), LedStatus);
            LedStatus = !LedStatus;
            vTaskDelay(100/portTICK_RATE_MS);
            HAL_GPIO_WritePin(GPIOE, (1 << GPIOE_GREEN_LED), LedStatus);
            LedStatus = !LedStatus;
            vTaskDelay(1000/portTICK_RATE_MS);
        }
        else {
            // TODO: Different flashing modes for different states/health etc
            // Just flash anyways for now....
            HAL_GPIO_WritePin(GPIOE, (1 << GPIOE_GREEN_LED), LedStatus);
            LedStatus = !LedStatus;
            vTaskDelay(100/portTICK_RATE_MS);
            HAL_GPIO_WritePin(GPIOE, (1 << GPIOE_GREEN_LED), LedStatus);
            LedStatus = !LedStatus;
            vTaskDelay(1000/portTICK_RATE_MS);
        }



    }

    // Remove a warning
    (void) shared;
}

void createStatusLEDTask(threadShared_t *critical) {
    xTaskCreate(vStatusLEDTask, "Blinker", configMINIMAL_STACK_SIZE,
            (void * ) critical, LED_PRIORITY, NULL);
}
