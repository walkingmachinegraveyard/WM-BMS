#ifndef BATTERY_H
#define BATTERY_H

enum battery_status {
    BATTERY_STATUS_UNKNOWN = 0x00,
    BATTERY_STATUS_CHARGING = 0x01,
    BATTERY_STATUS_DISCHARGING =0x02,
    BATTERY_STATUS_FULL = 0x03
};
typedef enum battery_status battery_status_t;

enum battery_health {
    BATTERY_HEALTH_UNKNOWN = 0x00,
    BATTERY_HEALTH_GOOD = 0x01,
    BATTERY_HEALTH_OVERHEAT = 0x02,
    BATTERY_HEALTH_DEAD = 0x03,
    BATTERY_HEALTH_OVER_VOLTAGE = 0x04,
    BATTERY_HEALTH_OVER_CURRENT = 0x05,
    BATTERY_IS_INFERIOR_TO_MAX_DELTA = 0x06,
    BATTERY_IS_SUPERIOR_TO_MAX_DELTA = 0x07,
    BATTERY_HEALTH_UNSPECIFIED_FAILURE = 0x08
};
typedef enum battery_health battery_health_t;

struct battery {
    battery_status_t status;
    battery_health_t health;
    uint16_t temperature;
    uint16_t voltage;
};
typedef struct battery battery_t;

// Init Battery
void battery_init(battery_t bat[]);


#endif // BATTERY_H
