/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2013 Francois Killeen
 * AD72
 */

// C Standard
#include <stdint.h>
// ChibiOS
#include "hal.h"
#include "ch.h"
// Local
#include "ad72.h"

#define CRC_DATA_WRITE 21
#define CRC_DATA_READ 22

// AD7280A checksum
// Computed on Bits[D31:D11] for a write operation,
// and Bits[D31:D10] for a read operation.
uint8_t do_crc8(ad7280a_packet_t *packet, crc_type_t crc) {
  uint8_t xor_1=0,xor_2=0,xor_3=0,xor_4=0,xor_5=0;
  uint8_t CRC_0=0,CRC_1=0,CRC_2=0,CRC_3=0,CRC_4=0,CRC_5=0,CRC_6=0,CRC_7=0;
  uint8_t num_bits=0,crc_out=0;
  int i;
  uint32_t data_in;
  // Data in is relative to the type of CRC
  switch(crc) {
    case WRITE_REGISTER:
      num_bits = CRC_DATA_WRITE;
      data_in = packet->crc_w_data.data;
      break;
    case READ_CONVERSION:
      num_bits = CRC_DATA_READ;
      data_in = packet->crc_r_data.data;
      break;
    case READ_REGISTER:
      num_bits = CRC_DATA_READ;
      data_in = packet->crc_r_data.data;
      break;
  }
  // Main CRC execution
  for (i=num_bits; i>=0; i--) {
    xor_5 = CRC_4 ^ CRC_7;
    xor_4 = CRC_2 ^ CRC_7;
    xor_3 = CRC_1 ^ CRC_7;
    xor_2 = CRC_0 ^ CRC_7;
    xor_1 = ((data_in & (1 << i)) >> i) ^ CRC_7;
    CRC_7 = CRC_6;
    CRC_6 = CRC_5;
    CRC_5 = xor_5;
    CRC_4 = CRC_3;
    CRC_3 = xor_4;
    CRC_2 = xor_3;
    CRC_1 = xor_2;
    CRC_0 = xor_1;
    }
  // Packing up the CRC
  crc_out = CRC_0;
  crc_out |= (CRC_1 << 1);
  crc_out |= (CRC_2 << 2);
  crc_out |= (CRC_3 << 3);
  crc_out |= (CRC_4 << 4);
  crc_out |= (CRC_5 << 5);
  crc_out |= (CRC_6 << 6);
  crc_out |= (CRC_7 << 7);
  return crc_out;
}

//Verifie que le crc recu est bon
uint8_t crc_conv_check(ad7280a_t *a) {
  crc_type_t crc_type = CRC_DATA_READ;
  ad7280a_packet_t packet;
  packet.packed = a->rxbuf;
  uint8_t received_crc = packet.r_conversion.crc;

  if(received_crc != do_crc8(&packet,crc_type)) {
    return 0;
  }

  return 1;
}

// Low speed SPI configuration (328.125kHz, CPHA=0, CPOL=0, MSb first)
static const SPIConfig ls_spicfg = {
  NULL,
  GPIOB,
  12,
  SPI_CR1_BR_2 | SPI_CR1_BR_1
};

uint32_t spi_exchange(ad7280a_t *a) {
  uint8_t i;
  uint8_t tx_split_buf[4];
  uint8_t rx_split_buf[4];
  a->rxbuf = 0;

  // Division du transfer packet en quatre divisions de 8 bits
  for(i = 0; i<4; ++i) {
      tx_split_buf[i] = a->txbuf >> (24 -(i*8));
  }

  // Spi Exchange
  palClearPad(GPIOC, 4);                             // Set CS to low
  spiAcquireBus(&SPID1);                             /* Acquire ownership of the bus.    */
  spiStart(&SPID1, &ls_spicfg);                      /* Setup transfer parameters.       */
  spiSelect(&SPID1);                                 /* Slave Select assertion.          */
  spiExchange(&SPID1, 4, tx_split_buf, rx_split_buf);/* Atomic transfer operations.      */
  spiUnselect(&SPID1);                               /* Slave Select de-assertion.       */
  spiReleaseBus(&SPID1);                             /* Ownership release.               */
  palSetPad(GPIOC, 4);                               // Set CS to high

  // Short sleep to allow proper data transfer
  chThdSleepMilliseconds(a->delay_ms);

  // Regroupement des quatre packets recu
  for(i = 0; i<4; i++) {
    a->rxbuf |= rx_split_buf[i] << (i*8);
  }
  return a->rxbuf;
}

// Power up the ad7280a by putting the powerdown pin to high
void power_up_ad7280a(ad7280a_t *a) {
  palSetPad(GPIOB,1);
  a->on_off = 1;
}

// Power down the ad7280a by putting the powerdown pin to low
void power_down_ad7280a(ad7280a_t *a) {
  palClearPad(GPIOB,1);
  a->on_off = 0;
}

