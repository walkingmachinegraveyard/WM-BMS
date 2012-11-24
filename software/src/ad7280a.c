
#include "ad7280a.h"


#define RX_BUFFER_LENGTH 4
static uint8_t rx_buffer[RX_BUFFER_LENGTH];
static uint8_t tx_buffer[4];

//
// AD7280A private
//

// AD7280A checksum
//
// Computed on Bit[31-10]
static uint32_t do_crc8(uint32_t buffer) {
    return 0;
}

//
// AD7280A public
//

/**
 * Open an AD7280A
 */
uint8_t ad7280a_open(ad7280a_t *a, SPIDriver *spid) {
    a->spid = spid;
    a->delay_ms = 6;
}

uint8_t ad7280a_probe(ad7280a_t *a) {

}

/**
 * Close an AD7280A
 */
uint8_t ad7280a_close(ad7280a_t *a) {
}

static void bus_lock(ad7280a_t *a) {
	spiAcquireBus(s->spid);
	spiSelect(s->spid);
}

static void bus_unlock(ad7280a_t *a) {
	spiUnselect(s->spid):
	spiReleaseBus(s->spid);
}

static int8_t bus_read_conversion(ad7280_t *a) {
	uint8_t channel_address = 0;
	uint16_t conversion_data = 0;

	bus_read(a);

	channel_address = packet >> 23 & 0x0F;
	conversion_data = packet >> 11 & 0x7FF;
}

static int8_t bus_read_register(ad7280_t *a) {
	uint8_t register_address = 0;
	uint8_t register_data = 0;

	bus_read(a);

	register_address = packet >> 21 & 0x3F;
	register_data = packet >> 13 & 0xFF;
}

static int8_t bus_read(ad7280_t *a, uint32_t dev, uint32_t reg) {
	uint32_t packet;

	spiReceive(s->spid, 1, &packet);

	write_ack = packet >> 10 & 0x01;
	crc = packet >> 2 & 0xFF;

	if (!write_ack) {
		// Fail: no write acknowledge
		return 1;
	}

	computed_crc = crc8(packet);

	if (computed_crc != crc) {
		// Fail: invalid crc
		return 1;
	}

	return 0;
}

static int8_t bus_write(ad7280_t *a, uint32_t dev, uint32_t reg, uint32_t val, uint32_t all) {
	// Pack data
	packet |= dev << 27;	// device address
	if (all) {
		// If 'all' flag is on, address should be 0
		packet = 0x0;
	}
	packet |= reg << 21;	// register address
	packet |= val << 13;	// register data
	packet |= all << 12;	// address all flag
	packet |= 0x2 << 0;		// bit pattern

	// Compute CRC
	crc = crc8(packet);
	packet |= crc << 0x03;

	// Send data on the SPI bus
	spiSend(s->spid, 1, (void *)&buffer);

	return 0;
}

/**
 * Read from an AD7280A
 */
int32_t ad7280a_read(ad7280_t *a, uint32_t dev, uint32_t reg) {
    uint32_t packet = 0;
    uint32_t dev = 0;
    uint32_t reg = 0;
    uint32_t val = 0;
    uint32_t all = 0;


	// 0. Lock the SPI bus
	bus_lock(a);

	// 1. Stop actual reading 
	bus_write(a, dev, AD7280A_READ, 0x00, reg << 0x2);

	// 2. Write the read address in the read register
	bus_write(a, dev, AD7280A_READ, 0x00, reg << 0x2);
	bus_read(a, &packet);
	bus_unlock(a);

    // Verify CRC
    if (crc8(rx_buffer) != rx_buffer >> 0x10) {
        return -1;
    }

    // Unpack data
    dev = packet >> 0x27;
    reg = (packet >> 0x21) & 0x0F;
    val = (packet >> 0x13) & 0x10;
    all = (packet >> 0x12) & 0x01;

    return val;
}

/**
 * Write to an AD7280A
 */
int32_t ad7280a_write(ad7280_t *a, uint32_t dev, uint32_t reg, uint32_t val, uint32_t all) {
    uint32_t packet = 0;
    uint32_t crc = 0;

	bus_lock(a);
	bus_write(a, dev, reg, val, all);
	bus_unlock(a);
}

//
// Public API
//

/**
 * Read all channels (Voltage + ADC) from the AD7280A
 */
int32_t ad7280a_read_all_channels(ad7280_t *a) {
}

