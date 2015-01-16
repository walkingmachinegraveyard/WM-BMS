/**
 * B management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2013 Francois Killeen
 *
 * Thermistor
 */
#include "FreeRTOS.h"
#include "task.h"
#include "therm.h"

/**
 * Initialise the thermistors
 * @param therm an array of thermistors
 */
void therm_init(therm_t therm[]) {
    configASSERT(therm != NULL);
    uint8_t i = 0;
    for (i = 0; i < 2; i++) {
        therm[i].present = 0;
        therm[i].temperature = 0;
        therm[i].therm_id = i + 1;
    }
}

/**
 * Read the temperature of the thermistors
 * @param therm an array of thermistors
 * @param ad72 the address of the AD7280A ADC converter
 */
void therm_read_temp(therm_t therm[], ad7280a_t *ad72) {
    configASSERT(therm != NULL);
    uint8_t i = 0;
    for (i = 0; i < 2; i++) {
        therm[i].temperature = ad7280a_read_therm(&therm[i], ad72);
    }
}
