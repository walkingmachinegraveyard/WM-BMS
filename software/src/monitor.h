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
#include "therm.h"

#define CHARGING_COMPLETE   4248  // 4.15V
#define CHARGING_INCOMPLETE 4200
#define MINIMAL_VOLTAGE     2886  // 2.8V
#define MAXIMUM_DELTA       50
#define MAX_TEMP 55               // Celcius


void monitor_UART_send_status(cell_t cells[], console_t *console, acs_t *acs);
void monitor_cellbalance(cell_t cells[], ad7280a_t *ad7280a);
void monitor_voltage(cell_t cells[], ad7280a_t *ad72);
void monitor_current(acs_t *curr_sens);



#endif /* MONITOR_H_ */
