/*
 * monitor.c
 *
 *  Created on: 2013-11-23
 *      Author: Francois Killeen
 */

#include "monitor.h"

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
			palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
			return;
		}

    // Max Delta Check:
    if (cells[i].delta == CELL_IS_SUPERIOR_TO_MAX_DELTA) {
      battery->health = BATTERY_IS_SUPERIOR_TO_MAX_DELTA;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
    } else {
      battery->health = BATTERY_IS_INFERIOR_TO_MAX_DELTA;
    }

		// OverVoltage Check:
		if (cells[i].voltage > MAX_VOLTAGE) {
			cells[i].health = CELL_HEALTH_OVER_VOLTAGE;
			battery->health = BATTERY_HEALTH_OVER_VOLTAGE;
			palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
			return;
		}

		// UnderVoltage Check:
		if (cells[i].voltage < MINIMAL_VOLTAGE) {
			cells[i].health = CELL_HEALTH_UNDER_VOLTAGE;
			battery->health = BATTERY_HEALTH_DEAD;
			palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
			return;
		}
	}

	// Overcurrent Check:
	if (acs->current > MAXIMUM_CURRENT) {
		battery->health = BATTERY_HEALTH_OVER_CURRENT;
		palSetPad(GPIOD, GPIOD_POWERMODULE);        // Emergency shutdown
		return;
	}
}

/**
 * Update the status of the cells
 * @param cell An array of cells
 * @param therm An array of thermistors
 * @param ad72 The address of the ad7280a
 */
void monitor_voltage(cell_t cells[], ad7280a_t *ad72, battery_t *batt) {

	uint8_t i;
	uint32_t batt_volt = 0;

	for (i = 0; i < 6; ++i) {
		// Check the cell voltage
		cells[i].voltage = ad7280a_read_cell(&cells[i], ad72);
		batt_volt += cells[i].voltage;
	}

	batt->voltage = batt_volt;
}

void monitor_temperature(cell_t cells[], therm_t therm[], ad7280a_t *ad72,
		battery_t *batt) {

	uint8_t i;

	// Check the temperature of the cells
	for (i = 0; i < 6; ++i) {
		therm_read_temp(therm, ad72);

		// Take the highest temperature from the two therms
		if (therm[0].temperature > therm[1].temperature) {
			cells[i].temperature = therm[0].temperature;
			batt->temperature = therm[0].temperature;
		} else {
			cells[i].temperature = therm[1].temperature;
			batt->temperature = therm[1].temperature;
		}
	}
}

/**
 * This function when called will make a decision as to which
 * cell should be balanced or not depending on their voltage.
 * @param cells     An array of cells
 * @param ad7280a   The address of the ad7280a structure
 */
void monitor_cellbalance(cell_t cells[], ad7280a_t *ad72) {
	uint8_t i;
	uint8_t lowest_cell;
	uint32_t compare = ~0;

	// Verify the CellBalance status of each cells
	for (i = 0; i <= 5; ++i) {
		if ((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
				>> (cells[i].cell_id + 1)) & 1)
			cells[i].is_balancing = CELL_IS_BALANCING;
		if (!((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
				>> (cells[i].cell_id + 1)) & 1))
			cells[i].is_balancing = CELL_IS_NOT_BALANCING;
	}

	// Find the cell with the lowest voltage
	for (i = 0; i < 6; ++i) {
		if (cells[i].voltage < compare) {
			compare = cells[i].voltage;
			lowest_cell = cells[i].cell_id;
		}
	}

	for (i = 0; i < 6; ++i) {
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

void monitor_current(acs_t *acs) {
	acs_read_currsens(acs);
}
