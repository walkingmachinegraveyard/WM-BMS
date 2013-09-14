//Author: Benjamin Vanheuverzwijn
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#include <ch.h>
#include <hal.h>

struct console {
    SerialDriver *sd;
    SerialConfig *sc;
};

void console_init(struct console *c);

int16_t console_readline(struct console *c, uint8_t *buffer, uint16_t count);

int16_t console_read(struct console *c, uint8_t *buffer, uint16_t count);

int16_t console_writeline(struct console *c, const char *buffer);

int16_t console_write(struct console *c, const char *buffer);

#endif
