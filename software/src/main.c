/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * BMS software entry point
 */

// libc

// ChibiOS
#include <ch.h>
#include <hal.h>

// BMS


bms_t bms;

/**
 * Entry point - init OS
 */
int main(int argc, char *argv[]) {
    // Init OS
    halInit();
    chSysInit();

    // Init systems

    // Infinite loop
    while (true) {
        chThdSleepMilliseconds(1337);
    }

    return 0;
}
