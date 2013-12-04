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

#define CHARGING_COMPLETE   4248  // 4.15V
#define CHARGING_INCOMPLETE 4200
#define MINIMAL_VOLTAGE     2886  // 2.8V
#define MAXIMUM_DELTA       15    // 15 mV
#define MAX_TEMP 55               // Celcius


void cell_update(cell_t cell[], therm_t therm[], ad7280a_t *ad72);

void cell_UART_send_status(cell_t cells[], console_t *console);


#endif /* MONITOR_H_ */
