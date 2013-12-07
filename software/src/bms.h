/**
 * Battery management system, Walking Machine, Ecole de Technologie Superieure
 * Copyright (C) 2012 Benjamin Vanheuverzwijn
 *
 * BMS software main header
 */

#ifndef BMS_H
#define BMS_H

struct bms {
    char *progname;
};

typedef struct therm {
    uint8_t therm_id;
    uint8_t present;
    uint32_t temperature;
} therm_t;

typedef struct ad7280a {
    uint8_t delay_ms;
    uint32_t rxbuf;
    uint32_t txbuf;
    uint32_t cellbalance;
    uint32_t on_off;
} ad7280a_t;

typedef struct bms bms_t;

void init_bms(bms_t *bms);

#endif
