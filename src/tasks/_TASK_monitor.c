/*
 * monitor.c
 *
 *  Created on: 2013-11-23
 *      Author: Francois Killeen
 */

#include "monitor.h"
#include "timers.h"
#include "BMS_tasks.h"
#include "messages.h"
#include "CO_driver.h"
#include "ad72.h"
#include "task.h"
#include "battery.h"
#include "cell.h"
#include "messages.h"


#define monitor_current(acs) acs_read_currsens(acs);

#if TRACE_DEBUG
    #define MONITOR_STACK_SIZE (4096/4)
#else
    #define MONITOR_STACK_SIZE (4096/4)
#endif

/**
 * Task to monitor the voltage, temperature and current values from the batteries
 * @param vParameters
 */
extern void vMonitorTask(void *vParameters) {

    // Initial Standby
    vTaskDelay(1000/portTICK_RATE_MS);

    TimerHandle_t monitor_timer;

    // Declaring devices
    battery_t batt;
    cell_t cells[6];
    therm_t therms[2];
    ad7280a_t ad72;
    acs_t acs;

    // CanOpen Variables
    uint16_t cellVoltages[6];
    uint8_t cellHealth[6];
    uint8_t cellStatus[6];

    /* Devices Init */
    init_ad7280a(&ad72);
    cell_init(cells);
    battery_init(&batt);
    acs_init(&acs);
    therm_init(therms);

    // Timer for the ADC convertions
    monitor_timer = xTimerCreate("MonitorTimer", (10), pdTRUE, (void *) 0,
            ADC_BeginConvertions);
    xTimerStart(monitor_timer, 0);

    while (1) {

#if SWV_DEBUG
        trace_puts("Monitor Task");
//        trace_printf("HeapLeft: %u \n", xPortGetFreeHeapSize());
#endif

        monitor_cells(cells, &ad72, &batt, therms);
        monitor_current(&acs);
        monitor_cellbalance(cells, &ad72);
        monitor_health_check(&batt, cells, &acs);

        // CAN OPEN OBJECT DICTIONARY UPDATE

        for(uint8_t i=0 ; i<6 ; ++i){
            cellVoltages[i] = cells[i].voltage;
            cellHealth  [i] = cells[i].health;
            cellStatus  [i] = cells[i].status;
        }
        writeCanOpenOD(CELL_VOLTAGES_ID, MONITOR_TASK_ID, cellVoltages,     6 * sizeof(uint16_t));
        writeCanOpenOD(CELL_HEALTH_ID,   MONITOR_TASK_ID, cellHealth,       6 * sizeof(uint8_t));
        writeCanOpenOD(CELL_STATUS_ID,   MONITOR_TASK_ID, cellStatus,       6 * sizeof(uint8_t));
        writeCanOpenOD(BATT_STATUS_ID,   MONITOR_TASK_ID, &batt.status,         sizeof(uint8_t));
        writeCanOpenOD(BATT_HEALTH_ID,   MONITOR_TASK_ID, &batt.health,         sizeof(uint8_t));
        writeCanOpenOD(BATT_TEMP_ID,     MONITOR_TASK_ID, &batt.temperature,    sizeof(uint16_t));
        writeCanOpenOD(BATT_VOLTAGE_ID,  MONITOR_TASK_ID, &batt.voltage,        sizeof(uint16_t));


#if USART_DEBUG
        consolePrintStatus(cells, &acs, therms, &batt);
#endif

    }

    (void) vParameters;
}

/**
 * Update the health status of the cell
 * @param battery The address of a battery type
 * @param cells   An array of cell types
 * @param acs     The address of the current sensor
 */
void monitor_health_check(battery_t *battery, cell_t cells[], acs_t *acs) {

    uint8_t i;
    battery->health = BATTERY_HEALTH_GOOD;

    for (i = 0; i < 6; ++i) {

        cells[i].health = CELL_HEALTH_GOOD;

        // Overheat Check:
        if (cells[i].temperature > MAX_TEMP) {
            cells[i].health = CELL_HEALTH_OVERHEAT;
            battery->health = BATTERY_HEALTH_OVERHEAT;
            HAL_GPIO_WritePin(GPIOD, (1 << GPIOD_POWERMODULE), GPIO_PIN_SET);
        }

        // Max Delta Check:
        if (cells[i].delta == CELL_IS_SUPERIOR_TO_MAX_DELTA) {
            battery->health = BATTERY_IS_SUPERIOR_TO_MAX_DELTA;
            HAL_GPIO_WritePin(GPIOD, (1 << GPIOD_POWERMODULE), GPIO_PIN_SET);
        } else {
            battery->health = BATTERY_IS_INFERIOR_TO_MAX_DELTA;
        }

        // OverVoltage Check:
        if (cells[i].voltage > MAX_VOLTAGE) {
            cells[i].health = CELL_HEALTH_OVER_VOLTAGE;
            battery->health = BATTERY_HEALTH_OVER_VOLTAGE;
            HAL_GPIO_WritePin(GPIOD, (1 << GPIOD_POWERMODULE), GPIO_PIN_SET);
        }

        // UnderVoltage Check:
        if (cells[i].voltage < MINIMAL_VOLTAGE) {
            cells[i].health = CELL_HEALTH_UNDER_VOLTAGE;
            battery->health = BATTERY_HEALTH_DEAD;
            HAL_GPIO_WritePin(GPIOD, (1 << GPIOD_POWERMODULE), GPIO_PIN_SET);
        }
    }

    // Overcurrent Check:
    if (acs->current > MAXIMUM_CURRENT) {
        battery->health = BATTERY_HEALTH_OVER_CURRENT;
        HAL_GPIO_WritePin(GPIOD, (1 << GPIOD_POWERMODULE), GPIO_PIN_SET);
    }
}

