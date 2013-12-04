/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * Battery
 */
#include <stdint.h>
#include "battery.h"
#include "ad72.h"

void battery_init(battery_t bat[]) {
  uint8_t i = 0;
  for(i=0; i<6; i++){
    bat[i].status = BATTERY_STATUS_UNKNOWN;
    bat[i].health = BATTERY_HEALTH_UNKNOWN;
    bat[i].power = BATTERY_POWER_UNKNOWN;
    bat[i].temperature = 0;
    bat[i].voltage = 0;
  }
}

battery_status_t battery_get_status(battery_t *bat) {
    return bat->status;
}

battery_health_t battery_get_health(battery_t *bat) {
    return bat->health;
}

battery_power_t battery_get_power(battery_t *bat) {
    return bat->power;
}

uint32_t battery_get_temperature(battery_t *bat) {
    return bat->temperature;
}

uint32_t battery_get_voltage(battery_t *bat) {
    return bat->voltage;
}


/*
 * Callbacks
*/

void battery_on_status_update(battery_t *bat) {
  (void)bat;
}

void battery_on_health_update(battery_t *bat) {
  (void)bat;
}

void battery_on_power_update(battery_t *bat) {
  (void)bat;
}
