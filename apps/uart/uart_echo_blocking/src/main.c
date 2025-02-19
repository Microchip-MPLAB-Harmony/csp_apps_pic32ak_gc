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

#define RX_BUFFER_SIZE 256
#define LED_On()                        LED_Set()
#define LED_Off()                       LED_Clear()

static char messageStart[] = "**** UART Line Echo Demo: Blocking Transfer without the interrupt ****\r\n\
**** Type a line of characters and press the Enter key. **** \r\n\
**** Entered line will be echoed back, and the LED is toggled. ****\r\n";
static char newline[] = "\r\n";
static char errorMessage[] = "\r\n**** UART error has occurred ****\r\n";
static char receiveBuffer[RX_BUFFER_SIZE] = {};
static char data = 0;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint8_t rxCounter=0;

    /* Initialize all modules */
    SYS_Initialize ( NULL );
    LED_Off();

    /* Send start message */
    UART3_Write(&messageStart, sizeof(messageStart));

    while ( true )
    {
        /* Check if there is a received character */
        if(UART3_ReceiverIsReady() == true)
        {
            if(UART3_ErrorGet() == UART_ERROR_NONE)
            {
                UART3_Read(&data, 1);

                if((data == '\n') || (data == '\r'))
                {
                    UART3_Write(newline,sizeof(newline));
                    UART3_Write(receiveBuffer,rxCounter);
                    UART3_Write(newline,sizeof(newline));
                    rxCounter = 0;
                    LED_Toggle();
                }
                else
                {
                    receiveBuffer[rxCounter++] = data;
                }
            }
            else
            {
                UART3_Write(errorMessage,sizeof(errorMessage));
            }
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

