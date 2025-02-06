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

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include "definitions.h"                // SYS function prototypes

/* Macro definitions */
#define LED_ON      LED0_Set
#define LED_OFF     LED0_Clear

#define SW_CRC_TRANSFER_SIZE        (9)
#define HW_CRC_TRANSFER_SIZE        (SW_CRC_TRANSFER_SIZE)

/* Expected CRC32 With Below Configurations and Input String : 0xCBF43926 */
#define POLYNOMIAL_LEN              32
#define CRC32_POLYNOMIAL            0x04C11DB7
#define CRC32_DIRECT_SEED           0xFFFFFFFF
#define CRC32_FINAL_XOR             0xFFFFFFFF
#define CRC32_REVERSE_INPUT         true
#define CRC32_REVERSE_OUTPUT        true
#define CRC32_ALTERNATE_MODE        true

/* Input String for Software/Hardware CRC    : 123456789 */
const static uint8_t srcBuffer[HW_CRC_TRANSFER_SIZE] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

static uint32_t CACHE_ALIGN reverse_table[256] = {0};

static uint32_t APP_bitReverse( uint32_t num, uint32_t bits)
{
    uint32_t out = 0;
    uint32_t i;

    for( i = 0; i < bits; i++ )
    {
        out <<= 1;

        if( num & 1 )
        {
            out |= 1;
        }

        num >>= 1;
    }

    return out;
}

static void APP_ReverseTabGen( void )
{
    uint32_t byte;

    for(  byte = 0; byte < 256; byte++ )
        reverse_table[ byte ] = (APP_bitReverse(byte, POLYNOMIAL_LEN) >> 24) & 0xFF;
}

static uint32_t APP_SWCRCGenerate(void)
{
    uint32_t   i, j, value;
    uint32_t   crc_tab[256];
    uint32_t   crc     = CRC32_DIRECT_SEED;
    uint8_t    data;

    if (CRC32_REVERSE_INPUT == true)
    {
        APP_ReverseTabGen();
    }

    for (i = 0; i < 256; i++)
    {
        value = (i << 24);

        for (j = 0; j < 8; j++)
        {
            if (value & (1 << (POLYNOMIAL_LEN - 1)))
            {
                value = (value << 1) ^ CRC32_POLYNOMIAL;
            }
            else
            {
                value <<= 1;
            }
        }
        crc_tab[i] = value;
    }

    for (i = 0; i < SW_CRC_TRANSFER_SIZE; i++)
    {
        data = srcBuffer[i];

        if (CRC32_REVERSE_INPUT == true)
        {
            data = reverse_table[(uint32_t)data];
        }

        crc = crc_tab[(data ^ (crc >> 24)) & 0xFF] ^ ((crc << 8));
    }

    if (CRC32_REVERSE_OUTPUT == true)
    {
        crc = APP_bitReverse(crc, POLYNOMIAL_LEN);
    }

    crc = (crc ^ CRC32_FINAL_XOR);

    return crc;
}

void APP_CRCConfigure( void )
{
    CRC_SETUP CRCSetup = {0};

    CRCSetup.reverse_crc_input  = CRC32_REVERSE_INPUT;
    CRCSetup.polynomial_length  = POLYNOMIAL_LEN;
    CRCSetup.polynomial         = CRC32_POLYNOMIAL;
    CRCSetup.reverse_crc_output = CRC32_REVERSE_OUTPUT;
    CRCSetup.final_xor_value    = CRC32_FINAL_XOR;

    CRC_CRCSetup(CRCSetup);
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint32_t sw_crc_32  = 0;
    uint32_t hw_crc_32 = 0;

    /* Initialize all modules */
    SYS_Initialize ( NULL );

    LED_OFF();

    printf("\r\n-------------------------------------------------------------");
    printf("\r\n\t\t CRC-32 Generator DEMO\t\t");
    printf("\r\n-------------------------------------------------------------");
    
    /* Generate Software CRC32 */
    sw_crc_32 = APP_SWCRCGenerate();

    /* Setup CRC */
    APP_CRCConfigure();

    /* Generate the CRC32 on source buffer */
    hw_crc_32 = CRC_CRCCalculate((void *)srcBuffer, HW_CRC_TRANSFER_SIZE, CRC32_DIRECT_SEED);

    if (hw_crc_32 == sw_crc_32)
    {
        /* Successfully transferred the data using DMAC */
        printf("\r\n CRC-32 Match : \r\n\n");
        LED_ON();
    }
    else
    {
        printf("\r\n CRC-32 MisMatch : \r\n\n");

    }

    printf( "\tPolynomial       = 0x%lx \r\n\n\t"
            "Hardware CRC-32  = 0x%lx \r\n\n\t"
            "Software CRC-32  = 0x%lx\r\n",
            (uint32_t)CRC32_POLYNOMIAL, hw_crc_32, sw_crc_32);

    while(true)
    {
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

