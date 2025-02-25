/*******************************************************************************
  INTC PLIB Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    plib_intc.c

  Summary:
    INTC PLIB Source File

  Description:
    None

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#include "plib_intc.h"
#include "interrupts.h"


void INTC_Initialize( void )
{
    /* Configure Interrupt priority */
    IPC6bits.T1IP = 1;


}

void INTC_SourceEnable( INT_SOURCE source )
{
    volatile uint32_t *IECx = (volatile uint32_t *)((uint32_t)&IEC0 + ((uint32_t)(0x10U * (source / 32U)) / 4U));
    *IECx |= 1UL << (source & 0x1fU);
}

void INTC_SourceDisable( INT_SOURCE source )
{
    volatile uint32_t *IECx = (volatile uint32_t *)((uint32_t)&IEC0 + ((uint32_t)(0x10U * (source / 32U)) / 4U));
    *IECx &= ~(1UL << (source & 0x1fU));
}

bool INTC_SourceIsEnabled( INT_SOURCE source )
{
    volatile uint32_t *IECx = (volatile uint32_t *)((uint32_t)&IEC0 + ((uint32_t)(0x10U * (source / 32U)) / 4U));

    return (((*IECx >> (source & 0x1fU)) & 0x01U) != 0U);
}

bool INTC_SourceStatusGet( INT_SOURCE source )
{
    volatile uint32_t *IFSx = (volatile uint32_t *)((uint32_t)&IFS0 + ((uint32_t)(0x10U * (source / 32U)) / 4U));

    return (((*IFSx >> (source & 0x1fU)) & 0x1U) != 0U);
}

void INTC_SourceStatusSet( INT_SOURCE source )
{
    volatile uint32_t *IFSx = (volatile uint32_t *) ((uint32_t)&IFS0 + ((uint32_t)(0x10U * (source / 32U)) / 4U));
    *IFSx |= 1UL << (source & 0x1fU);
}

void INTC_SourceStatusClear( INT_SOURCE source )
{
    volatile uint32_t *IFSx = (volatile uint32_t *) ((uint32_t)&IFS0 + ((uint32_t)(0x10U * (source / 32U)) / 4U));
    *IFSx &= ~(1UL << (source & 0x1fU));
}

void INTC_Enable( void )
{
    (void)__builtin_enable_interrupts();
}

bool INTC_Disable( void )
{
    bool processorStatus;
    
    /* Save the current processor status and then Disable the global interrupt */
    processorStatus = (INTCON1bits.GIE != 0U);
            
    (void)__builtin_disable_interrupts();

    /* return the processor status */
    return processorStatus;
}

void INTC_Restore( bool state )
{
    if (state)
    {
        /* restore the state of Global Interrupts before the disable occurred */
       (void)__builtin_enable_interrupts();
    }
}


/* End of file */