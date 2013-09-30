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
    bat[i].present = 0;
    bat[i].temperature = 0;
    bat[i].voltage = 0;
    bat[i].cell_id= i + 1;
  }
}

//Check the voltage of each individual cells
uint8_t battery_check_voltage(ad7280a_t *a,battery_t cell[]) {
  uint8_t i;

  for(i=0; i<6; i++) {
    cell[i].voltage = ad7280a_read_cell(i,a);
  }

  return 1;
}

// Checks the temperature from the two thermistors and apply them to all cells
uint8_t battery_check_temperature(ad7280a_t *a,battery_t cell[]) {
  uint8_t i;
  uint32_t average_temparature;

  // Read thermistor (choose therm from 1 to 2)
  average_temparature = (ad7280a_read_therm(1,a) + ad7280a_read_therm(2,a)) / 2;

  for(i=0; i<6; i++) {
    cell[i].temperature = average_temparature;
  }

  return 1;
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

uint32_t battery_is_present(battery_t *bat) {
    return bat->present;
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
}

void battery_on_health_update(battery_t *bat) {
}

void battery_on_power_update(battery_t *bat) {
}
