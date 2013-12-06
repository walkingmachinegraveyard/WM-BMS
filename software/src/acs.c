
#include <stdint.h>
#include <ch.h>
#include <hal.h>

#include "acs.h"

#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP1_BUF_DEPTH      2048*2*4
#define ACS_CURRENTSENS_RATIO   18.48
static volatile int32_t samples_average_CurrentSens = 0;
static volatile int32_t samples_average_VZCR = 0;

uint16_t dac_buffer = 2048;

// Dac callback
static void daccb(DACDriver *dacp) {
  (void)dacp;
}

// Dac error callback
static void dacerrcb(DACDriver *dacp) {
  (void)dacp;
}

// DAC Specific configurations
static DACConfig dac_config = {
  DAC_MODE_CONTINUOUS,
  20000,                // Frequency in Hz
  &dac_buffer,          // Pointer to dac_buffer
  NULL,                 // Pointer to second dac_buffer
  1,                    // dac_buffer size
  daccb,                // End of transfert callback
  dacerrcb,             // error callback
  DAC_DHRM_12BIT_RIGHT, // data holding register mode
  0                     // DAC CR flags
};

static void adc_conversion_end_CurrentSens(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
  (void) adcp;
  uint16_t i=0;
  samples_average_CurrentSens = 0;
  for (i=0; i<n; i++) {
      samples_average_CurrentSens += buffer[i];
  }
  samples_average_CurrentSens /= n;
  return;
}

static void adc_conversion_end_VZCR(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
  (void) adcp;
  uint16_t i=0;
  samples_average_VZCR =0;
  for (i=0; i<n; i++) {
      samples_average_VZCR += buffer[i];
  }
  samples_average_VZCR /= n;
  return;
}

static void adc_error(ADCDriver *adcp, adcerror_t err) {
  (void) adcp;
  (void) err;
}

static ADCConversionGroup adc_convgroup_config_CurrentSens= {
  FALSE,               // continuous
  1,                  // 2 channels
  adc_conversion_end_CurrentSens, // conversion_end callback
  adc_error,          // adc_error callback
  0,                  // CR1 - config register 1
  ADC_CR2_SWSTART,    // CR2 - config register 2
  0,                  // SMPR1 - sample time register 1
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_480),   // SMPR2 - sample time register 2
  ADC_SQR1_NUM_CH(1),                 // SQR1 - sequence register 1
  0,                                  // SQR2 - sequence register 2
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10),   // CurrentSens Channel
};

static ADCConversionGroup adc_convgroup_config_VZCR= {
  FALSE,               // continuous
  1,                  // 2 channels
  adc_conversion_end_VZCR, // conversion_end callback
  adc_error,          // adc_error callback
  0,                  // CR1 - config register 1
  ADC_CR2_SWSTART,    // CR2 - config register 2
  0,                  // SMPR1 - sample time register 1
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_480),   // SMPR2 - sample time register 2
  ADC_SQR1_NUM_CH(1),                 // SQR1 - sequence register 1
  0,                                  // SQR2 - sequence register 2
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11),   // CurrentSens Channel
};

int32_t acs_read_currsens(acs_t *acs) {

  int16_t result;
  float cal;

  adcStart(acs->adVZCR, NULL);
  adcStartConversion(acs->adVZCR, acs->acgVZCR, acs->samplesVZCR, ADC_BUFFER_DEPTH);
  adcStop(acs->adVZCR);

  adcStart(acs->adCurrentSens, NULL);
  adcStartConversion(acs->adCurrentSens, acs->acgCurrentSens, acs->samplesCurrentSens, ADC_BUFFER_DEPTH);
  adcStop(acs->adCurrentSens);

  cal = (((3.3/4096) * samples_average_CurrentSens)) * 1000;
  acs->vi_out = cal;

  cal = (((3.3/4096) * samples_average_VZCR) * 1000);
  acs->vzcr = cal;

  result = ((acs->vi_out - acs->vzcr) / ACS_CURRENTSENS_RATIO) * 1000;


  if(result < 0) {
    acs->current_direction = CURRENT_IS_NEGATIVE;
    result *= -1;
  } else {
    acs->current_direction = CURRENT_IS_POSITIVE;
  }

  return result;
}

// Enable overcurrent faulting
void acs_enable_fault(acs_t *acs) {
  palSetPad(GPIOA,3);
  acs->fault_en = 1;
}

// Disable overcurrent faulting
void acs_disable_fault(acs_t *acs) {
  palClearPad(GPIOA,3);
  acs->fault_en = 0;
}

static uint8_t acs_init_hw(acs_t *acs) {

  acs->dd     = &DACD1;
  acs->dc     = &dac_config;
  acs->vi_out = 0;
  acs->fault  = 1;
  acs->adCurrentSens     = &ADCD1;
  acs->adVZCR            = &ADCD2;
  acs->acgCurrentSens    = &adc_convgroup_config_CurrentSens;
  acs->acgVZCR           = &adc_convgroup_config_VZCR;
  acs->delay_ms          = 100;

  // Set output pin for overcurrent fault enable
  palSetGroupMode(GPIOA, PAL_PORT_BIT(3), 0, PAL_MODE_OUTPUT_PUSHPULL);
  acs_disable_fault(acs);
  // Set ouput pin to analog for the ACS VOC
  palSetGroupMode(GPIOA, PAL_PORT_BIT(4), 0, PAL_MODE_INPUT_ANALOG);
  // Set input pin for ADC VIout
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0), 0, PAL_MODE_INPUT_ANALOG);
  // Set input pin to analog for the CurrentSens
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0), 0, PAL_MODE_INPUT_ANALOG);
  // Set input pin to analog for the VZCR
  palSetGroupMode(GPIOC, PAL_PORT_BIT(1), 0, PAL_MODE_INPUT_ANALOG);

  return 0;
}

// ACS initialise
uint8_t acs_init(acs_t *acs) {
    acs_init_hw(acs);
    return 0;
}

// VOC = Sens × | IOC |
// Dans notre cas le Sens = 18.48mV/A
// LSB of the DAC is
// Choisir une valeure entre 0A et 60A
uint16_t acs_set_threshold(acs_t *acs, uint8_t amps) {
  (void) acs;
  if(amps>60) {
    amps = 60;
  }
  acs->threshold = (uint16_t)(amps * ACS_CURRENTSENS_RATIO);
  // Set the threshold
  dac_buffer = acs->threshold;
  // Start the dac driver
  dacStart(acs->dd,acs->dc);
  // Start sending the buffer
  dacStartSend(acs->dd);
  return 0;
}

