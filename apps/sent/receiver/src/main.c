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
        //LED4 on the curiosity board
        LED2_Set();
    }
	else{
        //LED3 on the curiosity board
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

