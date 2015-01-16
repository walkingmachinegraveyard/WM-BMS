//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "cmsis_device.h"
#include "BMS_tasks.h"
#include "queue.h"
#include "battery.h"
#include "cell.h"
#include "monitor.h"
#include "adc.h"
#include "can.h"
#include "dac.h"
#include "dma.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

// Sample pragmas to cope with warnings.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/*-----------------------------------------------------------*/
int main(int argc, char* argv[]) {

    // Debug Settings
    #if TRACE_DEBUG
        vTraceInitTraceData();
        uiTraceStart();
    #endif

    #if SWV_DEBUG
        trace_puts("Hello!");
    #endif

    // Initial LED Setup
    MX_GPIO_Init();
    HAL_GPIO_WritePin(GPIOE, (1 << GPIOE_GREEN_LED), GPIO_PIN_SET);

    // Shared Ressources
    static threadShared_t *shared;
    shared = pvPortMalloc(sizeof(threadShared_t));

    // Creating Tasks
    createCanTask(shared);
    createMessageCenterTask(shared);
    createCanOpenTasks(shared);
    createMonitorTask(shared);
    createStatusLEDTask(shared);

    vTaskStartScheduler();

    while (1);
    vPortFree(shared);
}



void vApplicationIdleHook(void) {
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
     to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
     task.  It is essential that code added to this hook function never attempts
     to block in any way (for example, call xQueueReceive() with a block time
     specified, or call vTaskDelay()).  If the application makes use of the
     vTaskDelete() API function (as this demo application does) then it is also
     important that vApplicationIdleHook() is permitted to return to its calling
     function, because it is the responsibility of the idle task to clean up
     memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void) {
    /* vApplicationMallocFailedHook() will only be called if
     configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
     function that will get called if a call to pvPortMalloc() fails.
     pvPortMalloc() is called internally by the kernel whenever a task, queue,
     timer or semaphore is created.  It is also called by various parts of the
     demo application.  If heap_1.c or heap_2.c are used, then the size of the
     heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
     FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
     to query the size of free heap space that remains (although it does not
     provide information on how the remaining heap might be fragmented). */

#if SWV_DEBUG
    trace_puts("MALLOC FAILED!");
#endif

    size_t remainingHeap;
    remainingHeap = xPortGetFreeHeapSize();
    taskDISABLE_INTERRUPTS();
    while(1);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {

    #if SWV_DEBUG
    trace_puts("UhOh! StackOverflow!");
    trace_printf("TaskName: %s\n", pcTaskName);
    #endif

    (void) pxTask;

    /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}

void vApplicationTickHook(void) {

}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
