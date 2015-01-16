//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdint.h>
#include "cortexm/ExceptionHandlers.h"

// ----------------------------------------------------------------------------

extern void
__attribute__((noreturn))
_start(void);

// ----------------------------------------------------------------------------
// Default exception handlers. Override the ones here by defining your own
// handler routines in your application code.
// ----------------------------------------------------------------------------

#if defined(DEBUG)

// The DEBUG version is not naked, to allow breakpoints at Reset_Handler
void __attribute__ ((section(".after_vectors"),noreturn))
Reset_Handler (void)
  {
    _start ();
  }

#else

// The Release version is optimised to a quick branch to _start.
void __attribute__ ((section(".after_vectors"),naked))
Reset_Handler(void)
{
  asm volatile
  (
      " b _start \n"
      :
      :
      :
  );
}

#endif

void __attribute__ ((section(".after_vectors"),weak))
NMI_Handler(void)
{
  while (1)
    {
    }
}

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
/* These are volatile to try and prevent the compiler/linker optimising them
away as the variables never actually get used.  If the debugger won't show the
values of the variables, make them global my moving their declaration outside
of this function. */
volatile uint32_t r0;
volatile uint32_t r1;
volatile uint32_t r2;
volatile uint32_t r3;
volatile uint32_t r12;
volatile uint32_t lr; /* Link register. */
volatile uint32_t pc; /* Program counter. */
volatile uint32_t psr;/* Program status register. */

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];

    /* When the following line is hit, the variables contain the register values. */
    for( ;; );
}

void __attribute__ ((section(".after_vectors"),weak))
HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n"
    );
}



#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

void __attribute__ ((section(".after_vectors"),weak))
MemManage_Handler(void)
  {
    while (1)
      {
      }
  }

void __attribute__ ((section(".after_vectors"),weak))
BusFault_Handler(void)
  {
    while (1)
      {
      }
  }

void __attribute__ ((section(".after_vectors"),weak))
UsageFault_Handler(void)
  {
    while (1)
      {
      }
  }

#endif

void __attribute__ ((section(".after_vectors"),weak))
SVC_Handler(void)
{
  while (1)
    {
    }
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

void __attribute__ ((section(".after_vectors"),weak))
DebugMon_Handler(void)
{
  while (1)
    {
    }
}

#endif

void __attribute__ ((section(".after_vectors"),weak))
PendSV_Handler(void)
{
  while (1)
    {
    }
}

void __attribute__ ((section(".after_vectors"),weak))
SysTick_Handler(void)
{
  while (1)
    {
    }
}

