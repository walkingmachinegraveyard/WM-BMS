/*
 * bms.c
 *
 *  Created on: 2013-09-21
 *      Author: francois
 */

#include "hal.h"
#include "ch.h"
#include "bms.h"
void init_bms(bms_t *bms) {

  bms->progname = "BMS_1.0";

  palSetGroupMode(GPIOA, PAL_PORT_BIT(0), 0, PAL_MODE_INPUT);
  palSetGroupMode(GPIOA, PAL_PORT_BIT(4), 0, PAL_MODE_INPUT);
  // Set the PowerDown Pin for AD7280a
  palSetGroupMode(GPIOB, PAL_PORT_BIT(1), 0, PAL_MODE_OUTPUT_PUSHPULL);

  // Set the CNVST Pin for AD7280a
  palSetGroupMode(GPIOB, PAL_PORT_BIT(0), 0, PAL_MODE_OUTPUT_PUSHPULL);

  palSetGroupMode(GPIOD, PAL_PORT_BIT(6), 0, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(GPIOD,6);              // SelfPower

  palSetGroupMode(GPIOC, PAL_PORT_BIT(4), 0, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(GPIOC, 4);           // Set du ChipSelect a zero (active low)
  palSetPadMode(GPIOA, 5 , PAL_MODE_ALTERNATE(5) |
  PAL_STM32_OSPEED_HIGHEST);       // SCLK
  palSetPadMode(GPIOA, 6 , PAL_MODE_ALTERNATE(5) |
  PAL_STM32_OSPEED_HIGHEST);       // MISO
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5) |
  PAL_STM32_OSPEED_HIGHEST);       // MOSI

  // Setup the LED default state
  palSetPad(GPIOE,GPIOE_GREEN_LED);
  palClearPad(GPIOE,GPIOE_RED_LED);

}
