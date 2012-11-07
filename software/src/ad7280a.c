
#include "ad7280a.h"

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

//
// SETTINGS
//
// Conversion input cells / aux adc
#define AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC           0x00
#define AD7280A_CONTROL_CONV_INPUT_6CELL_135ADC         0X01
#define AD7280A_CONTROL_CONV_INPUT_6CELL_0ADC           0x02
#define AD7280A_CONTROL_CONV_INPUT_ADC_SELF_TEST        0x03
// Conversion input read
#define AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC      0X00
#define AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_135ADC    0X01
#define AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_0ADC      0X02
#define AD7280A_CONTROL_CONV_INPUT_READ_DISABLE         0x03
// Conversation start format
#define AD7280A_CONTROL_CONV_START_FORMAT_CNVST         0x00
#define AD7280A_CONTROL_CONV_START_FORMAT_CS            0x01
// Conversion averaging
#define AD7280A_CONTROL_CONV_AVG_SINGLE                 0x00
#define AD7280A_CONTROL_CONV_AVG_BY_2                   0x01
#define AD7280A_CONTROL_CONV_AVG_BY_4                   0x02
#define AD7280A_CONTROL_CONV_AVG_BY_8                   0x03
// Power down format
#define AD7280A_CONTROL_PWRDOWN_FORMAT_PD               0x00
#define AD7280A_CONTROL_PWRDOWN_FORMAT_SOFTWARE         0x01
// Software reset
#define AD7280A_CONTROL_SWRST_OUT_OF_RESET              0x00
#define AD7280A_CONTROL_SWRST_RESET                     0x01
// Acquisition time
#define AD7280A_CONTROL_ACQ_TIME_400NS                  0x00
#define AD7280A_CONTROL_ACQ_TIME_800NS                  0x01
#define AD7280A_CONTROL_ACQ_TIME_1200NS                 0x02
#define AD7280A_CONTROL_ACQ_TIME_1600NS                 0x03
// Thermistor termination resistor
#define AD7280A_CONTROL_THERMISTOR_NOT_IN_USE           0x00
#define AD7280A_CONTROL_THERMISTOR_ENABLE               0x01
// Lock device address
#define AD7280A_CONTROL_LOCK_DEV_ADDR_DISABLE           0x00
#define AD7280A_CONTROL_LOCK_DEV_ADDR_ENABLE            0x01
// Increment device address
#define AD7280A_CONTROL_INC_DEV_ADDR_DISABLE            0x00
#define AD7280A_CONTROL_INC_DEV_ADDR_ENABLE             0x01
// Daisy chain register readback
#define AD7280A_CONTROL_CHAIN_REG_READBACK_DISABLE      0x00
#define AD7280A_CONTROL_CHAIN_REG_READBACK_ENABLE       0x01

#define RX_BUFFER_LENGTH 4
static uint8_t rx_buffer[RX_BUFFER_LENGTH];
static uint8_t tx_buffer[4];

//
// AD7280A private
//

// AD7280A checksum
//
// Computed on Bit[31-10]
static uint32_t crc8(uint32_t buffer) {
    return 0;
}

//
// AD7280A exported
//

/**
 * Open an AD7280A
 */
uint8_t ad7280a_open(ad7280a_t *a, SPIDriver *spid) {
    a->spid = spid;
}

/**
 * Close an AD7280A
 */
uint8_t ad7280a_close(ad7280a_t *a) {
}

/**
 * Read from an AD7280A
 */
uint8_t ad7280a_read(ad7280_t *a, uint32_t dev, uint32_t reg) {
    uint32_t packet = 0;
    uint32_t dev = 0;
    uint32_t reg = 0;
    uint32_t val = 0;
    uint32_t all = 0;

    spiAcquireBus(s->spid);

    spiSelect(s->spid);

    spiReceive(s->spid, rx_buffer, RX_BUFFER_LENGTH);

    spiUnselect(s->spid):

    spiReleaseBus(s->spid);

    // Verify CRC
    if (crc8(rx_buffer) != rx_buffer >> 0x10) {
        return -1;
    }

    // Unpack data
    dev = packet >> 0x27;
    reg = (packet >> 0x21) & 0x0F;
    val = (packet >> 0x13) & 0x10;
    all = (packet >> 0x12) & 0x01;
}

/**
 * Write to an AD7280A
 */
uint8_t ad7280a_write(ad7280_t *a, uint32_t dev, uint32_t reg, uint32_t val, uint32_t all) {
    uint32_t packet = 0;
    uint32_t crc = 0;

    // Pack data
    packet |= dev << 0x27;
    packet |= reg << 0x21;
    packet |= val << 0x13;
    packet |= all << 0x12;

    // Compute CRC
    crc = crc8(packet);
    packet |= crc << 0x02;

    spiAcquireBus(s->spi);

    spiSelect(s->spid);

    spiSend(s->spid, buflen, buffer);

    spiUnselect(s->spid):

    spiReleaseBus(s->spi);
}

