//Author: Benjamin Vanheuverzwijn
// libc
#include <stdint.h>
// chibios
#include <ch.h>
#include <hal.h>
// drive
#include "console.h"

static SerialConfig serial1_config = {
    38400,
    0,
    0,
    0,
};


//Sur le BMS -> USART1_RX = PA10
//           -> USART1_TX = PA9

void console_init(struct console *c) {
    c->sd = &SD1;
    c->sc = &serial1_config;

    sdStart(c->sd, c->sc);
    palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));
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

int16_t console_writeline(console_t *c, const char *buffer) {
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

char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i = -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
