/*******************************************************************************
 Configurable Logic Cell (CLC1) PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_clc1.c

  Summary:
    CLC1 PLIB Implementation file

  Description:
    This file defines the interface to the CDAC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_clc1.h"
#include <stddef.h>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: CLC1 Interface Routines
// *****************************************************************************
// *****************************************************************************
void CLC1_Initialize( void )
{
    /* Configure data sources */
    CLC1SEL =  (((0x4UL << _CLC1SEL_DS1_POSITION) & _CLC1SEL_DS1_MASK) |
                ((0x0UL << _CLC1SEL_DS2_POSITION) & _CLC1SEL_DS2_MASK) |
                ((0x3UL << _CLC1SEL_DS3_POSITION) & _CLC1SEL_DS3_MASK) |
                ((0x0UL << _CLC1SEL_DS4_POSITION) & _CLC1SEL_DS4_MASK));

    /* Configure gates */
    CLC1GLS =  (_CLC1GLS_G1D1T_MASK |
                _CLC1GLS_G3D3T_MASK);

    /* Configure logic cell */
    CLC1CON = (((0x1UL << _CLC1CON_MODE_POSITION) & _CLC1CON_MODE_MASK)
               | _CLC1CON_ON_MASK 
               | _CLC1CON_LCOE_MASK );

}

void CLC1_Enable(bool enable)
{
  if(enable == true)
  {
    CLC1CON |= _CLC1CON_ON_MASK;
  }
  else
  {
    CLC1CON &= ~(_CLC1CON_ON_MASK);
  }
}

