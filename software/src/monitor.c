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
void monitor_health_check(cell_t *cell) {
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
void monitor_voltage(cell_t cells[], ad7280a_t *ad72) {

  uint8_t i;

  for(i=0; i<6; ++i){
    // Check the cell voltage
    cells[i].voltage = ad7280a_read_cell(&cells[i],ad72);
  }
  //TODO EMERGENCY

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

  //TODO EMERGENCY
}



/*
 * 4.15 fully charged
15mV delta max
2.8 minimal cell voltage

// 55 max temp
//As soon as cell reaches minimum enable PowerModule_P

cell <2.6 ChargerEn_P = 0
 current < 60

As soon as cell reaches minimum enable PowerModule_P
 */
void monitor_cellbalance(cell_t cells[], ad7280a_t *ad7280a) {
  uint8_t i;
  uint8_t lowest_cell;
  uint32_t compare = 0xFFFFFFFF;

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
      ad7280a_balance_cell_on(&cells[i], ad7280a);
    } else
      ad7280a_balance_cell_off(&cells[i], ad7280a);
  }
}

void monitor_current(acs_t *acs) {

  acs_read_currsens(acs);
  //TODO EMERGENCY

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
