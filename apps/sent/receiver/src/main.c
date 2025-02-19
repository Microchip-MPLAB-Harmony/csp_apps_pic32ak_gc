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
//Data to be transmitted
SENT_DATA_TRANSMIT sentDataTransmit = {
	.status = 2,
    .data1 = 1,
    .data2 = 2
};
//To store the received data
SENT_DATA_RECEIVE sentDataReceive;
//Function to compare the transmitted and received data
bool compare(SENT_DATA_TRANSMIT *txdata, SENT_DATA_RECEIVE *rxdata)
{
    bool status = true;
    if (txdata->status != rxdata->status) {
        status = false;
    }
    
    if (txdata->data1 != rxdata->data1) {
        status = false;
    }
    
    if (txdata->data2 != rxdata->data2) {
        status = false;
    }
    return status;
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
     //Enabling Transmitter
    SENT1_Enable();
    //Enabling Receiver
    SENT2_Enable();
    //Transmitting data
    SENT1_Transmit(&sentDataTransmit);
    while (SENT1_IsTransmissionComplete() == false);
    while (SENT2_IsDataReceived() == false);
    //Storing received data
    sentDataReceive = SENT2_Receive();
    //Comparing transmitted and received data
    if(compare(&sentDataTransmit, &sentDataReceive)){
        //LED4 
        LED2_Set();
    }
	else{
        //LED3
		LED1_Set();
	}
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

