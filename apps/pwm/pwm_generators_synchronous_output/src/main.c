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
/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

/***************************************
 * Check PWM outputs on pins
 * Generator 1 PWM1H - RC3 
 * Generator 2 PWM2H - RC4
 * Generator 3 PWM3H - RC5
 * Generator 4 PWM4H - RC6
 ***************************************/

/* Duty cycle increment value 10% */
#define DUTY_INCREMENT (1600U)

/* Initial PWM Generator 1 period for 100kHz */
#define PWM_PERIOD          (0x3E70U)

/* This function is called after PWM Generator 1 EOC event */
void PWM_EOCEventHandler(PWM_GENERATOR genNum, uintptr_t context)
{
    if(genNum == PWM_GENERATOR_1)
    {
        if(!PWM_SoftwareUpdatePending(PWM_GENERATOR_1)) 
        {
            /* duty cycle values */
            static uint32_t duty0 = 3200U;
            static uint32_t duty1 = 3200U;
            static uint32_t duty2 = 3200U;
            static uint32_t duty3 = 3200U;

            PWM_DutyCycleSet(PWM_GENERATOR_1, duty0);
            PWM_DutyCycleSet(PWM_GENERATOR_2, duty1);
            PWM_DutyCycleSet(PWM_GENERATOR_3, duty2);
            PWM_DutyCycleSet(PWM_GENERATOR_4, duty3);

            /* Updates the Duty Cycle 10% to All PWM Generators but Set update Request to only Host PWM generator(PG1) */
            PWM_SoftwareUpdateRequest(PWM_GENERATOR_1);

            /* Increment duty cycle values */
            duty0 += DUTY_INCREMENT;
            duty1 += DUTY_INCREMENT;
            duty2 += DUTY_INCREMENT;
            duty3 += DUTY_INCREMENT;

            if (duty0 > PWM_PERIOD)
                duty0 = 0U;
            if (duty1 > PWM_PERIOD)
                duty1 = 0U;
            if (duty2 > PWM_PERIOD)
                duty2 = 0U;
            if (duty3 > PWM_PERIOD)
                duty3 = 0U;      
        }
    }
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    PWM_EOCEventCallbackRegister(PWM_GENERATOR_1, PWM_EOCEventHandler, (uintptr_t)NULL);
    
    /* Start all synchronous channels by starting generator 1*/
    PWM_Enable();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

