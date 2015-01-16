#include "cmsis_device.h"
#include "ad72.h"
#include "FreeRTOS.h"
#include "task.h"

#define CRC_DATA_WRITE 21
#define CRC_DATA_READ 22
#define BUFFERSIZE                       (COUNTOF(ad72->TxDatafer) - 1)
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/**
 * Initialize the ad7280
 * @param ad72 The address of the ad72 device
 * @return
 */
uint8_t init_ad7280a(ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);

    ad72->delay_ms = 50;

    ad72->transmitting = 0;

    ad72->RxData = 0;
    ad72->TxData = 0;

    power_up_ad7280a(ad72);

    // Software reset
    (ad72->TxData) = 0x01D2B412;
    spi_exchange(ad72);
    (ad72->TxData) = 0x01C2B6E2;
    spi_exchange(ad72);

    return 1;
}

/**
 * Write to a specific register
 * @param ad72 The address of the ad72 device
 * @param reg The register address you want to write in
 * @param data The data you want to write to the register
 * @param write_all Write All bit 1 : 0
 */
void bus_write(ad7280a_t *ad72, uint8_t reg, uint32_t data, uint32_t write_all) {
    configASSERT(ad72!=NULL);
    ad7280a_packet_t packet;
    crc_type_t crc = WRITE_REGISTER;
    // Pack the data in the appropriate bitfields
    packet.w_register.bit_pattern = 2;
    packet.w_register.reserved = 0;
    packet.w_register.write_all = write_all;
    packet.w_register.register_address = reg;
    packet.w_register.data = data;
    packet.w_register.device_address = 0;
    packet.w_register.crc = do_crc8(&packet, crc);
    (ad72->TxData) = packet.packed;
    spi_exchange(ad72);
}

/**
 * Read cell voltage
 * LSB = 4/4095 V + 1000mV
 * @param cell The address of the Cell you want to measure
 * @param ad72 The address of the ad7280a device
 * @return Returns the value of the cell in millivolts
 */
void ad7280a_read_cells(cell_t cells[], ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);
    ad7280a_packet_t packet;
    packet.packed = 0;

    // 1.Conversion Read Register Settings
    bus_write(ad72, AD7280A_READ,
            AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
                    | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC,
            WRITE_ALL_ENABLED);

    // 2. Control Register Settings
    bus_write(ad72, AD7280A_CONTROL,
            AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
                    | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC
                    | AD7280A_CONTROL_CONV_START_FORMAT_CNVST
                    | AD7280A_CONTROL_CONV_AVG_BY_8
                    | AD7280A_CONTROL_MUST_SET, WRITE_ALL_ENABLED);

    // 3.Program the CNVST control register to 0x02 to allow ad72 single pulse
    bus_write(ad72, AD7280A_CNVST_CONTROL, AD7280A_CNVST_CTRL_SINGLE,
    WRITE_ALL_ENABLED);

    // 4.1 Initiate conversions through the falling edge of CNVST.
    HAL_GPIO_WritePin(GPIOB, (1 << GPIOB_CNVST), GPIO_PIN_RESET);

    // 4.2 Allow sufficient time for all conversions to be completed
    vTaskDelay(ad72->delay_ms);

    // 4.3 Latch the CNVST back to one
    HAL_GPIO_WritePin(GPIOB, (1 << GPIOB_CNVST), GPIO_PIN_SET);

    // 6. Apply 32 SCLKS to have the data in the receive for cell #1
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    cells[0].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

    // 7. Apply 32 SCLKS to have the data in the receive for cell #2
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    cells[1].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

    // 8. Apply 32 SCLKS to have the data in the receive for cell #3
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    cells[2].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

    // 9. Apply 32 SCLKS to have the data in the receive for cell #4
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    cells[3].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

    // 10. Apply 32 SCLKS to have the data in the receive for cell #5
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    cells[4].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);

    // 11. Apply 32 SCLKS to have the data in the receive for cell #6
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    cells[5].voltage = ((packet.r_conversion.conversion_data * 0.975) + 1000);
}

