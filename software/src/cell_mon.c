/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * Cell monitor
 */

#include <stdbool.h>
#include <stdint.h>

#include <ch.h>

#include "ad7280a.h"
#include "cell_mon.h"

// Thread configuration
static WORKING_AREA(cell_monitor_wa, 128);

static void cell_monitor(void *arg) {
    // Remove a warning...
    (void) arg;

    // Thread loop
    while (true) {
        // Do something!
        chThdSleepMilliseconds(100);
    }
}

/**
 * Initialize cell monitior
 */
void cell_mon_init(cell_mon_t *cmon, ad7280a_t *ad7280a) {
    cmon->ad7280a = ad7280a;

    chThdCreateStatic(cell_monitor_wa, sizeof(cell_monitor_wa), NORMALPRIO, cell_monitor);
}
