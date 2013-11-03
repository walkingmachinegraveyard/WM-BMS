/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * Cell monitor
 */

#include <stdbool.h>
#include <stdint.h>

#include <ch.h>

#include "ad72.h"
#include "cell_mon.h"
#include "therm.h"

void cell_update(battery_t cell[], therm_t therm[], ad7280a_t *ad72) {
  uint8_t i;

  for(i=0; i<6; i++){
    // Check the cell voltage
    cell[i].voltage = ad7280a_read_cell(&cell[i],ad72);

    // Confirm the presence of the battery
    if(ad7280a_read_cell(&cell[i],ad72))
    cell[i].present = 0;

    // Check the temperature of the cell
    therm_read_temp(therm,ad72);
    if(therm[1].temperature > therm[2].temperature) {
      cell[i].temperature = therm[1].temperature;
    }
    else {
      cell[i].temperature = therm[2].temperature;
    }

    // Check the status of the cell
    //bat[i].status = USE THE CAN BUS

    //===================================
    // Update the health of the battery
    //===================================
    cell[i].health = BATTERY_HEALTH_GOOD;

    // Overheat Check:
    if(cell[i].temperature > MAX_TEMP)
    cell[i].health = BATTERY_HEALTH_OVERHEAT;

    // OverVoltage Check:
    if(cell[i].voltage > CHARGING_COMPLETE)
    cell[i].health = BATTERY_HEALTH_OVER_VOLTAGE;

    // UnderVoltage Check:
    if(cell[i].voltage < MINIMAL_VOLTAGE)
    cell[i].health = BATTERY_HEALTH_UNDER_VOLTAGE;
  }
}

/**
 * Verify the cell during the charge cycle
 * @param cell The address of the cell to verify
 * @param ad7280a The address of the ad7280a
 */
void cell_check_charging(battery_t *cell, ad7280a_t *ad7280a) {

  if(cell->is_balancing == BATTERY_IS_BALANCING) {
    if(cell->voltage <= CHARGING_INCOMPLETE) // If the cell voltage is over 4100mV
      ad7280a_balance_cell_off(cell, ad7280a);
    }
    else {
      return;
    }

  if(cell->is_balancing == BATTERY_IS_NOT_BALANCING) {
    if(cell->voltage >= CHARGING_COMPLETE) { // If the cell voltage is over 4200mV
      ad7280a_balance_cell_on(cell, ad7280a);
    }
    else {
      return;
    }
  }
}
