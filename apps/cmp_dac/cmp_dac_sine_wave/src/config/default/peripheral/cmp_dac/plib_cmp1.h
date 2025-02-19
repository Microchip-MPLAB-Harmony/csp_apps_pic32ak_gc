/*******************************************************************************
  cmp1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_cmp1.h
 
  Summary:
    cmp1 PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    cmp1 peripheral.
 
*******************************************************************************/
 
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_CMP1_H
#define PLIB_CMP1_H

// Section: Included Files

#include <stdbool.h>
#include <stdint.h>
#include "device.h"
#include "plib_cmp_common.h"

// Section: Data Type Definitions

/**
 * @brief    This macro defines input clock for CMP1 
 */
#define CMP1_DAC_CLOCK_FREQUENCY 200000000UL

// Section: CMP1 Module APIs

/**
 * @brief    Initialize the CMP1 module
 * @param    none
 * @return   none  
 */
void CMP1_Initialize(void);

/**
 * @brief    Deinitializes the CMP1 to POR values
 * @param    none
 * @return   none  
 */
void CMP1_Deinitialize(void);

/**
 * @brief    Calibrates the CMP1 module and enables ripple reduction mode
 * @param    none
 * @return   none  
 */
void CMP1_Calibrate(void);
 
/**
 * @brief    This inline function returns the comparator output status 
 * @param    none
 * @return   true   - Comparator output is high
 * @return   false  - Comparator output is low
 */
inline static bool CMP1_StatusGet(void)
{
    return (bool)(DAC1CONbits.CMPSTAT);
}

/**
 * @brief    This inline function enables the common DAC module
 * @param    none
 * @return   none 
 */
inline static void CMP1_Enable(void)
{
    DACCTRL1bits.ON = 1U;
}

/**
 * @brief    This inline function disables the common DAC module
 * @param    none
 * @return   none  
 */
inline static void CMP1_Disable(void)
{
    DACCTRL1bits.ON = 0U;
}

/**
 * @brief    This inline function enables the individual DAC module
 * @param    none
 * @return   none 
 */
inline static void CMP1_DACEnable(void)
{
    DAC1CONbits.DACEN = 1U;
}

/**
 * @brief    This inline function disables the individual DAC module
 * @param    none
 * @return   none
 */
inline static void CMP1_DACDisable(void)
{
    DAC1CONbits.DACEN = 0U;
}

/**
 * @brief      This inline function writes DAC data to register
 * @param[in]  value - DAC Data write value
 * @return     none 
 */
inline static void CMP1_DACDataWrite(size_t value)
{
    DAC1DATbits.DACDAT = value;
}


#endif //PLIB_CMP1_H

/**
  End of File
*/

