/*******************************************************************************
  Clock PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_clk_common.h
 
  Summary:
    Clock Common Header File
 
  Description:
    This file has prototype of all the interfaces which are common for all the
    Clock peripherals.
 
*******************************************************************************/
 
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

#ifndef PLIB_CLK_COMMON_H
#define PLIB_CLK_COMMON_H

// /cond IGNORE_THIS
/* Provide C++ Compatibility */
#ifdef __cplusplus

    extern "C" {

#endif
// /endcond

// Section: Included Files

#include <stdint.h>

typedef enum
{
    CLOCK_GENERATOR_1 = 1,    
    CLOCK_GENERATOR_2 = 2,    
    CLOCK_GENERATOR_3 = 3,    
    CLOCK_GENERATOR_12 = 12,    
    
    CLOCK_SYSTEM = 1,
    CLOCK_FRC = 2,
    CLOCK_WDT = 3,
    CLOCK_BFRC = 3,
    CLOCK_CCP = 12,
    CLOCK_REFO1 = 12

} CLOCK_GENERATOR;

typedef enum
{
    CLOCK_GEN1_FAIL_MASK = 0x1,   
    CLOCK_GEN2_FAIL_MASK = 0x2,   
    CLOCK_GEN3_FAIL_MASK = 0x4,   
    CLOCK_GEN12_FAIL_MASK = 0x800,   
    CLOCK_PLL1_FAIL_MASK = 0x1000000,    
} CLOCK_FAIL_STATUS_MASKS;

/** 
  @brief    Clock-Fail callback function prototype
*/
typedef void (* CLOCK_CombinedFailCallback)(uintptr_t context);

// /cond IGNORE_THIS
// Section: Local Objects **** Do Not Use ****

typedef struct
{
    CLOCK_CombinedFailCallback                callback;
    uintptr_t                                 context;
} CLOCK_CombinedFailObject;

// /endcond

// /cond IGNORE_THIS
/* Provide C++ Compatibility */
#ifdef __cplusplus

    }

#endif
// /endcond

#endif // PLIB_CLK_COMMON_H

/*******************************************************************************
 End of File
*/


