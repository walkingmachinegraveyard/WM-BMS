
#include <stdint.h>
#include <ch.h>
#include <hal.h>

#include "acs.h"

#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP1_BUF_DEPTH      2048*2*4
static volatile int32_t samples_average = 0;
static volatile int32_t samples_average2 = 0;
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

static void adc_conversion_end(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
  (void) adcp;
  uint16_t i=0;
  for (i=0; i<n; i++) {
      samples_average += buffer[i];
  }
  samples_average /= n;
  return;
}

static void adc_error(ADCDriver *adcp, adcerror_t err) {
  (void) adcp;
  (void) err;
}

static void adc_conversion_end2(ADCDriver *adcp2, adcsample_t *buffer2, size_t n2) {
  (void) adcp2;
  uint16_t i2=0;
  for (i2=0; i2<n2; i2++) {
      samples_average2 += buffer2[i2];
  }
  samples_average2 /= n2;
  return;
}

static void adc_error2(ADCDriver *adcp2, adcerror_t err2) {
  (void) adcp2;
  (void) err2;
}

static ADCConversionGroup adc_convgroup_config = {
  FALSE,               // continuous
  1,                  // 2 channels
  adc_conversion_end, // conversion_end callback
  adc_error,          // adc_error callback
  0,                  // CR1 - config register 1
  ADC_CR2_SWSTART,    // CR2 - config register 2
  0,                  // SMPR1 - sample time register 1
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_15),   // SMPR2 - sample time register 2
  ADC_SQR1_NUM_CH(1),                 // SQR1 - sequence register 1
  0,                                  // SQR2 - sequence register 2
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10),   // CurrentSens Channel
};

static ADCConversionGroup adc_convgroup_config2 = {
  FALSE,               // continuous
  1,                  // 2 channels
  adc_conversion_end2, // conversion_end callback
  adc_error2,          // adc_error callback
  0,                  // CR1 - config register 1
  ADC_CR2_SWSTART,    // CR2 - config register 2
  0,                  // SMPR1 - sample time register 1
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_15),   // SMPR2 - sample time register 2
  ADC_SQR1_NUM_CH(2),                 // SQR1 - sequence register 1
  0,                                  // SQR2 - sequence register 2
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11),    // VZCR Channel
};

int16_t acs_read_currsens(acs_t *acs) {
  (void) acs;
  adcStart(acs->ad, NULL);
  adcStartConversion(acs->ad, acs->acg, acs->samples, ADC_BUFFER_DEPTH);
  chThdSleepMilliseconds(acs->delay_ms);
  adcStop(acs->ad);
  return (int16_t)samples_average;
}

int16_t acs_read_vzcr(acs_t *acs) {
  (void) acs;
  adcStart(acs->ad2, NULL);
  adcStartConversion(acs->ad2, acs->acg2, acs->samples2, ADC_BUFFER_DEPTH);
  chThdSleepMilliseconds(acs->delay_ms);
  adcStop(acs->ad2);
  return (int16_t)samples_average2;
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
  acs->ad     = &ADCD3;
  acs->ad2    = &ADCD2;
  acs->acg    = &adc_convgroup_config;
  acs->acg2   = &adc_convgroup_config2;
  acs->delay_ms = 100;

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
// Dans notre cas le Sens = 28mV/A
// Choisir une valeure entre 0A et 102A
uint16_t acs_set_threshold(acs_t *acs, uint8_t amps) {
  (void) acs;
  if(amps>102) {
    amps = 102;
  }
  // Set the value between 0 and 2^12
  acs->threshold = (uint16_t)(amps * 40);
  // Set the threshold
  dac_buffer = acs->threshold;
  // Start the dac driver
  dacStart(acs->dd,acs->dc);
  // Start sending the buffer
  dacStartSend(acs->dd);
  return 0;
}

