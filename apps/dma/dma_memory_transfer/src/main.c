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
#include <string.h>                     // Defines strncmp
#include <stdio.h>
#include "definitions.h"                // SYS function prototypes


/* Macro definitions */
#define LED_On()      LED_Set()
#define LED_Off()     LED_Clear()

#define TRANSFER_SIZE 100

uint8_t srcBuffer[TRANSFER_SIZE] = {0};
uint8_t dstBuffer[TRANSFER_SIZE] = {0};

volatile bool dmaXferDone = false;

void DMA_EventHandler(DMA_TRANSFER_EVENT status, uintptr_t context)
{
    if(status & DMA_TRANSFER_EVENT_COMPLETE)
    {
        dmaXferDone = true;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint32_t i = 0;

    /* Initialize all modules */
    SYS_Initialize ( NULL );
    LED_Off();

    /* Build the srcBuffer */
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        srcBuffer[i] = i;
    }
    
    printf("\n\r-------------------------------------------------------------");
    printf("\n\r\t\t DMA Memory Transfer DEMO\t\t");
    printf("\n\r-------------------------------------------------------------");
    
    /* Register a callback with DMA PLIB to get transfer complete and error events. */
    DMA_ChannelCallbackRegister(DMA_CHANNEL_0, DMA_EventHandler, 0);

    DMA_ChannelTransfer(DMA_CHANNEL_0, srcBuffer, dstBuffer, TRANSFER_SIZE);
    
    /* enable the software trigger to start the transfer. */
    DMA_ChannelSoftwareTriggerEnable(DMA_CHANNEL_0);
    
    while (dmaXferDone == false);
    
    if(dmaXferDone == true)
    {
        dmaXferDone = false;

        if(memcmp(srcBuffer, dstBuffer, TRANSFER_SIZE) == 0)
        {
            /* Successfully transferred the data using DMAC */
            printf("\n\r DMA Memory Transfer Successful with Data Match\n\r");                    
            LED_On();
        }
        else
        {
            /* Data transfers done, but data mismatch occurred */
            printf("\n\r DMA Memory Transfer Successful with Data Mismatch !!!\n\r");                    
            LED_Off();
        }
    }

    while (1)
    {
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

