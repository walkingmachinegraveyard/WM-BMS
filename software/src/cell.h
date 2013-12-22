#ifndef cell_H
#define cell_H

// Local
#include <stdint.h>
#include "bms.h"
#include "console.h"

typedef enum cell_status {
    CELL_STATUS_UNKNOWN = 0x00,
    CELL_STATUS_CHARGING = 0x01,
    CELL_STATUS_DISCHARGING = 0x02,
    CELL_STATUS_FULL = 0x04
} cell_status_t;

typedef enum cell_health {
    CELL_HEALTH_UNKNOWN = 0x00,
    CELL_HEALTH_GOOD = 0x01,
    CELL_HEALTH_OVERHEAT = 0x02,
    CELL_HEALTH_DEAD = 0x04,
    CELL_HEALTH_OVER_VOLTAGE = 0x08,
    CELL_HEALTH_UNDER_VOLTAGE = 0x10,
    CELL_HEALTH_UNSPECIFIED_FAILURE = 0x20
} cell_health_t;

enum cell_balancing {
    CELL_IS_BALANCING = 0x01,
    CELL_IS_NOT_BALANCING = 0x00
};
typedef enum cell_balancing cell_balancing_t;

typedef struct {
    uint32_t cell_id;
    cell_status_t status;
    cell_health_t health;
    cell_balancing_t is_balancing;
    uint32_t temperature;
    uint32_t voltage;
} cell_t;

// Init cell
void cell_init(cell_t cell[], ad7280a_t *ad72);

uint32_t cell_get_voltage(cell_t *cell);


#endif // cell_H
