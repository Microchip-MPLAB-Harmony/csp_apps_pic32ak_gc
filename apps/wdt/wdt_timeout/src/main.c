/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project. The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all modules in the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h" 

#define SWITCH_PIN         GPIO_PIN_RA6
#define LED_Set()       GPIO_RC10_Set() 

volatile bool switch_pressed = false;
volatile bool timeoutExpired = false;

// Define the GPIO_PortToggle function
void switch_handler(GPIO_PIN pin, uintptr_t context)
{
    switch_pressed = true;
}

void TimeoutHandler(void)
{
    timeoutExpired = true;
}

// Corrected callback function to match the expected signature
void tmr_callback_fn(uint32_t status, uintptr_t context){
    LED_Set();
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void)
{
    /* Initialize all modules */
    SYS_Initialize(NULL);

    WDT_Enable();
    TMR1_CallbackRegister( tmr_callback_fn, (uintptr_t)NULL);
    TMR1_Start();
    GPIO_PinInterruptCallbackRegister(SWITCH_PIN, switch_handler, (uintptr_t)NULL);
    GPIO_PinIntEnable(SWITCH_PIN, GPIO_INTERRUPT_ON_MISMATCH);
    switch_pressed = false;

    while (true)
    {
        if (!switch_pressed)
        {
            if (timeoutExpired)
            {
                timeoutExpired = false;
                LED_Set(); // Toggle the LED
                WDT_Clear();
            }
        }
        else
        {
            switch_pressed = false;
            while (1); // Emulate deadlock
        }
    }

    /* Execution should not come here during normal operation */
    return (EXIT_FAILURE);
}

/*******************************************************************************
 End of File
*/
