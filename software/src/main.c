// C Headers
#include <stdint.h>

// ChibiOS
#include "ch.h"
#include "hal.h"
// Local Headers
#include "bms.h"

// Declarations des modules
bms_t bms;
battery_t cells[6];
ad7280a_t ad72;
acs_t acs;

//Cell Monitoring Thread
static WORKING_AREA(cell_monitor_wa, 128);
static void cell_monitor(void *arg) {
  // Remove a warning...
  (void) arg;

  // Thread loop
  while (true) {
  //  battery_check_voltage(&ad72,cells);
    chThdSleepMilliseconds(1337);
  }

}

//// Current Monitoring Thread
//static WORKING_AREA(current_monitor_wa, 128);
//static void current_monitor(void *arg) {
//  // Remove a warning...
//  (void) arg;
//  acs_set_threshold(&acs,25);
//  acs_enable_fault(&acs);
//
//  // Thread loop
//  while (true) {
//    acs.vi_out = acs_read_currsens(&acs);
//    acs.vzcr   = acs_read_vzcr(&acs);
//    chThdSleepMilliseconds(133);
//  }
//}

// Main Thread
int main(int argc, char *argv[]) {
  // Init OS
  halInit();
  chSysInit();

  // Init BMS
  init_bms(&bms);
  init_ad7280a(&ad72);
  battery_init(cells);

  // CellMonitor Thread
  chThdCreateStatic(cell_monitor_wa, sizeof(cell_monitor_wa),
                    NORMALPRIO, cell_monitor, NULL);

//  // CurrentMonitor Thread
//  chThdCreateStatic(current_monitor_wa, sizeof(current_monitor_wa),
//                    NORMALPRIO, current_monitor, NULL);

  // Infinite loop
  while (true) {
    chThdSleepMilliseconds(133);
    battery_check_voltage(&ad72,cells);
    chThdSleepMilliseconds(133);

  }
    return 0;
}
