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


//Cell Monitoring Thread
//static WORKING_AREA(cell_monitor_wa, 128);
//static void cell_monitor(void *arg) {
//  // Remove a warning...
//  (void) arg;
//
//  // Thread loop
//  while (true) {
//  //  cell_check_voltage(&ad72,cells);
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

  // Declarations des modules
  bms_t bms;
  cell_t cells[6];
  therm_t therms[2];
  ad7280a_t ad72;
  acs_t acs;
  console_t console;

  // Remove warnings
  (void) argc;
  (void) argv;
  // Init OS
  halInit();
  chSysInit();

  // Init BMS
  acs_init(&acs);
  init_bms(&bms);
  console_init(&console);
  init_ad7280a(&ad72);
  cell_init(cells);

  // Test Variables
//  ad7280a_packet_t PT;
  uint32_t t1;
    char buffer[20];
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

//    console_write(&console, "Current Sens: ");
//    acs_read_currsens(&acs);
//    itoa(acs.vi_out, buffer);
//    console_write(&console, buffer);
//
//    console_write(&console, "  VZCR: ");
//    acs_read_currsens(&acs);
//    itoa(acs.vzcr, buffer);
//    console_write(&console, buffer);
//
//    console_write(&console, "   Output Current: ");
//
//    if (acs.current_direction == CURRENT_IS_NEGATIVE)
//      console_write(&console, "-");
//
//    itoa(acs_read_currsens(&acs), buffer);
//    console_writeline(&console, buffer);
//    cell_update(cells,therms,&ad72);
//    cell_UART_send_status(cells, &console);
//    cells[0].cell_id = 0x3;
//    ad7280a_read_cell(&cells[0],&ad72);
//    chThdSleepMilliseconds(133);
//



    monitor_voltage(cells, &ad72);
    monitor_cellbalance(cells, &ad72);
    monitor_UART_send_status(cells, &console, &acs);


    chThdSleepMilliseconds(133);
  }
    return 0;
}