/**
 * Update the voltage of the cells and their temperature
 * @param cells   An array of cells
 * @param ad72    The address of the AD7280A ADC
 * @param batt    The battery being monitored
 * @param therms  An array of thermistors
 */
void monitor_cells(cell_t cells[], ad7280a_t *ad72, battery_t *batt,
        therm_t therms[]) {

    uint8_t i;
    ad7280a_packet_t packet;
    uint32_t batt_volt = 0;

    ad7280a_read_cells(cells, ad72);

    // 12. Apply 32 SCLKS to have the data in the receive for therm #1
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    therms[0].temperature = packet.r_conversion.conversion_data;

    // 13. Apply 32 SCLKS to have the data in the receive for therm #2
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    therms[1].temperature = packet.r_conversion.conversion_data;

//TODO Fix the god damn therms!

//    ad7280a_read_therm(&therms[1], ad72);
//    ad7280a_read_therm(&therms[0], ad72);

    // 14. Take the highest temperature from the two therms
    for (i = 0; i < 6; ++i) {
        if (therms[0].temperature > therms[1].temperature) {
            cells[i].temperature = therms[0].temperature;
            batt->temperature = therms[0].temperature;
        } else {
            cells[i].temperature = therms[1].temperature;
            batt->temperature = therms[1].temperature;
        }
    }

    // 15. Apply 32 SCLKS four times to clear data remaining AUX data
    for (i = 0; i <= 4; ++i) {
        (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
        spi_exchange(ad72);
    }

    // 16. Sum up the voltages to indicate the battery voltage
    for (i = 0; i < 6; ++i) {
        batt_volt += cells[i].voltage;
    }

    batt->voltage = batt_volt;
}

/**
 * This function when called will make a decision as to which
 * cell should be balanced or not depending on their voltage.
 * @param cells     An array of cells
 * @param ad7280a   The address of the ad7280a structure
 */
void monitor_cellbalance(cell_t cells[], ad7280a_t *ad72) {
    uint8_t lowest_cell = INT8_MAX;
    uint32_t compare = ~0;

    // Verify the CellBalance status of each cells
    for (uint8_t i = 0; i <= 5; ++i) {
        if ((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
                >> (cells[i].cell_id + 1)) & 1)
            cells[i].is_balancing = CELL_IS_BALANCING;
        if (!((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
                >> (cells[i].cell_id + 1)) & 1))
            cells[i].is_balancing = CELL_IS_NOT_BALANCING;
    }

    // Find the cell with the lowest voltage
    for (uint8_t i = 0; i < 6; ++i) {
        if (cells[i].voltage < compare) {
            compare = cells[i].voltage;
            lowest_cell = cells[i].cell_id;
        }
    }

    for (uint8_t i = 0; i < 6; ++i) {
        // Declare an emergency if the delta between cells voltage is superior to the accepted limit
        if ((cells[i].voltage - cells[lowest_cell - 1].voltage) > MAX_DELTA)
            cells[i].delta = CELL_IS_SUPERIOR_TO_MAX_DELTA;
        else
            cells[i].delta = CELL_IS_INFERIOR_TO_MAX_DELTA;

        // Verify if the delta OR the voltage to any cells is superior to the limit
        if (((cells[i].voltage - cells[lowest_cell - 1].voltage) > DELTA)
                || cells[i].voltage > MAX_VOLTAGE) {

            if (cells[i].is_balancing == CELL_IS_NOT_BALANCING) // Check if there is transition
                ad7280a_balance_cell_on(&cells[i], ad72);

        } else if (cells[i].is_balancing == CELL_IS_BALANCING) // Check if there is transition
            ad7280a_balance_cell_off(&cells[i], ad72);
    }
}

void createMonitorTask(threadShared_t *critical) {

    /* Periph init */
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_DAC_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    MX_DMA_Init();

    xTaskCreate(vMonitorTask, "Monitor", MONITOR_STACK_SIZE, (void * ) critical,
            MONITOR_PRIORITY, NULL);
}
