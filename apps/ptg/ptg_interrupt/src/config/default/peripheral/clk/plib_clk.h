/*******************************************************************************
  clock PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_clk.h
 
  Summary:
    Clock PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    Clock peripheral.
 
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

#ifndef PLIB_CLK_H
#define PLIB_CLK_H

// /cond IGNORE_THIS
/* Provide C++ Compatibility */
#ifdef __cplusplus

    extern "C" {

#endif
// /endcond

// Section: Included Files

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include "plib_clk_common.h"

// Section: CLOCK APIs

/**
 * @brief    Initializes all the CLOCK sources and clock switch configurations. 
 * @pre      none
 * @param    none
 * @return   none  
 */
void CLOCK_Initialize(void);

/**
 * @brief       This inline function returns clock generator frequency in Hz
 * @pre         none
 * @param[in]   generator - instance of clock generator
 * @return      Clock frequency in Hz
 */
inline static uint32_t CLOCK_GeneratorFrequencyGet(CLOCK_GENERATOR generator)
{
    uint32_t genFrequency = 0x0U;

    switch(generator)
    {
        case CLOCK_GENERATOR_1:
                genFrequency = 8000000UL;
                break;
        case CLOCK_GENERATOR_2:
                genFrequency = 8000000UL;
                break;
        case CLOCK_GENERATOR_3:
                genFrequency = 8000000UL;
                break;
        case CLOCK_GENERATOR_6:
                genFrequency = 8000000UL;
                break;
        case CLOCK_GENERATOR_10:
                genFrequency = 8000000UL;
                break;
        default:
                break;
    }
    return genFrequency;
}

/**
 * @brief       This inline function returns clock generator failure status.
 * @pre         none
 * @param       none
 * @return      32-bit status value. Use status masks in \ref CLOCK_FAIL_STATUS_MASKS to derive individual status.
 * @note        In interrupt mode this function has to be called inside  CombinedClockFailCallback 
 *              to know the status during failure event.
 */
inline static uint32_t CLOCK_FailStatusGet(void)
{
    uint32_t failStatus = (uint32_t)CLKFAIL;
    CLKFAIL =  0x0U;
    return failStatus;   
}


// /cond IGNORE_THIS
/* Provide C++ Compatibility */
#ifdef __cplusplus

    }

#endif
// /endcond

#endif // PLIB_CLK_H

/*******************************************************************************
 End of File
*/

