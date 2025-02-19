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

static uint16_t adc_count;
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

void trigger0callback(uintptr_t context){
    LED_Set(); 
}
void trigger1callback(uintptr_t context){
    while(!ADC1_ChannelResultIsReady(ADC1_CHANNEL0));
    adc_count = ADC1_ChannelResultGet(ADC1_CHANNEL0); 
    printf("Post ADC conversion, the result is as follows\n\r");
    printf("ADC Count = 0x%03x", adc_count);
    
}
int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    printf("\n\r---------------------------------------------------------");
    printf("\n\r                    PTG Interrupt Demo                 ");
    printf("\n\r---------------------------------------------------------\n\r");
    printf("Press switch S1 to start the step sequence\n\r");
    //Register a callback for trigger0 event
    PTG_EventCallbackRegister(TRIGGER0, trigger0callback, (uintptr_t) NULL);
    //Register a callback for trigger1 event
    PTG_EventCallbackRegister(TRIGGER1, trigger1callback, (uintptr_t) NULL);
    //Enabling PTG
    PTG_Enable();
    //Starting the Step Sequence
    PTG_StepSequenceStart();
    
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

