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
#include <string.h>
#include <stdio.h>
#include "definitions.h"                // SYS function prototypes

static uint8_t txBuffer[50];
static uint8_t rxBuffer[10];
volatile static uint32_t nBytesRead = 0;
volatile static bool txThresholdEventReceived = false;
volatile static bool rxThresholdEventReceived = false;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

void usartReadEventHandler(UART_EVENT event, uintptr_t context )
{
    uint32_t nBytesAvailable = 0;
    
    if (event == UART_EVENT_READ_THRESHOLD_REACHED)
    {
        /* Receiver should atleast have the thershold number of bytes in the ring buffer */
        nBytesAvailable = UART3_ReadCountGet();
        
        nBytesRead += UART3_Read((uint8_t*)&rxBuffer[nBytesRead], nBytesAvailable);                          
    }
}

void usartWriteEventHandler(UART_EVENT event, uintptr_t context )
{
    txThresholdEventReceived = true;
}

int main ( void )
{
    uint32_t nBytes = 0;        
    
    /* Initialize all modules */
    SYS_Initialize ( NULL );          
    
    /* Register a callback for write events */
    UART3_WriteCallbackRegister(usartWriteEventHandler, (uintptr_t) NULL);
    
    /* Register a callback for read events */
    UART3_ReadCallbackRegister(usartReadEventHandler, (uintptr_t) NULL);              
    
    /* Print the size of the read buffer on the terminal */
    nBytes = sprintf((char*)txBuffer, "RX Buffer Size = %d\r\n", (int)UART3_ReadBufferSizeGet());
    
    UART3_Write((uint8_t*)txBuffer, nBytes);  
    
    /* Print the size of the write buffer on the terminal */
    nBytes = sprintf((char*)txBuffer, "TX Buffer Size = %d\r\n", (int)UART3_WriteBufferSizeGet());
    
    UART3_Write((uint8_t*)txBuffer, nBytes);    
    
    UART3_Write((uint8_t*)"Adding 10 characters to the TX buffer - ", sizeof("Adding 10 characters to the TX buffer - "));    
    
    /* Wait for all bytes to be transmitted out */
    while (UART3_WriteCountGet() != 0);    
    
    UART3_Write((uint8_t*)"0123456789", 10);           
        
    /* Print the amount of free space available in the TX buffer. This should be 10 bytes less than the configured write buffer size. */
    nBytes = sprintf((char*)txBuffer, "\r\nFree Space in Transmit Buffer = %d\r\n", (int)UART3_WriteFreeBufferCountGet());

    UART3_Write((uint8_t*)txBuffer, nBytes);    
    
    /* Let's enable notifications to get notified when the TX buffer is empty */
    UART3_WriteThresholdSet(UART3_WriteBufferSizeGet());   
    
    /* Enable notifications */
    UART3_WriteNotificationEnable(true, false);
   
    /* Wait for the TX buffer to become empty. Flag "txThresholdEventReceived" is set in the callback. */
    while (txThresholdEventReceived == false);
    
    txThresholdEventReceived = false;    
    
    /* Disable TX notifications */
    UART3_WriteNotificationEnable(false, false);
    
    UART3_Write((uint8_t*)"Enter 10 characters. The received characters are echoed back. \r\n>", sizeof("Enter 10 characters. The received characters are echoed back. \r\n>"));               
            
    /* Wait till 10 (or more) characters are received */
    while (UART3_ReadCountGet() < 10);
    
    /* At-least 10 characters are available in the RX buffer. Read out into the application buffer */
    UART3_Read((uint8_t*)rxBuffer, 10);  
    
    /* Echo the received data */
    UART3_Write((uint8_t*)rxBuffer, 10);    
    
    /* Now demonstrating receiver notifications */
    UART3_Write((uint8_t*)"\r\n Now turning on RX notifications \r\n>", sizeof("\r\n Now turning on RX notifications \r\n>"));
    
    /* For demonstration purpose, set a threshold value to receive a callback after every 5 characters are received */
    UART3_ReadThresholdSet(5);
    
    /* Enable RX event notifications */
    UART3_ReadNotificationEnable(true, false);
                   
    while(1)
    {
        /* Wait until at-least 10 characters are entered by the user */
        while (nBytesRead < 10);    
    
        /* Echo the received data */
        UART3_Write((uint8_t*)rxBuffer, nBytesRead);
        
        UART3_Write((uint8_t*)"\r\n>", 3);

        nBytesRead = 0;
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

