// C Headers
#include <stdint.h>

// ChibiOS
#include "ch.h"
#include "hal.h"
// Local Headers
#include "bms.h"
#include "battery.h"
#include "ad72.h"
#include "acs.h"
#include "therm.h"
#include "cell_mon.h"

// Declarations des modules
  bms_t bms;
  battery_t cells[6];
  therm_t therms[2];
  ad7280a_t ad72;
  acs_t acs;

//Cell Monitoring Thread
//static WORKING_AREA(cell_monitor_wa, 128);
//static void cell_monitor(void *arg) {
//  // Remove a warning...
//  (void) arg;
//
//  // Thread loop
//  while (true) {
//  //  battery_check_voltage(&ad72,cells);
//    chThdSleepMilliseconds(1337);
//    chThdSleepMilliseconds(1337);
//    chThdSleepMilliseconds(1337);
//    chThdSleepMilliseconds(1337);
//    chThdSleepMilliseconds(1337);
// }

//}

// Current Monitoring Thread
//static WORKING_AREA(current_monitor_wa, 128);
//static void current_monitor(void *arg) {
  // Remove a warning...
//  (void) arg;
//  acs_set_threshold(&acs,25);
//  acs_enable_fault(&acs);

  // Thread loop
//  while (true) {
//    acs.vi_out = acs_read_currsens(&acs);
//    acs.vzcr   = acs_read_vzcr(&acs);
//    chThdSleepMilliseconds(133);
//  }
//}

// Main Thread
int main(int argc, char *argv[]) {
  // Remove warnings
  (void) argc;
  (void) argv;
  // Init OS
  halInit();
  chSysInit();



  // Init BMS
  init_bms(&bms);
  init_ad7280a(&ad72);
  battery_init(cells);

  // Test Variables
//  ad7280a_packet_t PT;
//  uint32_t t1;
//  uint32_t t2;
//  uint32_t t3;
//  uint32_t t4;
//  uint32_t t5;
//  uint32_t t6;
//  uint8_t  crc;

//
//  // CellMonitor Thread
//  chThdCreateStatic(cell_monitor_wa, sizeof(cell_monitor_wa),
//                    NORMALPRIO, cell_monitor, NULL);

  // CurrentMonitor Thread
//  chThdCreateStatic(current_monitor_wa, sizeof(current_monitor_wa),
//                    NORMALPRIO, (tfunc_t)current_monitor, NULL);

  // Infinite loop
  while (true) {

  }

    return 0;
}

