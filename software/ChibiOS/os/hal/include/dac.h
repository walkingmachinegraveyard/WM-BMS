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
 * @file    dac.h
 * @brief   DAC Driver macros and structures.
 *
 * @addtogroup DAC
 * @{
 */

#ifndef _DAC_H_
#define _DAC_H_

#if HAL_USE_DAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    DAC modes
 * @{
 */
typedef enum {
  DAC_MODE_ONESHOT = 0,             /**< Only send buffer once.    */
  DAC_MODE_CONTINUOUS = 1,        /**< Send buffer continuously.    */
  DAC_MODE_DOUBLEBUFFER = 2,      /**< Send both buffers continuously. */
} dacmode_t;
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    DAC configuration options
 * @{
 */
/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(DAC_USE_WAIT) || defined(__DOXYGEN__)
#define DAC_USE_WAIT                TRUE
#endif

/**
 * @brief   Enables the @p dacAcquireBus() and @p dacReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(DAC_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define DAC_USE_MUTUAL_EXCLUSION    TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if DAC_USE_MUTUAL_EXCLUSION && !CH_USE_MUTEXES && !CH_USE_SEMAPHORES
#error "DAC_USE_MUTUAL_EXCLUSION requires CH_USE_MUTEXES and/or CH_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  DAC_UNINIT = 0,                   /**< Not initialized.                   */
  DAC_STOP = 1,                     /**< Stopped.                           */
  DAC_READY = 2,                    /**< Ready.                             */
  DAC_ACTIVE = 3,                   /**< Exchanging data.                   */
  DAC_COMPLETE = 4,                 /**< Asynchronous operation complete.   */
  DAC_ERROR = 5 			        /**< Error.                             */
} dacstate_t;

#include "dac_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */

/**
 * @brief   Sends data over the DAC bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @iclass
 */
#define dacStartSendI(dacp) {                               \
  (dacp)->state = DAC_ACTIVE;                              \
  if ((dacp)->config->mode == DAC_MODE_ONESHOT) { \
    dac_lld_send(dacp);                                            \
    } \
  else if ((dacp)->config->mode == DAC_MODE_CONTINUOUS) { \
    dac_lld_send_continuous(dacp);                                            \
    } \
  else if ((dacp)->config->mode == DAC_MODE_DOUBLEBUFFER) { \
    dac_lld_send_doublebuffer(dacp);                                            \
    } \
}

/**
 * @name    Low Level driver helper macros
 * @{
 */
#if DAC_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Waits for operation completion.
 * @details This function waits for the driver to complete the current
 *          operation.
 * @pre     An operation must be running while the function is invoked.
 * @note    No more than one thread can wait on a DAC driver using
 *          this function.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
#define _dac_wait_s(dacp) {                                                 \
  chDbgAssert((dacp)->thread == NULL,                                       \
              "_dac_wait(), #1", "already waiting");                        \
  (dacp)->thread = chThdSelf();                                             \
  chSchGoSleepS(THD_STATE_SUSPENDED);                                       \
}

/**
 * @brief   Wakes up the waiting thread.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
#define _dac_wakeup_isr(dacp) {                                             \
  if ((dacp)->thread != NULL) {                                             \
    Thread *tp = (dacp)->thread;                                            \
    (dacp)->thread = NULL;                                                  \
    chSysLockFromIsr();                                                     \
    chSchReadyI(tp);                                                        \
    chSysUnlockFromIsr();                                                   \
  }                                                                         \
}
#else /* !DAC_USE_WAIT */
#define _dac_wait_s(dacp)
#define _dac_wakeup_isr(dacp)
#endif /* !DAC_USE_WAIT */

/**
 * @brief   Full transfer ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          - Waiting thread wakeup, if any.
 *          - Driver state transitions.
 *          .
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
#define _dac_isr_full_code(dacp) {                                               \
  if ((dacp)->config->callback) {                                             \
    (dacp)->state = DAC_COMPLETE;                                           \
    (dacp)->config->callback(dacp);                                           \
    if ((dacp)->state == DAC_COMPLETE)                                      \
      (dacp)->state = DAC_READY;                                            \
  }                                                                         \
  else {                                                                      \
    (dacp)->state = DAC_READY;                                              \
  }                                                                         \
  _dac_wakeup_isr(dacp);                                                  \
}

/**
 * @brief   Half transfer ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          - Waiting thread wakeup, if any.
 *          - Driver state transitions.
 *          .
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
#define _dac_isr_half_code(dacp) {                                               \
/* TODO */                                                                       \
}
/** @} */

/**
 * @brief   Error handler ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          - Waiting thread wakeup, if any.
 *          - Driver state transitions.
 *          .
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
#define _dac_isr_error_code(dacp) {                                               \
  if ((dacp)->config->errcallback) {                                             \
    (dacp)->state = DAC_ERROR;                                           \
    (dacp)->config->errcallback(dacp); }                                           \
  else {                                                                      \
    (dacp)->state = DAC_ERROR;                                              \
  }      \
  _dac_wakeup_isr(dacp);                                                  \
}
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void dacInit(void);
  void dacObjectInit(DACDriver *dacp);
  void dacStart(DACDriver *dacp, const DACConfig *config);
  void dacStop(DACDriver *dacp);
  void dacStartSend(DACDriver *dacp);
#if DAC_USE_WAIT
  void dacSend(DACDriver *dacp);
#endif /* DAC_USE_WAIT */
#if DAC_USE_MUTUAL_EXCLUSION
  void dacAcquireBus(DACDriver *dacp);
  void dacReleaseBus(DACDriver *dacp);
#endif /* DAC_USE_MUTUAL_EXCLUSION */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DAC */

#endif /* _DAC_H_ */

/** @} */
