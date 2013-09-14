//Author: Benjamin Vanheuverzwijn
// libc
#include <stdint.h>
// chibios
#include <ch.h>
#include <hal.h>
// drive
#include "console.h"

static SerialConfig serial2_config = {
    38400,
    0,
    0,
    0,
};


//Sur le BMS -> USART1_RX = PA10
//           -> USART1_TX = PA9

static void console_init_hw(struct console *c) {
    c->sd = &SD2;
    c->sc = &serial2_config;

    sdStart(c->sd, c->sc);
    palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
}

void console_init(struct console *c) {
    console_init_hw(c);
}

int16_t console_readline(struct console *c, uint8_t *buffer, uint16_t count) {
    (void) c;
    (void) buffer;
    (void) count;
    return 0;
}

int16_t console_read(struct console *c, uint8_t *buffer, uint16_t count) {
    (void) c;
    (void) buffer;
    (void) count;
    return 0;
}

int16_t console_writeline(struct console *c, const char *buffer) {
    int16_t bytes_sent = 0;

    bytes_sent = console_write(c, buffer);
    bytes_sent += console_write(c, "\r\n");

    return bytes_sent;
}

int16_t console_write(struct console *c, const char *buffer) {
    uint16_t i = 0;

    while (buffer[i] != '\0') {
        sdPut(c->sd, buffer[i++]);
    }

    return i;
}

