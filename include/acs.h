#ifndef ACS_H
#define ACS_H

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

typedef enum current_direction {
    CURRENT_IS_POSITIVE = 0x01, CURRENT_IS_NEGATIVE = 0x00
} current_direction_t;

#define ADC_BUFFER_DEPTH 16
//==============================================================================
struct acs {
    uint32_t delay_ms;
    uint32_t threshold;   // Current threshold (0A to 102A)
    int32_t vi_out;      // Analog Voltage coming from the ACS
    int32_t vzcr;        // Analog Zero Current Reference
    uint32_t fault;       // Fault State (Active LOW)
    uint8_t fault_en;     // Enables Overcurrent Faulting when High
    uint32_t current;
    current_direction_t current_direction;

};
typedef struct acs acs_t;

//==============================================================================
// Public:
//==============================================================================

// Initialise the ACS
void acs_init(struct acs *m);

// Set Current threshold (0A to 102A)
uint16_t acs_set_threshold(struct acs *acs, uint8_t amps);

// Enables overcurrent faulting on the ACS
void acs_enable_fault(acs_t *acs);

// Disable overcurrent faulting on the ACS
void acs_disable_fault(acs_t *acs);

// Read the CurrSens coming from the ACS
int32_t acs_read_currsens(acs_t *acs);

// Read the VZCR coming from the ACS
int16_t acs_read_vzcr(acs_t *acs);
//==============================================================================

#endif // ACS_H
