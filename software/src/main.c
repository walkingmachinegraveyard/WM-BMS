// C Headers
#include <stdint.h>

// ChibiOS
#include "ch.h"
#include "hal.h"
// Local Headers
#include "bms.h"

// Declarations des modules

//Cell Monitoring Thread
static WORKING_AREA(cell_monitor_wa, 128);
static void cell_monitor(void *arg) {
  // Remove a warning...
  (void) arg;

  // Thread loop
  while (true) {
  //  battery_check_voltage(&ad72,cells);
    chThdSleepMilliseconds(1337);
    chThdSleepMilliseconds(1337);
    chThdSleepMilliseconds(1337);
    chThdSleepMilliseconds(1337);
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
//  // Init OS
  halInit();
  chSysInit();


  bms_t bms;
  battery_t cells[6];
  ad7280a_t ad72;
  acs_t acs;
  // Init BMS
  init_bms(&bms);
  init_ad7280a(&ad72);
  battery_init(cells);

  // Test Variables
  ad7280a_packet_t PT;
  uint32_t t1;
  uint32_t t2;
  uint32_t t3;
  uint32_t t4;
  uint32_t t5;
  uint32_t t6;
  uint8_t  crc;

//
//  // CellMonitor Thread
//  chThdCreateStatic(cell_monitor_wa, sizeof(cell_monitor_wa),
//                    NORMALPRIO, cell_monitor, NULL);

//  // CurrentMonitor Thread
//  chThdCreateStatic(current_monitor_wa, sizeof(current_monitor_wa),
//                    NORMALPRIO, current_monitor, NULL);

  // Infinite loop
  while (true) {




      t1 = ad7280a_read_cell(1, &ad72);
      PT.packed = ad72.rxbuf;
      t2 = ad7280a_read_cell(2, &ad72);
      PT.packed = ad72.rxbuf;
      t3 = ad7280a_read_cell(3, &ad72);
      PT.packed = ad72.rxbuf;
      t4 = ad7280a_read_cell(4, &ad72);
      PT.packed = ad72.rxbuf;
      t5 = ad7280a_read_cell(5, &ad72);
      PT.packed = ad72.rxbuf;
      t6 = ad7280a_read_cell(6, &ad72);
      PT.packed = ad72.rxbuf;










//    // DEBUG TIME
//
//    (ad72.txbuf) = 0x038011CA;
//    spi_exchange(&ad72);
//    (ad72.txbuf) = 0x01A0131A;
//    spi_exchange(&ad72);
//    (ad72.txbuf) = 0x03A0546A;
//    spi_exchange(&ad72);
//
//    // 5.Initiate conversions through the falling edge of CNVST.
//    palClearPad(GPIOB,0);
//
//    // 5.1 Allow sufficient time for all conversions to be completed
//    chThdSleepMilliseconds(ad72.delay_ms);
//
//    // 5.2 Latch the CNVST back to one
//    palSetPad(GPIOB,0);
//
//    (ad72.txbuf) = 0xF800030A;
//    spi_exchange(&ad72);
//    PT.packed = ad72.rxbuf;
//    t1 = PT.r_conversion.conversion_data;
//
//    (ad72.txbuf) = 0xF800030A;
//    spi_exchange(&ad72);
//    PT.packed = ad72.rxbuf;
//    t2 = PT.r_conversion.conversion_data;
//
//    (ad72.txbuf) = 0xF800030A;
//    spi_exchange(&ad72);
//    PT.packed = ad72.rxbuf;
//    t3 = PT.r_conversion.conversion_data;
//
//    (ad72.txbuf) = 0xF800030A;
//    spi_exchange(&ad72);
//    PT.packed = ad72.rxbuf;
//    t4 = PT.r_conversion.conversion_data;
//
//    (ad72.txbuf) = 0xF800030A;
//    spi_exchange(&ad72);
//    PT.packed = ad72.rxbuf;
//    t5 = PT.r_conversion.conversion_data;
//
//    (ad72.txbuf) = 0xF800030A;
//    spi_exchange(&ad72);
//    PT.packed = ad72.rxbuf;
//    t6 = PT.r_conversion.conversion_data;

  }

    return 0;
}

