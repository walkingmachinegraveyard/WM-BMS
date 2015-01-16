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
#include "adc.h"
#include "can.h"
#include "dac.h"
#include "dma.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "acs.h"
#include "cell.h"

#define MAX_VOLTAGE         4128
#define MINIMAL_VOLTAGE     2800
#define DELTA       		15
#define MAX_DELTA  			20     // in mV
#define MAXIMUM_CURRENT     50000  // in mA
#define MAX_TEMP            55     // Celcius

void monitor_cellbalance(cell_t cells[], ad7280a_t *ad7280a);
void monitor_health_check(battery_t *battery, cell_t cells[], acs_t *acs);
void monitor_cells(cell_t cells[], ad7280a_t *ad72, battery_t *batt, therm_t therms[]);
void monitor_current(acs_t *acs);

#endif /* MONITOR_H_ */
