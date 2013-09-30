/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    dac.c
 * @brief   DAC Driver code.
 *
 * @addtogroup DAC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_DAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   DAC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void dacInit(void) {

  dac_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p DACDriver structure.
 *
 * @param[out] dacp     pointer to the @p DACDriver object
 *
 * @init
 */
void dacObjectInit(DACDriver *dacp) {

  dacp->state = DAC_STOP;
  dacp->config = NULL;
#if DAC_USE_WAIT
  dacp->thread = NULL;
#endif /* DAC_USE_WAIT */
#if DAC_USE_MUTUAL_EXCLUSION
#if CH_USE_MUTEXES
  chMtxInit(&dacp->mutex);
#else
  chSemInit(&dacp->semaphore, 1);
#endif
#endif /* DAC_USE_MUTUAL_EXCLUSION */
#if defined(DAC_DRIVER_EXT_INIT_HOOK)
  DAC_DRIVER_EXT_INIT_HOOK(dacp);
#endif
}

/**
 * @brief   Configures and activates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] config    pointer to the @p DACConfig object
 *
 * @api
 */
void dacStart(DACDriver *dacp, const DACConfig *config) {

  chDbgCheck((dacp != NULL) && (config != NULL), "dacStart");

  chSysLock();
  chDbgAssert((dacp->state == DAC_STOP) || (dacp->state == DAC_READY),
              "dacStart(), #1", "invalid state");
  dacp->config = config;
  dac_lld_start(dacp);
  dacp->state = DAC_READY;
  chSysUnlock();
}

/**
 * @brief Deactivates the DAC peripheral.
 * @note  Deactivating the peripheral also enforces a release of the slave
 *        select line.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @api
 */
void dacStop(DACDriver *dacp) {

  chDbgCheck(dacp != NULL, "dacStop");

  chSysLock();
  chDbgAssert((dacp->state == DAC_STOP) || (dacp->state == DAC_READY),
              "dacStop(), #1", "invalid state");
  dac_lld_stop(dacp);
  dacp->state = DAC_STOP;
  chSysUnlock();
}

/**
 * @brief   Sends data over the DAC bus.
 * @details This asynchronous function starts a one-shot transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void dacStartSend(DACDriver *dacp) {

  chDbgCheck((dacp != NULL), "dacStartSend");

  chSysLock();
  chDbgAssert(dacp->state == DAC_READY, "dacStartSend(), #1", "not ready");
  dacStartSendI(dacp);
  chSysUnlock();
}

#if DAC_USE_WAIT || defined(__DOXYGEN__)

/**
 * @brief   Sends data over the DAC bus.
 * @details This synchronous function performs a transmit operation.
 * @pre     In order to use this function the option @p DAC_USE_WAIT must be
 *          enabled.
 * @pre     In order to use this function the driver must have been configured
 *          without callbacks (@p callback = @p NULL).
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void dacSend(DACDriver *dacp) {

  chDbgCheck((dacp != NULL), "dacSend");
  chSysLock();
  chDbgAssert(dacp->state == DAC_READY, "dacSend(), #1", "not ready");
  chDbgAssert(dacp->config->callback == NULL, "dacSend(), #2", "has callback");
  dacStartSendI(dacp);
  _dac_wait_s(dacp);
  chSysUnlock();
}
#endif /* DAC_USE_WAIT */

#if DAC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the DAC bus.
 * @details This function tries to gain ownership to the DAC bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option @p DAC_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @api
 */
void dacAcquireBus(DACDriver *dacp) {

  chDbgCheck(dacp != NULL, "dacAcquireBus");

#if CH_USE_MUTEXES
  chMtxLock(&dacp->mutex);
#elif CH_USE_SEMAPHORES
  chSemWait(&dacp->semaphore);
#endif
}

/**
 * @brief   Releases exclusive access to the DAC bus.
 * @pre     In order to use this function the option @p DAC_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @api
 */
void dacReleaseBus(DACDriver *dacp) {

  chDbgCheck(dacp != NULL, "dacReleaseBus");

#if CH_USE_MUTEXES
  (void)dacp;
  chMtxUnlock();
#elif CH_USE_SEMAPHORES
  chSemSignal(&dacp->semaphore);
#endif
}
#endif /* DAC_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_DAC */

/** @} */
