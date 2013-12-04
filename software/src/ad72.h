#ifndef AD72_H
#define AD72_H

#include <stdint.h>
// ChibiOS
#include "hal.h"
#include "ch.h"
// Local
#include "bms.h"
#include "cell.h"


//==============================================================================
//
// REGISTERS
//
#define AD7280A_CELL_VOLTAGE_1          0x00    // Read only
#define AD7280A_CELL_VOLTAGE_2          0x01    // Read only
#define AD7280A_CELL_VOLTAGE_3          0x02    // Read only
#define AD7280A_CELL_VOLTAGE_4          0x03    // Read only
#define AD7280A_CELL_VOLTAGE_5          0x04    // Read only
#define AD7280A_CELL_VOLTAGE_6          0x05    // Read only
#define AD7280A_AUX_ADC_1               0x06    // Read only
#define AD7280A_AUX_ADC_2               0x07    // Read only
#define AD7280A_AUX_ADC_3               0x08    // Read only
#define AD7280A_AUX_ADC_4               0x09    // Read only
#define AD7280A_AUX_ADC_5               0x0A    // Read only
#define AD7280A_AUX_ADC_6               0x0B    // Read only
#define AD7280A_SELF_TEST               0x0C    // Read only
#define AD7280A_CONTROL                 0x0D    // Read/write
#define AD7280A_CONTROL2                0x0E    // Read/write
#define AD7280A_CELL_OVERVOLTAGE        0x0F    // Read/write
#define AD7280A_CELL_UNDERVOLTAGE       0X10    // Read/write
#define AD7280A_AUX_ADC_OVERVOLTAGE     0x11    // Read/write
#define AD7280A_AUX_ADC_UNDERVOLTAGE    0x12    // Read/write
#define AD7280A_ALERT                   0x13    // Read/write
#define AD7280A_CELL_BALANCE            0x14    // Read/write
#define AD7280A_CB1_TIMER               0x15    // Read/write
#define AD7280A_CB2_TIMER               0x16    // Read/write
#define AD7280A_CB3_TIMER               0x17    // Read/write
#define AD7280A_CB4_TIMER               0x18    // Read/write
#define AD7280A_CB5_TIMER               0x19    // Read/write
#define AD7280A_CB6_TIMER               0x1A    // Read/write
#define AD7280A_PD_TIMER                0x1B    // Read/write
#define AD7280A_READ                    0x1C    // Read/write
#define AD7280A_CNVST_CONTROL           0x1D    // Read/write
//==============================================================================
//
// SETTINGS
//
//==============================================================================
// CONTROL HIGH BYTE
// Conversion input cells / aux adc
#define AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC           (0x00 << 6)
#define AD7280A_CONTROL_CONV_INPUT_6CELL_135ADC         (0X01 << 6)
#define AD7280A_CONTROL_CONV_INPUT_6CELL_0ADC           (0x02 << 6)
#define AD7280A_CONTROL_CONV_INPUT_ADC_SELF_TEST        (0x03 << 6)
// Conversion input read
#define AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC      (0X00 << 4)
#define AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_135ADC    (0X01 << 4)
#define AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_0ADC      (0X02 << 4)
#define AD7280A_CONTROL_CONV_INPUT_READ_DISABLE         (0x03 << 4)
// Conversation start format
#define AD7280A_CONTROL_CONV_START_FORMAT_CNVST         (0x00 << 3)
#define AD7280A_CONTROL_CONV_START_FORMAT_CS            (0x01 << 3)
// Conversion averaging
#define AD7280A_CONTROL_CONV_AVG_SINGLE                 (0x00 << 1)
#define AD7280A_CONTROL_CONV_AVG_BY_2                   (0x01 << 1)
#define AD7280A_CONTROL_CONV_AVG_BY_4                   (0x02 << 1)
#define AD7280A_CONTROL_CONV_AVG_BY_8                   (0x03 << 1)
// Power down format
#define AD7280A_CONTROL_PWRDOWN_FORMAT_PD               (0x00 << 0)
#define AD7280A_CONTROL_PWRDOWN_FORMAT_SOFTWARE         (0x01 << 0)
// CONTROL LOW-BYTE
// Software reset
#define AD7280A_CONTROL_SWRST_OUT_OF_RESET              (0x00 << 7)
#define AD7280A_CONTROL_SWRST_RESET                     (0x01 << 7)
// Acquisition time
#define AD7280A_CONTROL_ACQ_TIME_400NS                  (0x00 << 5)
#define AD7280A_CONTROL_ACQ_TIME_800NS                  (0x01 << 5)
#define AD7280A_CONTROL_ACQ_TIME_1200NS                 (0x02 << 5)
#define AD7280A_CONTROL_ACQ_TIME_1600NS                 (0x03 << 5)
// Must be set to 1
#define AD7280A_CONTROL_MUST_SET                        (0x01 << 4)
// Thermistor termination resistor
#define AD7280A_CONTROL_THERMISTOR_NOT_IN_USE           (0x00 << 3)
#define AD7280A_CONTROL_THERMISTOR_ENABLE               (0x01 << 3)
// Lock device address
#define AD7280A_CONTROL_LOCK_DEV_ADDR_DISABLE           (0x00 << 2)
#define AD7280A_CONTROL_LOCK_DEV_ADDR_ENABLE            (0x01 << 2)
// Increment device address
#define AD7280A_CONTROL_INC_DEV_ADDR_DISABLE            (0x00 << 1)
#define AD7280A_CONTROL_INC_DEV_ADDR_ENABLE             (0x01 << 1)
// Daisy chain register readback
#define AD7280A_CONTROL_CHAIN_REG_READBACK_DISABLE      (0x00 << 0)
#define AD7280A_CONTROL_CHAIN_REG_READBACK_ENABLE       (0x01 << 0)

