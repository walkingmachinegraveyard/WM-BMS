/**
 * Cell management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * cell
 */
#include <stdint.h>
#include "cell.h"
#include "ad72.h"

void cell_init(cell_t cell[], ad7280a_t *ad72) {
  uint8_t i;
  for(i=0; i<6; i++){
    cell[i].status = CELL_STATUS_UNKNOWN;
    cell[i].health = CELL_HEALTH_UNKNOWN;
    cell[i].temperature = 0;
    cell[i].voltage = 0;
    cell[i].cell_id= i + 1;
    ad7280a_balance_cell_off(&cell[i], ad72);
  }
}

cell_status_t cell_get_status(cell_t *cell) {
    return cell->status;
}

cell_health_t cell_get_health(cell_t *cell) {
    return cell->health;
}

uint8_t cell_is_balancing(cell_t *cell) {
    return (uint8_t)cell->is_balancing;
}

uint32_t cell_get_temperature(cell_t *cell) {
    return cell->temperature;
}

uint32_t cell_get_voltage(cell_t *cell) {
    return cell->voltage;
}

/*
 * Callbacks
 */

void cell_on_status_update(cell_t *cell) {
  (void) cell; // Remove a warning
}

void cell_on_health_update(cell_t *cell) {
  (void) cell; // Remove a warning
}

void cell_on_power_update(cell_t *cell) {
  (void) cell; // Remove a warning
}
