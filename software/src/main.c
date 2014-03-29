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

struct can_instance {
  CANDriver     *canp;
  uint32_t      led;
};

static const CANConfig cancfg = {
  CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
  CAN_BTR_LBKM | CAN_BTR_SJW(0) | CAN_BTR_TS2(1) |
  CAN_BTR_TS1(8) | CAN_BTR_BRP(6)
};

static const struct can_instance can1 = {&CAND1, 1};
static const struct can_instance can2 = {&CAND2, 2};

static Mutex mtx; /* Mutex declaration */

//  chMtxLock(&mtx);
//  /* Protected code */
//  chMtxUnlock();

/*
 * Receiver thread.
 */
static WORKING_AREA(can_rx1_wa, 256);
static WORKING_AREA(can_rx2_wa, 256);
static msg_t can_rx(void *p) {
  struct can_instance *cip = p;
  EventListener el;
  CANRxFrame rxmsg;

  (void)p;
  chRegSetThreadName("receiver");
  chEvtRegister(&cip->canp->rxfull_event, &el, 0);
  while(!chThdShouldTerminate()) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
      continue;
    while (canReceive(cip->canp, CAN_ANY_MAILBOX,
                      &rxmsg, TIME_IMMEDIATE) == RDY_OK) {
      /* Process message.*/
      palTogglePad(GPIOD, cip->led);
    }
  }
  chEvtUnregister(&CAND1.rxfull_event, &el);
  return 0;
}

/*
 * Transmitter thread.
 */
static WORKING_AREA(can_tx_wa, 256);
static msg_t can_tx(void * p) {
  CANTxFrame txmsg;

  (void)p;
  chRegSetThreadName("transmitter");
  txmsg.IDE = CAN_IDE_EXT;
  txmsg.EID = 0x01234567;
  txmsg.RTR = CAN_RTR_DATA;
  txmsg.DLC = 8;
  txmsg.data32[0] = 0x55AA55AA;
  txmsg.data32[1] = 0x00FF00FF;

  while (!chThdShouldTerminate()) {
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(100));
    canTransmit(&CAND2, CAN_ANY_MAILBOX, &txmsg, MS2ST(100));
    chThdSleepMilliseconds(500);
  }
  return 0;
}

// Monitoring Thread
static WORKING_AREA(monitor_thread_wa, 128);
static void monitor_thread(void *arg) {

  (void) arg;  // remove a warning...
  acs_set_threshold(&acs, 25);
  acs_enable_fault(&acs);

  while (true) {
    monitor_cells(cells, &ad72, &batt, therms);
    monitor_current(&acs);
//  monitor_cellbalance(cells, &ad72);
    monitor_health_check(&batt, cells, &acs);
    consolePrintStatus(cells, &console, &acs, therms, &batt);
    chThdSleepMilliseconds(133);

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
  battery_init(&batt);

  // Monitor Thread Initialization
  chThdCreateStatic(monitor_thread_wa, sizeof(monitor_thread_wa), NORMALPRIO,
      (tfunc_t) monitor_thread, NULL);

  /*
   * Activates the CAN drivers 1 and 2.
   */
  canStart(&CAND1, &cancfg);
  canStart(&CAND2, &cancfg);

  /*
   * Starting the transmitter and receiver threads.
   */
  chThdCreateStatic(can_rx1_wa, sizeof(can_rx1_wa), NORMALPRIO + 7,
                    can_rx, (void *)&can1);
  chThdCreateStatic(can_rx2_wa, sizeof(can_rx2_wa), NORMALPRIO + 7,
                    can_rx, (void *)&can2);
  chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), NORMALPRIO + 7,
                    can_tx, NULL);

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