// Initialize the ad7280
uint8_t init_ad7280a(ad7280a_t *a) {

  a->delay_ms = 100;
  a->txbuf = 0;
  a->rxbuf = 0;
  a->cellbalance = 0;
  power_up_ad7280a(a);

  // set Bit D2 and Bit D0 of the control register to 1
  // and set Bit D1 of the control
  (a->txbuf) = 0x01C2B6E2;
  spi_exchange(a);

  // Write the register address corresponding to the lower byte
  // of the control register to the read register on all parts.
  (a->txbuf) = 0x038716CA;
  spi_exchange(a);

  // Apply a CS low pulse that frames 32 SCLKs
  // (This is used to verify that the ad7280a has received and
  // locked his unique address)
  (a->txbuf) = 0xF800030A;
  spi_exchange(a);
  return 1;
}

// Write to a specific register
void bus_write(ad7280a_t *a, uint8_t reg, uint32_t data, uint32_t write_all) {
  ad7280a_packet_t packet;
  crc_type_t crc = WRITE_REGISTER;
  // Pack the data in the appropriate bitfields
  packet.w_register.bit_pattern = 2;
  packet.w_register.reserved = 0;
  packet.w_register.write_all = write_all;
  packet.w_register.register_address = reg;
  packet.w_register.data = data;
  packet.w_register.device_address = 0;
  packet.w_register.crc = do_crc8(&packet,crc);
  (a->txbuf) = packet.packed;
  spi_exchange(a);
}

// Read cell voltage (choose from 1 to 6)
uint32_t ad7280a_read_cell(uint8_t cell,ad7280a_t *a) {
  ad7280a_packet_t packet;
  packet.packed = 0;

  // 1.On ecrit le numero de registre de la cellule
  bus_write(a, AD7280A_READ, ((cell-1) << 2) , 0);

  // 2. Turn off the read operation
  bus_write(a, AD7280A_CONTROL, AD7280A_CONTROL_CONV_INPUT_6CELL_0ADC
            | AD7280A_CONTROL_CONV_INPUT_READ_DISABLE, 1);

  // 3.Control Register Settings
  bus_write(a, AD7280A_CONTROL, AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
            | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC
            | AD7280A_CONTROL_CONV_START_FORMAT_CNVST
            | AD7280A_CONTROL_CONV_AVG_BY_8, 0);

  // 4.Program the CNVST control register to 0x02 to allow a single pulse
  bus_write(a, AD7280A_CNVST_CONTROL, AD7280A_CNVST_CTRL_SINGLE, 0);

  // 5.Initiate conversions through the falling edge of CNVST.
  palClearPad(GPIOB,0);

  // 5.1 Allow sufficient time for all conversions to be completed
  chThdSleepMilliseconds(a->delay_ms);

  // 5.2 Latch the CNVST back to one
  palSetPad(GPIOB,0);

  // 6 Gate the CNVST, this prevents unintentional conversions
  bus_write(a, AD7280A_CNVST_CONTROL, AD7280A_CNVST_CTRL_GATED, 0);

  //7. On applique 32 SCLKs pour avoir la lecture dans le rxbuf
  (a->txbuf) = AD7280A_RETRANSMIT_SCLKS; // (aucun data/registres)
  spi_exchange(a);
  packet.packed = (a->rxbuf);

  return packet.r_conversion.conversion_data;
}

// Read a Single Configuration Register
uint32_t ad7280a_read_register(uint8_t address,ad7280a_t *a) {

  ad7280a_packet_t packet;
  packet.packed = 0;

  // 1. Turn off the read operation
  bus_write(a, AD7280A_CONTROL, AD7280A_CONTROL_CONV_INPUT_6CELL_0ADC
            | AD7280A_CONTROL_CONV_INPUT_READ_DISABLE, 1);

  // 2. Turn on the read operation on the addressed part
  bus_write(a, AD7280A_CONTROL, AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
            | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC, 0);

  // 3. Write the register address to the read register
  bus_write(a, AD7280A_READ, address << 2 , 0);

  // 4. On applique 32 SCLKs pour avoir la lecture dans le rxbuf
  (a->txbuf) = 0xF800030A; // (aucun data/registres)
  spi_exchange(a);
  packet.packed = (a->rxbuf);

  return packet.r_register.data;
}

// Read thermistor (choose therm from 1 to 2)
uint32_t ad7280a_read_therm(uint8_t therm,ad7280a_t *a) {
  return ad7280a_read_cell((therm+6),a);
}

// Activate cell balance (choose from 1 to 6)
void ad7280a_balance_cell_on(uint8_t cell, ad7280a_t *a) {
  a->cellbalance |= (1<<(cell+1));
  bus_write(a, AD7280A_CELL_BALANCE, a->cellbalance , 0);
}

// Deactivate cell balance (choose from 1 to 6)
void ad7280a_balance_cell_off(uint8_t cell, ad7280a_t *a) {
  a->cellbalance &= ~(1<<(cell+1));
  bus_write(a, AD7280A_CELL_BALANCE, a->cellbalance, 0);
}
