/*
 * monitor.h
 *
 *  Created on: 2013-11-23
 *      Author: francois
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include "cell.h"
#include "ad72.h"
#include "acs.h"
#include "battery.h"
#include "therm.h"

#define MAX_VOLTAGE         4128
#define MINIMAL_VOLTAGE     2800
#define DELTA       		15
#define MAX_DELTA  			100
#define MAXIMUM_CURRENT     5000  // 50 amps
#define MAX_TEMP            55    // Celcius

void monitor_cellbalance(cell_t cells[], ad7280a_t *ad7280a);
void monitor_health_check(battery_t *battery, cell_t cells[], acs_t *acs);
void monitor_voltage(cell_t cells[], ad7280a_t *ad72, battery_t *batt);
void monitor_current(acs_t *acs);

#endif /* MONITOR_H_ */
