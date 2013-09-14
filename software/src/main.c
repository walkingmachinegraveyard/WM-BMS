// C Headers
#include <stdint.h>

// ChibiOS
#include "ch.h"
#include "hal.h"
// Local Headers
#include "bms.h"

/*
 * Low speed SPI configuration (328.125kHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig ls_spicfg = {
  NULL,
  GPIOA,
  12,
  SPI_CR1_BR_2 | SPI_CR1_BR_1
};

// Thread du SPI
static WORKING_AREA(spi_thread_1_wa, 256);
static msg_t spi_thread_1(void *p) {

  (void)p;
  chRegSetThreadName("SPI");
  // Declarations des modules:

  // Init batteries
  battery_t cells[6];
  int i = 0;
  for(i=0; i<6; i++){
    battery_init(&cells[i],i);
  }

  ad7280a_t ad72;
  acs_t acs;
  // Init systems
  acs_init(&acs);
  init_ad7280a(&ad72);

  while (TRUE) {
    ad7280a_read_cell(2,&ad72);
  }
  return 0;
}

int main(int argc, char *argv[]) {


    // Init OS
    halInit();
    chSysInit();

    // Init BMS
    palSetGroupMode(GPIOD, PAL_PORT_BIT(6), 0, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOD,6);            // SelfPower



    // Init du pinout pour le SPI
      palSetPadMode(GPIOA, 5 , PAL_MODE_ALTERNATE(5) |
        PAL_STM32_OSPEED_HIGHEST);       // SCLK
      palSetPadMode(GPIOA, 6 , PAL_MODE_ALTERNATE(5) |
        PAL_STM32_OSPEED_HIGHEST);       // MISO
      palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5) |
        PAL_STM32_OSPEED_HIGHEST);       // MOSI

      palSetGroupMode(GPIOC, PAL_PORT_BIT(4), 0, PAL_MODE_OUTPUT_PUSHPULL);
      // CS
      //Set du ChipSelect a zero (active low)
      palClearPad(GPIOC, 4);             // CS
      // Thread du SPI
      chThdCreateStatic(spi_thread_1_wa, sizeof(spi_thread_1_wa),
                        NORMALPRIO + 1, spi_thread_1, NULL);



    // Infinite loop
    while (true) {
        chThdSleepMilliseconds(1337);
    }

    return 0;
}