/**
 * Power up the ad7280a by putting the powerdown pin to high
 * @param ad72 The address of the ad72 device
 */
void power_up_ad7280a(ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);
    HAL_GPIO_WritePin(GPIOB, (1 << GPIOB_PWRDOWN), GPIO_PIN_SET);
    ad72->on_off = ENABLE;
}

/**
 * Power down the ad7280a by putting the powerdown pin to low
 * @param ad72 The address of the ad72 device
 */
void power_down_ad7280a(ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);
    HAL_GPIO_WritePin(GPIOB, (1 << GPIOB_PWRDOWN), GPIO_PIN_RESET);
    ad72->on_off = DISABLE;
}

/**
 * AD7280A checksum Computed on Bits[D31:D11] for ad72 write operation,
 * and Bits[D31:D10] for ad72 read operation.
 * @param packet The adress of the packet in which the data is located
 * @param crc The type of CRC (WRITE_REGISTER,READ_REGISTER,READ_CONVERSION)
 * @return 8 Bit CRC
 */
uint8_t do_crc8(ad7280a_packet_t *packet, crc_type_t crc) {
    configASSERT(packet!=NULL);
    uint8_t xor_1 = 0, xor_2 = 0, xor_3 = 0, xor_4 = 0, xor_5 = 0;
    uint8_t CRC_0 = 0, CRC_1 = 0, CRC_2 = 0, CRC_3 = 0, CRC_4 = 0, CRC_5 = 0,
            CRC_6 = 0, CRC_7 = 0;
    uint8_t num_bits = 0, crc_out = 0;
    int i;
    uint32_t data_in = 0;
    // Data in is relative to the type of CRC
    switch (crc) {
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
    for (i = num_bits; i >= 0; i--) {
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

/**
 * Verifies that the received CRC is correct
 * @param ad72 Addres pointing to the AD7280A device
 * @return 1 if CRC is good 0 if not
 */
uint8_t crc_conv_check(ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);
    crc_type_t crc_type = READ_CONVERSION;
    ad7280a_packet_t packet;
    packet.packed = ad72->RxData;
    uint8_t received_crc = packet.r_conversion.crc;

    if (received_crc != do_crc8(&packet, crc_type))
        return 0;
    else
        return 1;
}

/**
 * Read ad72 Single Configuration Register
 * @param address Adress of the register
 * @param ad72 The address of the ad72 device
 * @return Return the value of the register read
 */
uint32_t ad7280a_read_register(uint8_t address, ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);
    ad7280a_packet_t packet;
    packet.packed = 0;

    // 1. Turn off the read operation
    bus_write(ad72, AD7280A_CONTROL,
            AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
                    | AD7280A_CONTROL_CONV_INPUT_READ_DISABLE,
            WRITE_ALL_ENABLED);

    // 2. Turn on the read operation on the addressed part
    bus_write(ad72, AD7280A_CONTROL,
            AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
                    | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC,
            WRITE_ALL_DISABLED);

    // 3. Write the register address to the read register
    bus_write(ad72, AD7280A_READ, address << 2, WRITE_ALL_DISABLED);

    // 4. On applique 32 SCLKs pour avoir la lecture dans le rxbuf
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (aucun data/registres)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);
    return packet.r_register.data;
}

/**
 * Read thermistor
 * @param therm the address of the therm to read
 * @param ad72 The address of the ad72 device
 * @return Return the therm read value
 */
