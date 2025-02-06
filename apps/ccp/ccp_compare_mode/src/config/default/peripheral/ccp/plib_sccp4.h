/*******************************************************************************
  Capture/Compare/PWM/Timer Module (CCP) Peripheral Library Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_sccp4.h

  Summary:
    CCP PLIB Header File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_SCCP4_H
#define PLIB_SCCP4_H

#include <stddef.h>
#include <stdint.h>
#include "device.h"
#include "plib_ccp_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END


// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

/*************************** SCCP4 API ****************************************/
// *****************************************************************************
/* Function:
   void SCCP4_CompareInitialize (void)

  Summary:
    Initialization function SCCP4 peripheral

  Description:
    This function initializes the SCCP4 peripheral with user input
	from the configurator.

  Parameters:
    void

  Returns:
    void
*/
void SCCP4_CompareInitialize (void);

// *****************************************************************************
/* Function:
   void SCCP4_CompareStart (void)

  Summary:
    Enable function SCCP4 peripheral

  Description:
    This function enables the SCCP4 peripheral

  Parameters:
    void

  Returns:
    void
*/
void SCCP4_CompareStart (void);

// *****************************************************************************
/* Function:
   void SCCP4_CompareStop (void)

  Summary:
    Disable function SCCP4 peripheral

  Description:
    This function disables the SCCP4 peripheral.

  Parameters:
    void

  Returns:
    void
*/
void SCCP4_CompareStop (void);

void SCCP4_CompareAutoShutdownClear (void);

void SCCP4_CompareAutoShutdownSet (void);


void SCCP4_Compare16bitValueSet (uint16_t value);

uint16_t SCCP4_Compare16bitValueGet (void);

void SCCP4_Compare16bitPeriodValueSet (uint16_t value);

uint16_t SCCP4_Compare16bitPeriodValueGet (void);



// *****************************************************************************
/* Function:
  void SCCP4_TimerCallbackRegister( CCP_TIMER_CALLBACK callback, uintptr_t context )

  Summary:
    Sets the callback function for a ocmp interrupt.

  Description:
    This function sets the callback function that will be called when the timer overflows.

  Precondition:
    None.

  Parameters:
    *callback   - a pointer to the function to be called when value is reached.
                  Use NULL to Un Register the timer callback

    context     - a pointer to user defined data to be used when the callback
                  function is called. NULL can be passed in if no data needed.

  Returns:
    void
*/
void SCCP4_TimerCallbackRegister(CCP_TIMER_CALLBACK callback, uintptr_t context);


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

// DOM-IGNORE-END
#endif // _PLIB_SCCP4_H
