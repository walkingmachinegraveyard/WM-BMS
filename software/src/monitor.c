/*
 * monitor.c
 *
 *  Created on: 2013-11-23
 *      Author: Francois Killeen
 */

#include "monitor.h"

/**
 * Update the health status of the cell
 * @param battery The address of a battery type
 * @param cells   An array of cell types
 * @param acs     The address of the current sensor
 */
void monitor_health_check(battery_t *battery, cell_t cells[], acs_t *acs) {

  uint8_t i;
  battery->health = BATTERY_HEALTH_GOOD;

  for (i = 0; i < 6; ++i) {

    cells[i].health = CELL_HEALTH_GOOD;

    // Overheat Check:
    if (cells[i].temperature > MAX_TEMP) {
      cells[i].health = CELL_HEALTH_OVERHEAT;
      battery->health = BATTERY_HEALTH_OVERHEAT;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
    }

    // Max Delta Check:
    if (cells[i].delta == CELL_IS_SUPERIOR_TO_MAX_DELTA) {
      battery->health = BATTERY_IS_SUPERIOR_TO_MAX_DELTA;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
    } else {
      battery->health = BATTERY_IS_INFERIOR_TO_MAX_DELTA;
    }

    // OverVoltage Check:
    if (cells[i].voltage > MAX_VOLTAGE) {
      cells[i].health = CELL_HEALTH_OVER_VOLTAGE;
      battery->health = BATTERY_HEALTH_OVER_VOLTAGE;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
    }

    // UnderVoltage Check:
    if (cells[i].voltage < MINIMAL_VOLTAGE) {
      cells[i].health = CELL_HEALTH_UNDER_VOLTAGE;
      battery->health = BATTERY_HEALTH_DEAD;
      palSetPad(GPIOD, GPIOD_POWERMODULE);      // Emergency shutdown
    }
  }

  // Overcurrent Check:
  if (acs->current > MAXIMUM_CURRENT) {
    battery->health = BATTERY_HEALTH_OVER_CURRENT;
    palSetPad(GPIOD, GPIOD_POWERMODULE);        // Emergency shutdown
  }
}

/**
 * Update the voltage of the cells and their temperature
 * @param cells   An array of cells
 * @param ad72    The address of the AD7280A ADC
 * @param batt    The battery being monitored
 * @param therms  An array of thermistors
 */
