/*
 * Cell management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2013 Francois Killeen
 *
 */

// C Headers
#include <stdint.h>

// ChibiOS
#include "ch.h"
#include "hal.h"

// Local Headers
#include "bms.h"
#include "cell.h"
#include "ad72.h"
#include "acs.h"
#include "therm.h"
#include "monitor.h"
#include "console.h"

// Declarations des modules
bms_t bms;
battery_t batt;
cell_t cells[6];
therm_t therms[2];
ad7280a_t ad72;
acs_t acs;
console_t console;

static Mutex mtx; /* Mutex declaration */

//  chMtxLock(&mtx);
//  /* Protected code */
//  chMtxUnlock();

// CanBus  Thread
static WORKING_AREA(canbus_thread_wa, 128);
static void canbus_thread(void *arg) {

  (void) arg;  // remove a warning...

  while (true) {
    chThdSleepMilliseconds(133);
  }
}

// Monitoring Thread
static WORKING_AREA(monitor_thread_wa, 128);
static void monitor_thread(void *arg) {

  (void) arg;  // remove a warning...
  acs_set_threshold(&acs, 25);
  acs_enable_fault(&acs);

  while (true) {
    monitor_voltage(cells, &ad72, &batt);
    monitor_current(&acs);
//  monitor_cellbalance(cells, &ad72);
    monitor_health_check(&batt, cells, &acs);
    consolePrintStatus(cells, &console, &acs, &batt);
  }
}

// Main Thread
int main(int argc, char *argv[]) {

  // Remove warnings
  (void) argc;
  (void) argv;

  // Init OS
  halInit();
  chSysInit();
  chMtxInit(&mtx); /* Mutex initialization */

  // Init BMS
  acs_init(&acs);
  init_bms(&bms);
  console_init(&console);
  init_ad7280a(&ad72);
  cell_init(cells, &ad72);

  // Monitor Thread Initialization
  chThdCreateStatic(monitor_thread_wa, sizeof(monitor_thread_wa), NORMALPRIO,
      (tfunc_t) monitor_thread, NULL);

  // CanBus Thread Initialization
  chThdCreateStatic(canbus_thread_wa, sizeof(canbus_thread_wa), NORMALPRIO,
      (tfunc_t) canbus_thread, NULL);

  // Infinite loop
  while (true) {

    // Check if emergency Dipswitch is activated (TESTING PURPOSES)!!!
    if (!palReadPad(GPIOE, 0) || !palReadPad(GPIOE, 1) || !palReadPad(GPIOE, 2)
        || !palReadPad(GPIOE, 3))
    {
      palClearPad(GPIOD, 6);
    }

    chThdSleepMilliseconds(133);
  }
  return 0;
}
