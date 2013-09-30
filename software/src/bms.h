/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * BMS software main header
 */

#ifndef BMS_H
#define BMS_H

#include "ad72.h"
#include "battery.h"
#include "acs.h"

struct bms {
    char *progname;
};

typedef struct bms bms_t;

void init_bms(bms_t *bms);

#endif