//==============================================================================
// CNVST Control Register Settings
//==============================================================================
//CNVST input not gated (default)
#define AD7280A_CNVST_CTRL_NOT_GATED                    0x00
//CNVST input gated
#define AD7280A_CNVST_CTRL_GATED                        0x01
//Allow single CNVST pulse
#define AD7280A_CNVST_CTRL_SINGLE                       0x02
//==============================================================================

#define AD7280A_RETRANSMIT_SCLKS                        0xF800030A

typedef struct ad7280a {
    uint8_t delay_ms;
    uint32_t rxbuf;
    uint32_t txbuf;
    uint32_t cellbalance;
    uint32_t on_off;
} ad7280a_t;

typedef enum crc_type {
    WRITE_REGISTER,
    READ_CONVERSION,
    READ_REGISTER,
}crc_type_t;

//============BitFields=========================================================
struct ad7280a_write_register_bitfield {
    uint32_t bit_pattern: 3; //BitPattern must be 010
    uint32_t crc: 8;
    uint32_t reserved: 1;
    uint32_t write_all: 1;
    uint32_t data: 8;
    uint32_t register_address: 6;
    uint32_t device_address: 5;  //0x00 By Default
};

struct ad7280a_read_conversion_bitfield {
    uint32_t reserved: 2;
    uint32_t crc: 8;
    uint32_t write_ack: 1;
    uint32_t conversion_data: 12;
    uint32_t channel_address: 4;
    uint32_t device_address: 5; //0x00 By Default
};

struct ad7280a_read_register_bitfield {
    uint32_t reserved: 2;
    uint32_t crc: 8;
    uint32_t write_ack: 1;
    uint32_t reserved2: 2;
    uint32_t data: 8;
    uint32_t register_address: 6;
    uint32_t device_address: 5;  //0x00 By Default
};

struct ad7280a_crc_read_data_bitfield {
    uint32_t padding: 10; //Not used during CRC
    uint32_t data: 22;
};

struct ad7280a_crc_write_data_bitfield {
    uint32_t padding: 11; //Not used during CRC
    uint32_t data: 21;
};

//==============================================================================
union ad7280a_packet {
  uint32_t packed;
  struct ad7280a_write_register_bitfield w_register;
  struct ad7280a_read_register_bitfield r_register;
  struct ad7280a_read_conversion_bitfield r_conversion;
  struct ad7280a_crc_read_data_bitfield crc_r_data;
  struct ad7280a_crc_write_data_bitfield crc_w_data;
};
typedef union ad7280a_packet ad7280a_packet_t;

//==============================================================================
// Public Functions:

// Init AD72
uint8_t init_ad7280a(ad7280a_t *ad72);
// Turn on the AD72
void power_up_ad7280a(ad7280a_t *ad72);
// Turn off the AD72
void power_down_ad7280a(ad7280a_t *ad72);
// Read a Single Configuration Register
uint32_t ad7280a_read_register(uint8_t address,ad7280a_t *ad72);
// Read cell voltage (choose from 1 to 6)
uint32_t ad7280a_read_cell(cell_t *cell,ad7280a_t *ad72);
// Read thermistor (choose therm from 1 to 2)
uint32_t ad7280a_read_therm(therm_t *therm, ad7280a_t *ad72);
// Activate cell balance (choose from 1 to 6)
void ad7280a_balance_cell_on(cell_t *cell, ad7280a_t *ad72);
// Deactivate cell balance (choose from 1 to 6)
void ad7280a_balance_cell_off(cell_t *cell, ad7280a_t *ad72);
//==============================================================================

//==============================================================================
// Private functions:

void bus_write(ad7280a_t *a, uint8_t reg, uint32_t data, uint32_t write_all);
// Compile a crc for the specific packet to send
uint8_t do_crc8(ad7280a_packet_t *packet, crc_type_t crc);
// Start a spi exchange between the STM32 and the AD72
uint32_t spi_exchange(ad7280a_t *ad72);
// Test the received crc if it matches the packet
uint8_t crc_conv_check(ad7280a_t *ad72);
//==============================================================================


#endif // AD72_H
