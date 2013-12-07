/*
 * monitor.c
 *
 *  Created on: 2013-11-23
 *      Author: francois
 */

#include "monitor.h"

/**
 * Update the health status of the cell
 * @param cell The address of the cell
 */
void monitor_health_check(battery_t *battery, cell_t cells[], acs_t *acs) {

  uint8_t i;
  battery->health = BATTERY_HEALTH_GOOD;

  for(i=0; i<6; ++i) {

    cells[i].health = CELL_HEALTH_GOOD;

    // Overheat Check:
    if(cells[i].temperature > MAX_TEMP) {
      cells[i].health = CELL_HEALTH_OVERHEAT;
      battery->health = BATTERY_HEALTH_OVERHEAT;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
      return;
    }

    // OverVoltage Check:
    if(cells[i].voltage > MAX_VOLTAGE) {
      cells[i].health = CELL_HEALTH_OVER_VOLTAGE;
      battery->health = BATTERY_HEALTH_OVER_VOLTAGE;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
      return;
    }

    // UnderVoltage Check:
    if(cells[i].voltage < MINIMAL_VOLTAGE) {
      cells[i].health = CELL_HEALTH_UNDER_VOLTAGE;
      battery->health = BATTERY_HEALTH_DEAD;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
      return;
    }
  }

  // Overcurrent Check:
  if(acs->current > MAXIMUM_CURRENT) {
    battery->health = BATTERY_HEALTH_OVER_CURRENT;
    palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
    return;
  }
}

/**
 * Update the status of the cells
 * @param cell An array of cells
 * @param therm An array of thermistors
 * @param ad72 The address of the ad7280a
 */
void monitor_voltage(cell_t cells[], ad7280a_t *ad72) {

  uint8_t i;

  for(i=0; i<6; ++i){
    // Check the cell voltage
    cells[i].voltage = ad7280a_read_cell(&cells[i],ad72);
  }
}



void monitor_temperature(cell_t cells[], therm_t therm[], ad7280a_t *ad72) {

  uint8_t i;

  // Check the temperature of the cells
  for(i=0; i<6; ++i) {
    therm_read_temp(therm,ad72);
    if(therm[0].temperature > therm[1].temperature) {
      cells[i].temperature = therm[0].temperature;
    }
    else {
      cells[i].temperature = therm[1].temperature;
    }
  }
}

/**
 * This function when called will make a decision as to which
 * cell should be balanced or not depending on their voltage.
 * @param cells     An array of cells
 * @param ad7280a   The address of the ad7280a structure
 */
void monitor_cellbalance(cell_t cells[], ad7280a_t *ad7280a) {
  uint8_t i;
  uint8_t lowest_cell;
  uint32_t compare = ~0;

  // Find the cell with the lowest voltage
  for(i=0; i<6; ++i) {
    if(cells[i].voltage < compare) {
      compare = cells[i].voltage;
      lowest_cell = cells[i].cell_id;
    }
  }

  // Verify if it's delta to any cells is superior to the limit
  for(i=0; i<6; ++i) {
    if((cells[i].voltage - cells[lowest_cell-1].voltage) > MAXIMUM_DELTA) {
      // Check if there is transition to prevent flooding of the SPI
      if(cells[i].is_balancing == CELL_IS_NOT_BALANCING)
      ad7280a_balance_cell_on(&cells[i], ad7280a);
    } else
      // Check if there is transition to prevent flooding of the SPI
      if(cells[i].is_balancing == CELL_IS_BALANCING)
      ad7280a_balance_cell_off(&cells[i], ad7280a);
  }
}

void monitor_current(acs_t *acs) {
  acs_read_currsens(acs);
}

void monitor_UART_send_status(cell_t cells[], console_t *console, acs_t *acs) {

  char buffer[20];
  console_write(console,"  cell1: ");
  console_write(console, itoa(cells[0].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[0].is_balancing, buffer));
  console_write(console,"  | cell2: ");
  console_write(console, itoa(cells[1].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[1].is_balancing, buffer));
  console_write(console,"  | cell3: ");
  console_write(console, itoa(cells[2].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[2].is_balancing, buffer));
  console_write(console,"  | cell4: ");
  console_write(console, itoa(cells[3].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[3].is_balancing, buffer));
  console_write(console,"  | cell5: ");
  console_write(console, itoa(cells[4].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[4].is_balancing, buffer));
  console_write(console,"  | cell6: ");
  console_write(console, itoa(cells[5].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[5].is_balancing, buffer));
  console_write(console, " Current Sens: ");
  monitor_current(acs);
  itoa(acs->vi_out, buffer);
  console_write(console, buffer);
  console_write(console, "  VZCR: ");
  monitor_current(acs);
  itoa(acs->vzcr, buffer);
  console_write(console, buffer);
  console_write(console, "   Output Current: ");
  if (acs->current_direction == CURRENT_IS_NEGATIVE)
    console_write(console, "-");
  itoa(acs_read_currsens(acs), buffer);
  console_writeline(console, buffer);
}
