#ifndef ACS_H
#define ACS_H

#include <stdint.h>

#include <ch.h>
#include <hal.h>

#define ADC_BUFFER_DEPTH 16
//==============================================================================
struct acs {
    uint32_t delay_ms;
    uint32_t threshold;   // Current threshold (0A to 102A)
    uint32_t vi_out;      // Analog Voltage coming from the ACS
    uint32_t vzcr;        // Analog Zero Current Reference
    uint32_t fault;       // Fault State (Active LOW)
    uint8_t fault_en;     // Enables Overcurrent Faulting when High
    DACDriver *dd;
    DACConfig *dc;
    ADCDriver *ad;
    ADCConversionGroup *acg;
    adcsample_t samples[ADC_BUFFER_DEPTH];
    ADCDriver *ad2;
    ADCConversionGroup *acg2;
    adcsample_t samples2[ADC_BUFFER_DEPTH];
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
int16_t acs_read_currsens(acs_t *acs);

// Read the VZCR coming from the ACS
int16_t acs_read_vzcr(acs_t *acs);
//==============================================================================

#endif // ACS_H
