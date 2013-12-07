#ifndef ACS_H
#define ACS_H

#include <stdint.h>

#include <ch.h>
#include <hal.h>

typedef enum current_direction {
    CURRENT_IS_POSITIVE = 0x01,
    CURRENT_IS_NEGATIVE = 0x00
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
    DACDriver *dd;
    DACConfig *dc;
    ADCDriver *adCurrentSens;
    ADCConversionGroup *acgCurrentSens;
    adcsample_t samplesCurrentSens[ADC_BUFFER_DEPTH];
    ADCDriver *adVZCR;
    ADCConversionGroup *acgVZCR;
    adcsample_t samplesVZCR[ADC_BUFFER_DEPTH];
};
typedef struct acs acs_t;

//==============================================================================
// Public:
//==============================================================================

// Initialise the ACS
uint8_t acs_init(struct acs *m);

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
