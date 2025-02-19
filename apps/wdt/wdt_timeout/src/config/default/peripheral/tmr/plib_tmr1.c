
/*******************************************************************************
  TMR Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_tmr1.c

  Summary
    TMR1 peripheral library source file.

  Description
    This file implements the interface to the TMR peripheral library.  This
    library provides access to and control of the associated peripheral
    instance.

*/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "stdbool.h"
#include "device.h"
#include "plib_tmr1.h"
#include "interrupts.h"

volatile static TIMER_OBJECT tmr1Obj;
void T1_InterruptHandler (void);

// Section: Macro Definitions

//Timer Pre-Scalar options
#define T1CON_TCKPS_1_1      ((uint32_t)(_T1CON_TCKPS_MASK & ((uint32_t)(0) << _T1CON_TCKPS_POSITION))) 
#define T1CON_TCKPS_1_8      ((uint32_t)(_T1CON_TCKPS_MASK & ((uint32_t)(1) << _T1CON_TCKPS_POSITION))) 
#define T1CON_TCKPS_1_64      ((uint32_t)(_T1CON_TCKPS_MASK & ((uint32_t)(2) << _T1CON_TCKPS_POSITION))) 
#define T1CON_TCKPS_1_256      ((uint32_t)(_T1CON_TCKPS_MASK & ((uint32_t)(3) << _T1CON_TCKPS_POSITION))) 

//Clock selection options
#define T1CON_SRC_SEL_STANDARD      ((uint32_t)(_T1CON_TCS_MASK & ((uint32_t)(0) << _T1CON_TCS_POSITION))) 
#define T1CON_SRC_SEL_EXTERNAL      ((uint32_t)(_T1CON_TCS_MASK & ((uint32_t)(1) << _T1CON_TCS_POSITION))) 

void TMR1_Initialize(void)
{
    /* Disable Timer */
    T1CONbits.ON = 0;

	T1CON = 0x0UL;
    /* Clear counter */
    TMR1 = 0x0UL;

    /*Set period */
    PR1 = 0x2faf080UL; /* Decimal Equivalent 50000000 */

    /* Setup TMR1 Interrupt */
    TMR1_InterruptEnable();  /* Enable interrupt on the way out */
}

void TMR1_Deinitialize(void)
{
	/* Stopping the timer */
	TMR1_Stop();
	
	/* Deinitializing the registers to POR values */
	T1CON = 0x0UL;
	TMR1  = 0x0UL;
	PR1   = 0xFFFFFFFFUL;
}
	
void TMR1_Start (void)
{
    T1CONbits.ON = 1;
}

void TMR1_Stop (void)
{
    T1CONbits.ON = 0;
}


void TMR1_PeriodSet(uint32_t period)
{
    PR1 = period;
}


uint32_t TMR1_PeriodGet(void)
{
    return PR1;
}


uint32_t TMR1_CounterGet(void)
{
    return TMR1;
}

uint32_t TMR1_FrequencyGet(void)
{
    return TIMER_CLOCK_FREQUENCY;
}

void __attribute__((used)) T1_InterruptHandler (void)
{
    uint32_t status = _T1IF;
    _T1IF = 0;

    if((tmr1Obj.callback_fn != NULL))
    {
        uintptr_t context = tmr1Obj.context;
        tmr1Obj.callback_fn(status, context);
    }
}


void TMR1_InterruptEnable(void)
{
    _T1IE = 1;
}


void TMR1_InterruptDisable(void)
{
     _T1IE = 0;
}


void TMR1_CallbackRegister( TMR_CALLBACK callback_fn, uintptr_t context )
{
    /* - Save callback_fn and context in local memory */
    tmr1Obj.callback_fn = callback_fn;
    tmr1Obj.context = context;
}
