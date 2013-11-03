#ifndef BATTERY_H
#define BATTERY_H


// Local
#include <stdint.h>
#include "bms.h"

typedef enum battery_status {
    BATTERY_STATUS_UNKNOWN = 0x00,
    BATTERY_STATUS_CHARGING = 0x01,
    BATTERY_STATUS_DISCHARGING = 0x02,
    BATTERY_STATUS_FULL = 0x04
} battery_status_t;

typedef enum battery_health {
    BATTERY_HEALTH_UNKNOWN = 0x00,
    BATTERY_HEALTH_GOOD = 0x01,
    BATTERY_HEALTH_OVERHEAT = 0x02,
    BATTERY_HEALTH_DEAD = 0x04,
    BATTERY_HEALTH_OVER_VOLTAGE = 0x08,
    BATTERY_HEALTH_UNDER_VOLTAGE = 0x10,
    BATTERY_HEALTH_UNSPECIFIED_FAILURE = 0x20
} battery_health_t;

enum battery_balancing {
    BATTERY_IS_BALANCING = 0x00,
    BATTERY_IS_NOT_BALANCING = 0x01
};
typedef enum battery_balancing battery_balancing_t;

typedef struct {
    uint8_t present;
    uint8_t cell_id;
    battery_status_t status;
    battery_health_t health;
    battery_balancing_t is_balancing;
    uint32_t temperature;
    uint32_t voltage;
} battery_t;


// Init Battery
void battery_init(battery_t bat[]);

uint32_t battery_get_voltage(battery_t *bat);

#endif // BATTERY_H
