/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * BMS software main header
 */

#ifndef BMS_H
#define BMS_H

#define WRITE_ALL_ENABLED  1
#define WRITE_ALL_DISABLED 0

struct bms {
    char *progname;
};

typedef struct {
    uint8_t therm_id;
    uint8_t present;
    uint32_t temperature;
} therm_t;

typedef struct bms bms_t;

void init_bms(bms_t *bms);

#endif
