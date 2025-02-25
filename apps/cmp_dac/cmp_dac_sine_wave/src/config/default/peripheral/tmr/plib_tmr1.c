
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

/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "stdbool.h"
#include "device.h"
#include "plib_tmr1.h"
#include "interrupts.h"

volatile static TIMER_OBJECT tmr1Obj;

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
    PR1 = 0xf41UL; /* Decimal Equivalent 3905 */

    tmr1Obj.tickCounter = 0;
    tmr1Obj.callback_fn = NULL;

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

uint32_t TMR1_GetTickCounter(void)
{
    return tmr1Obj.tickCounter;
}

void TMR1_StartTimeOut (TMR_TIMEOUT* timeout, uint32_t delay_ms)
{
    timeout->start = TMR1_GetTickCounter();
    timeout->count = (delay_ms * 1000U)/TMR_INTERRUPT_PERIOD_IN_US;
}

void TMR1_ResetTimeOut (TMR_TIMEOUT* timeout)
{
    timeout->start = TMR1_GetTickCounter();
}

bool TMR1_IsTimeoutReached (TMR_TIMEOUT* timeout)
{
    bool valTimeout  = true;
    if ((tmr1Obj.tickCounter - timeout->start) < timeout->count)
    {
        valTimeout = false;
    }

    return valTimeout;

}

void __attribute__((used)) T1_InterruptHandler (void)
{
    uint32_t status = _T1IF;
    _T1IF = 0;

    tmr1Obj.tickCounter++;

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
