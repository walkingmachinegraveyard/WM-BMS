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
void cell_health_check(cell_t *cell) {
    cell->health = CELL_HEALTH_GOOD;

    // Overheat Check:
    if(cell->temperature > MAX_TEMP)
      cell->health = CELL_HEALTH_OVERHEAT;

    // OverVoltage Check:
    if(cell->voltage > CHARGING_COMPLETE)
      cell->health = CELL_HEALTH_OVER_VOLTAGE;

    // UnderVoltage Check:
    if(cell->voltage < MINIMAL_VOLTAGE)
      cell->health = CELL_HEALTH_UNDER_VOLTAGE;
}

/**
 * Update the status of the cells
 * @param cell An array of cells
 * @param therm An array of thermistors
 * @param ad72 The address of the ad7280a
 */
void cell_update(cell_t cells[], therm_t therm[], ad7280a_t *ad72) {
  uint8_t i;

  for(i=0; i<6; ++i){
    // Check the cell voltage
    cells[i].voltage = ad7280a_read_cell(&cells[i],ad72);
  }
}
//    // Check the temperature of the cell
//    therm_read_temp(therm,ad72);
//    if(therm[0].temperature > therm[1].temperature) {
//      cells[i].temperature = therm[0].temperature;
//    }
//    else {
//      cells[i].temperature = therm[1].temperature;
//    }
//
//    // Update the health of the cell
//    cell_health_check(&cells[i]);
//  }
//}

/**
 * Verify the cell during the charge cycle
 * @param cell The address of the cell to verify
 * @param ad7280a The address of the ad7280a
 */

/*
 * 4.15 fully charged
15mV delta max
2.8 minimal cell voltage

55 max temp

cell <2.6 ChargerEn_P = 0
 current < 60

As soon as cell reaches minimum enable PowerModule_P
 */
void cell_check_charge(cell_t *cell, ad7280a_t *ad7280a) {
  // Find the cell with the lowest voltage







//  if(cell->is_balancing == cell_IS_BALANCING) {
//    if(cell->voltage <= CHARGING_INCOMPLETE) // If the cell voltage is over 4100mV
//      ad7280a_balance_cell_off(cell, ad7280a);
//    }
//    else {
//      return;
//    }
//
//  if(cell->is_balancing == cell_IS_NOT_BALANCING) {
//    if(cell->voltage >= CHARGING_COMPLETE) { // If the cell voltage is over 4200mV
//      ad7280a_balance_cell_on(cell, ad7280a);
//    }
//    else {
//      return;
//    }
//  }
}

void cell_UART_send_status(cell_t cells[], console_t *console) {
  char buffer[20];
  console_write(console,"  cell1 voltage: ");
  console_write(console, itoa(cells[0].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[0].is_balancing, buffer));
  console_write(console,"  | cell2 voltage: ");
  console_write(console, itoa(cells[1].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[1].is_balancing, buffer));
  console_write(console,"  | cell3 voltage: ");
  console_write(console, itoa(cells[2].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[2].is_balancing, buffer));
  console_write(console,"  | cell4 voltage: ");
  console_write(console, itoa(cells[3].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[3].is_balancing, buffer));
  console_write(console,"  | cell5 voltage: ");
  console_write(console, itoa(cells[4].voltage, buffer));
  console_write(console," balancing: ");
  console_write(console, itoa(cells[4].is_balancing, buffer));
  console_write(console,"  | cell6 voltage: ");
  console_write(console, itoa(cells[5].voltage, buffer));
  console_write(console," balancing: ");
  console_writeline(console, itoa(cells[5].is_balancing, buffer));
}

//TODO
void cell_check_discharge(cell_t *cell, ad7280a_t *ad72) {
(void)cell;
(void)ad72;
}
