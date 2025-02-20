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


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

SENT_DATA_TRANSMIT sentDataTransmit = {
    .status = 2,
    .data1 = 1,
    .data2 = 2,
    .data3 = 3,
    .data4 = 4,
    .data5 = 5,
    .data6 = 6
};

void sent_callback_tx(uintptr_t context){
    //LED4 on the curiosity board
    LED_Set();
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    //Registering a callback function to be called when transmission is complete
    SENT1_TransmitCompleteCallbackRegister(sent_callback_tx, (uintptr_t) NULL);
    //Setting the mode of operation of transmitter as Asynchronous
    SENT1_TransmitModeSet(SENT_TRANSMIT_ASYNCHRONOUS);
    //Enabling the module to start transmitting
    SENT1_Enable();
    //Transmitting data 
    SENT1_Transmit(&sentDataTransmit);
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