uint32_t ad7280a_read_therm(therm_t *therm, ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);
    ad7280a_packet_t packet;
    packet.packed = 0;

    // 1. Turn off the read operation
    bus_write(ad72, AD7280A_CONTROL,
            AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
                    | AD7280A_CONTROL_CONV_INPUT_READ_DISABLE,
            WRITE_ALL_ENABLED);

    // 2.Control Register Settings
    bus_write(ad72, AD7280A_CONTROL,
            AD7280A_CONTROL_CONV_INPUT_6CELL_6ADC
                    | AD7280A_CONTROL_CONV_INPUT_READ_6VOLT_6ADC
                    | AD7280A_CONTROL_CONV_START_FORMAT_CNVST
                    | AD7280A_CONTROL_CONV_AVG_BY_8
                    | AD7280A_CONTROL_THERMISTOR_ENABLE
                    | AD7280A_CONTROL_ACQ_TIME_1600NS | AD7280A_CONTROL_MUST_SET,
            WRITE_ALL_ENABLED);

    // 3.Program the CNVST control register to 0x02 to allow ad72 single pulse
    bus_write(ad72, AD7280A_CNVST_CONTROL, AD7280A_CNVST_CTRL_SINGLE,
    WRITE_ALL_DISABLED);

    // 4.Initiate conversions through the falling edge of CNVST.
    HAL_GPIO_WritePin(GPIOB, (1 << GPIOB_CNVST), GPIO_PIN_RESET);

    // 5.1 Allow sufficient time for all conversions to be completed
    vTaskDelay(ad72->delay_ms);

    // 5.2 Latch the CNVST back to one
    HAL_GPIO_WritePin(GPIOB, (1 << GPIOB_CNVST), GPIO_PIN_SET);

    // 6 Gate the CNVST, this prevents unintentional conversions
    bus_write(ad72, AD7280A_CNVST_CONTROL, AD7280A_CNVST_CTRL_GATED,
    WRITE_ALL_DISABLED);

    // 7.On ecrit le numero de registre de la cellule
    bus_write(ad72, AD7280A_READ, ((therm->therm_id + 6) - 1) << 2,
    WRITE_ALL_DISABLED);

    // 8. Apply 32 SCLKS to have the data in the receive buffer
    (ad72->TxData) = AD7280A_RETRANSMIT_SCLKS; // (NODATA)
    spi_exchange(ad72);
    packet.packed = (ad72->RxData);

    return packet.r_conversion.conversion_data;
}

/**
 * Activate a single cell balance
 * @param cell The address of the cell to start balancing
 * @param ad72 The address of the ad72 device
 */
void ad7280a_balance_cell_on(cell_t *cell, ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);

    // Enable the cellbalance
    ad72->cellbalance |= (1 << (cell->cell_id + 1));
    bus_write(ad72, AD7280A_CELL_BALANCE, ad72->cellbalance,
    WRITE_ALL_DISABLED);

    // Verify the cellbalance request is received by the AD7280a
    if ((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
            >> (cell->cell_id + 1)) & 1)
        cell->is_balancing = CELL_IS_BALANCING;

    // Activate the CellBalance Timer (Watchdog) for 72 seconds
    ad72->cellbalance |= (1 << (cell->cell_id + 1));
    bus_write(ad72, AD7280A_CELL_BALANCE + cell->cell_id, 0x8,
    WRITE_ALL_DISABLED);

}

/**
 * Deactivate a single cell balance
 * @param cell The address of the cell to stop balancing
 * @param ad72 The address of the ad72 device
 */
void ad7280a_balance_cell_off(cell_t *cell, ad7280a_t *ad72) {
    configASSERT(ad72!=NULL);
    configASSERT(cell!=NULL);
    // Disable the cellbalance
    ad72->cellbalance &= ~(1 << (cell->cell_id + 1));
    bus_write(ad72, AD7280A_CELL_BALANCE, ad72->cellbalance,
    WRITE_ALL_DISABLED);

    // Verify the cellbalance request is received by the AD7280a
    if (!((ad7280a_read_register(AD7280A_CELL_BALANCE, ad72)
            >> (cell->cell_id + 1)) & 1))
        cell->is_balancing = CELL_IS_NOT_BALANCING;
}
