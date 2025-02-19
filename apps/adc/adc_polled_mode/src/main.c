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

#define ADC_VREF                (3.3f)
#define ADC_MAX_COUNT           (4096U)

static uint16_t adc_count;
static float input_voltage;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    printf("\n\r---------------------------------------------------------");
    printf("\n\r                    ADC Polled Demo                 ");
    printf("\n\r---------------------------------------------------------\n\r");
    
    while (1)
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

        /* Auto sampling mode is used, so no code is needed to start sampling */
		
        /* Start ADC conversion in software */
        ADC1_ChannelSoftwareTriggerEnable(ADC1_CHANNEL0);

        /* Wait till ADC conversion result is available */
        while(!ADC1_ChannelResultIsReady(ADC1_CHANNEL0))
        {

        };

        /* Read the ADC result */
        adc_count = ADC1_ChannelResultGet(ADC1_CHANNEL0);
        input_voltage = (float)adc_count * ADC_VREF / ADC_MAX_COUNT;

        printf("ADC Count = 0x%03x, ADC Input Voltage = %d.%02d V \r", adc_count, (int)input_voltage, (int)((input_voltage - (int)input_voltage)*100.0));
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/
