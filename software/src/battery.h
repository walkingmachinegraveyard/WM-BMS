#ifndef BATTERY_H
#define BATTERY_H

#include "ad72.h"

enum battery_status {
    BATTERY_STATUS_UNKNOWN,
    BATTERY_STATUS_CHARGING,
    BATTERY_STATUS_DISCHARGING,
    BATTERY_STATUS_FULL,
};
typedef enum battery_status battery_status_t;

enum battery_health {
    BATTERY_HEALTH_UNKNOWN,
    BATTERY_HEALTH_GOOD,
    BATTERY_HEALTH_OVERHEAT,
    BATTERY_HEALTH_DEAD,
    BATTERY_HEALTH_OVER_VOLTAGE,
    BATTERY_HEALTH_UNSPECIFIED_FAILURE,
};
typedef enum battery_health battery_health_t;

enum battery_power {
    BATTERY_POWER_UNKNOWN,
    BATTERY_POWER_NONE,
    BATTERY_POWER_AC,
};
typedef enum battery_power battery_power_t;

struct battery {
    battery_status_t status;
    battery_health_t health;
    battery_power_t power;
    uint32_t temperature;
    uint32_t voltage;
};
typedef struct battery battery_t;

// Init Battery
void battery_init(battery_t bat[]);

// Read all voltages;
uint8_t battery_check_voltage(ad7280a_t *a,battery_t cell[]);


#endif // BATTERY_H
