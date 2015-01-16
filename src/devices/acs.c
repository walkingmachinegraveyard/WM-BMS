#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_device.h"
#include "acs.h"
#include "adc.h"
#include "Board.h"

#define ACS_CURRENTSENS_RATIO   18.48
#define ACS_MAX_CURRENT         50

uint16_t dac_buffer = 2048;

/**
 * This function initiate a lecture for of the output current
 * @param acs The address of the ACS current sensor
 * @return    The value in mA of the current being read
 */
int32_t acs_read_currsens(acs_t *acs) {
    configASSERT(acs!=NULL);

    int16_t result;
    float cal;

    cal = (((3.3 / 4096) * uhADCxConvertedValue2)) * 1000;
    acs->vi_out = cal;

    cal = (((3.3 / 4096) * uhADCxConvertedValue1) * 1000);
    acs->vzcr = cal;

    result = ((acs->vi_out - acs->vzcr) / ACS_CURRENTSENS_RATIO) * 1000;
    if (result < 0) {

        acs->current_direction = CURRENT_IS_NEGATIVE;
        result *= -1;
    } else {
        acs->current_direction = CURRENT_IS_POSITIVE;
    }

    acs->current = result;

    return result;
}

/**
 * Enable overcurrent faulting
 * @param acs The address of the ACS current sensor
 */
void acs_enable_fault(acs_t *acs) {
    configASSERT(acs!=NULL);
    HAL_GPIO_WritePin(GPIOA, (1 << GPIOA_CURRFAULT), GPIO_PIN_SET);
    acs->fault_en = 1;
}

/**
 * Disable overcurrent faulting
 * @param acs The address of the ACS current sensor
 */
void acs_disable_fault(acs_t *acs) {
    configASSERT(acs!=NULL);
    HAL_GPIO_WritePin(GPIOA, (1 << GPIOA_CURRFAULT), GPIO_PIN_RESET);
    acs->fault_en = 0;
}

void acs_init(acs_t *acs) {
    configASSERT(acs!=NULL);
    acs->delay_ms = 10;
    acs->threshold = ACS_MAX_CURRENT;   // Current threshold (0A to 102A)
    acs->vi_out = 0;      // Analog Voltage coming from the ACS
    acs->vzcr = 0;        // Analog Zero Current Reference
    acs->fault = 1;       // Fault State (Active LOW)
    acs->fault_en = 1;     // Enables Overcurrent Faulting when High
    acs->current = 0;
}

///**
// * VOC = Sens × | IOC |
// * Dans notre cas le Sens = 18.48mV/A
// * LSB of the DAC is
//   Choisir une valeure entre 0A et 60A
// * @param acs The address of the ACS current sensor
// * @param amps The ammount of AMPS you want to limit
// * @return
// */
//uint16_t acs_set_threshold(acs_t *acs, uint8_t amps) {
//  (void) acs;
//  if(amps>60) {
//    amps = 60;
//  }
//  acs->threshold = (uint16_t)(amps * ACS_CURRENTSENS_RATIO);
//
//  /*##-1- Initialize the DAC peripheral ######################################*/
//  HAL_DAC_Init(&DacHandle
//
//
//  /*##-2- DAC channel1 Configuration #########################################*/
//  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
//  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
//
//  HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL1)
//
//  /*##-3- Enable DAC Channel1 and associeted DMA #############################*/
//  HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL1, (uint32_t*)amps, 1, DAC_ALIGN_8B_R)
//
//  return 0;
//}

