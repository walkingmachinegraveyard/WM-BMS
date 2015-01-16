/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * Battery
 */
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_device.h"
#include "battery.h"

/**
 * Initialise the battery
 * @param bat The address of the battery to monitor
 */
void battery_init(battery_t *bat) {
  configASSERT(bat!=NULL);
  uint8_t i = 0;
  for(i=0; i<6; i++){
    bat->status = BATTERY_STATUS_UNKNOWN;
    bat->health = BATTERY_HEALTH_UNKNOWN;
    bat->temperature = 0;
    bat->voltage = 0;
  }
}

/**
 * Get the status of the battery
 * @param bat the address of the battery
 * @return the status of the battery
 */
battery_status_t battery_get_status(battery_t *bat) {
    configASSERT(bat!=NULL);
    return bat->status;
}

/**
 * Get the health of the battery
 * @param bat the address of the battery
 * @return the health of the battery
 */
battery_health_t battery_get_health(battery_t *bat) {
    configASSERT(bat!=NULL);
    return bat->health;
}

/**
 * Get the temperature of the battery
 * @param bat the address of the battery
 * @return the temperature of the battery in celicius
 */
uint32_t battery_get_temperature(battery_t *bat) {
    configASSERT(bat!=NULL);
    return bat->temperature;
}

/**
 * Get the voltage of the battery
 * @param bat the address of the battery
 * @return the total voltage of the battery
 */
uint32_t battery_get_voltage(battery_t *bat) {
    configASSERT(bat!=NULL);
    return bat->voltage;
}