void monitor_cells(cell_t cells[], ad7280a_t *ad72, battery_t *batt,
    therm_t therms[]) {

  uint8_t i;
  ad7280a_packet_t packet;
  uint32_t batt_volt = 0;

  // 1.Conversion Read Register Settings
  bus_write(ad72, AD7280A_READ,
      AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
          | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC, WRITE_ALL_ENABLED);

  // 2. Control Register Settings
  bus_write(ad72, AD7280A_CONTROL,
      AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
          | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC
          | AD7280A_CONTROL_CONV_START_FORMAT_CNVST
          | AD7280A_CONTROL_CONV_AVG_BY_8
          | AD7280A_CONTROL_THERMISTOR_NOT_IN_USE
   //       | AD7280A_CONTROL_THERMISTOR_ENABLE
   //       | AD7280A_CONTROL_ACQ_TIME_800NS
   //       | AD7280A_CONTROL_ACQ_TIME_1600NS
   //       | AD7280A_CONTROL_MUST_SET
          , WRITE_ALL_ENABLED);

  // 3.Program the CNVST control register to 0x02 to allow ad72 single pulse
  bus_write(ad72, AD7280A_CNVST_CONTROL, AD7280A_CNVST_CTRL_SINGLE,
  WRITE_ALL_ENABLED);

  // 4.1 Initiate conversions through the falling edge of CNVST.
  palClearPad(GPIOB, GPIOB_CNVST);

  // 4.2 Allow sufficient time for all conversions to be completed
  chThdSleepMilliseconds(ad72->delay_ms);

  // 4.3 Latch the CNVST back to one
  palSetPad(GPIOB, GPIOB_CNVST);

  // 5 Gate the CNVST, this prevents unintentional conversions
  bus_write(ad72, AD7280A_CNVST_CONTROL, AD7280A_CNVST_CTRL_GATED,
  WRITE_ALL_DISABLED);

  // 6. Apply 32 SCLKS to have the data in the receive for cell #1
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  cells[0].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

  // 7. Apply 32 SCLKS to have the data in the receive for cell #2
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  cells[1].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

  // 8. Apply 32 SCLKS to have the data in the receive for cell #3
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  cells[2].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

  // 9. Apply 32 SCLKS to have the data in the receive for cell #4
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  cells[3].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

  // 10. Apply 32 SCLKS to have the data in the receive for cell #5
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  cells[4].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

  // 11. Apply 32 SCLKS to have the data in the receive for cell #6
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  cells[5].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

  // 12. Apply 32 SCLKS to have the data in the receive for therm #1
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  //therms[0].temperature =
  //    packet.r_conversion.conversion_data;

  ad7280a_read_therm(&therms[0], ad72);

    //  ((packet.r_conversion.conversion_data * 0.975) + 1000);

  // 13. Apply 32 SCLKS to have the data in the receive for therm #2
  (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
  spi_exchange(ad72);
  packet.packed = (ad72->rxbuf);
  //therms[1].temperature =
    //  packet.r_conversion.conversion_data;
  ad7280a_read_therm(&therms[1], ad72);



  // 14. Take the highest temperature from the two therms
  for(i = 0; i < 6; ++i){
    if (therms[0].temperature > therms[1].temperature) {
      cells[i].temperature = therms[0].temperature;
      batt->temperature = therms[0].temperature;
    } else {
      cells[i].temperature = therms[1].temperature;
      batt->temperature = therms[1].temperature;
    }
  }

  // 15. Apply 32 SCLKS four times to clear data remaining AUX data
  for (i = 0; i <= 4; ++i) {
    (ad72->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
  }

  // 16. Sum up the voltages to indicate the battery voltage
  for (i = 0; i < 6; ++i) {
    batt_volt += cells[i].voltage;
  }

  batt->voltage = batt_volt;
}

/**
 * This function when called will make a decision as to which
 * cell should be balanced or not depending on their voltage.
 * @param cells     An array of cells
 * @param ad7280a   The address of the ad7280a structure
 */
void monitor_cellbalance(cell_t cells[], ad7280a_t *ad72) {
  uint8_t i;
  uint8_t lowest_cell;
  uint32_t compare = ~0;

  // Verify the CellBalance status of each cells
  for (i = 0; i <= 5; ++i) {
    if ((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
        >> (cells[i].cell_id + 1)) & 1)
      cells[i].is_balancing = CELL_IS_BALANCING;
    if (!((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
        >> (cells[i].cell_id + 1)) & 1))
      cells[i].is_balancing = CELL_IS_NOT_BALANCING;
  }

  // Find the cell with the lowest voltage
  for (i = 0; i < 6; ++i) {
    if (cells[i].voltage < compare) {
      compare = cells[i].voltage;
      lowest_cell = cells[i].cell_id;
    }
  }

  for (i = 0; i < 6; ++i) {
    // Declare an emergency if the delta between cells voltage is superior to the accepted limit
    if ((cells[i].voltage - cells[lowest_cell - 1].voltage) > MAX_DELTA)
      cells[i].delta = CELL_IS_SUPERIOR_TO_MAX_DELTA;
    else
      cells[i].delta = CELL_IS_INFERIOR_TO_MAX_DELTA;

    // Verify if the delta OR the voltage to any cells is superior to the limit
    if (((cells[i].voltage - cells[lowest_cell - 1].voltage) > DELTA)
        || cells[i].voltage > MAX_VOLTAGE) {

      if (cells[i].is_balancing == CELL_IS_NOT_BALANCING) // Check if there is transition
        ad7280a_balance_cell_on(&cells[i], ad72);

    } else if (cells[i].is_balancing == CELL_IS_BALANCING) // Check if there is transition
      ad7280a_balance_cell_off(&cells[i], ad72);
  }
}

/**
 * This function simply calls the acs_read_currsens function
 * (Used for readability purposes)
 * @param acs The address of the ACS current sensor
 */
void monitor_current(acs_t *acs) {
  acs_read_currsens(acs);
}
