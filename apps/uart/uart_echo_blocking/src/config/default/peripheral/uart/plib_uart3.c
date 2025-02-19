/*******************************************************************************
  uart3 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_uart3.c
 
  Summary:
    uart3 PLIB Source File
 
  Description:
    None
 
*******************************************************************************/
 
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
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


// Section: Included Files

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "device.h"
#include "interrupts.h"
#include "plib_uart3.h"


// Section: Macro Definitions

//UART UxCON MODE options
#define U3CON_MODE_8_BIT_NONE          ((uint32_t)(_U3CON_MODE_MASK & ((uint32_t)(0) << _U3CON_MODE_POSITION))) 
#define U3CON_MODE_7_BIT          ((uint32_t)(_U3CON_MODE_MASK & ((uint32_t)(1) << _U3CON_MODE_POSITION))) 
#define U3CON_MODE_8_BIT_ODD          ((uint32_t)(_U3CON_MODE_MASK & ((uint32_t)(2) << _U3CON_MODE_POSITION))) 
#define U3CON_MODE_8_BIT_EVEN          ((uint32_t)(_U3CON_MODE_MASK & ((uint32_t)(3) << _U3CON_MODE_POSITION))) 

//UART UxCON STP options
#define U3CON_STP_1_SENT_1_RECEIVE           ((uint32_t)(_U3CON_STP_MASK & ((uint32_t)(0) << _U3CON_STP_POSITION))) 
#define U3CON_STP_1_5_SENT_1_5_RECEIVE           ((uint32_t)(_U3CON_STP_MASK & ((uint32_t)(1) << _U3CON_STP_POSITION))) 
#define U3CON_STP_2_SENT_2_RECEIVE           ((uint32_t)(_U3CON_STP_MASK & ((uint32_t)(2) << _U3CON_STP_POSITION))) 
#define U3CON_STP_2_SENT_1_RECEIVE           ((uint32_t)(_U3CON_STP_MASK & ((uint32_t)(3) << _U3CON_STP_POSITION))) 

//UART UxCON CLKSEL options
#define U3CON_CLKSEL_UPB_CLOCK        ((uint32_t)(_U3CON_CLKSEL_MASK & ((uint32_t)(0) << _U3CON_CLKSEL_POSITION))) 
#define U3CON_CLKSEL_CLOCK_GEN_8        ((uint32_t)(_U3CON_CLKSEL_MASK & ((uint32_t)(1) << _U3CON_CLKSEL_POSITION))) 

//UART UxCON FLO options
#define U3CON_FLO_NONE        ((uint32_t)(_U3CON_FLO_MASK & ((uint32_t)(0) << _U3CON_FLO_POSITION))) 

#define UART_MAX_BAUD 0xFFFFFUL
#define UART_MIN_FRACTIONAL_BAUD 16U


// Section: UART3 Implementation

void static UART3_ErrorClear( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint8_t dummyData = 0u;

    errors = (UART_ERROR)(U3STAT & (_U3STAT_RXFOIF_MASK | _U3STAT_FERIF_MASK | _U3STAT_PERIF_MASK));

    if(errors != UART_ERROR_NONE)
    {
        /* If it's a overrun error then clear it to flush FIFO */
        if(U3STATbits.RXFOIF != 0U)
        {
            U3STATbits.RXFOIF = 0U;
        }

        /* Read existing error bytes from FIFO to clear parity and framing error flags */
        while(U3STATbits.RXBE != 1U)
        {
            dummyData = (uint8_t)U3RXB;
        }

    }

    // Ignore the warning
    (void)dummyData;
}

void UART3_Initialize( void )
{
    /*
    Baud Rate:      115207.373
    */
    U3CON = (U3CON_MODE_8_BIT_NONE
            |U3CON_STP_1_SENT_1_RECEIVE
            |_U3CON_CLKMOD_MASK
            |U3CON_FLO_NONE
            |U3CON_CLKSEL_UPB_CLOCK);

    /* BAUD Rate register Setup */
    U3BRG = 0x364;

    /* Turn ON UART3 */
    U3CON |= (_U3CON_ON_MASK
                 |_U3CON_TXEN_MASK
                 |_U3CON_RXEN_MASK);
}

bool UART3_SerialSetup(UART_SERIAL_SETUP *setup, uint32_t srcClkFreq)
{
    bool status = false;
    uint32_t baud;
    uint32_t ctrlReg;
    uint32_t uxbrg;

    if (setup != NULL)
    {
        baud = setup->baudRate;

        if ((baud == 0U) || ((setup->dataWidth == UART_DATA_7_BIT) && (setup->parity != UART_PARITY_NONE)))
        {
            return status;
        }


        srcClkFreq = UART3_FrequencyGet();

        
        /* Turn OFF UART3. Save UTXEN, URXEN bits as these are cleared upon disabling UART */
        ctrlReg = U3CON & (_U3CON_TXEN_MASK | _U3CON_RXEN_MASK );
        U3CONbits.ON = 0U;
        
        /* Calculate BRG value in fractional mode as it has least error rate */
        uxbrg = (srcClkFreq/baud);
        /* Check if the valid baud value is set */
        if(uxbrg < UART_MIN_FRACTIONAL_BAUD)
        {
            /* Baud rate cannot be achieved with current clock source value */
            return status;
        }
        else if(uxbrg > UART_MAX_BAUD)
        {
            /* Calculate BRG value for high speed mode*/
            uxbrg = (srcClkFreq/(4U*baud)) - 1U;
            U3CONbits.BRGS = 1U;
            
            if(uxbrg > UART_MAX_BAUD)
            {
                /* Calculate BRG value for low speed mode*/
                uxbrg = (srcClkFreq/(16U*baud)) - 1U;
                U3CONbits.BRGS = 0U;
                if(uxbrg > UART_MAX_BAUD)
                {
                    /* Baud rate cannot be achieved with current clock source value */
                    return status;
                }
            }
        }
        else
        {
            U3CONbits.CLKMOD = 1U;
        }
        
        if(setup->dataWidth == UART_DATA_8_BIT)
        {
            /* Configure UART3 mode with parity if mode is 8 bit */
            U3CONbits.MODE = setup->parity;
        }
        else
        {
            /* Configure UART3 mode to 7 bit */
            U3CONbits.MODE = setup->dataWidth;
        }

        /* Configure UART3 mode */
        U3CONbits.STP = setup->stopBits;

        /* Configure UART3 Baud Rate */
        U3BRG = uxbrg;

        /* Restore UTXEN, URXEN bits. */
        U3CON |= ctrlReg;
        
        U3CONbits.ON = 1U;

        status = true;
    }

    return status;
}

bool UART3_Read(void* buffer, const size_t size)
{
    bool status = false;
    uint32_t errorStatus;
    size_t processedSize = 0;

    if(buffer != NULL)
    {

        /* Clear error flags and flush out error data that may have been received when no active request was pending */
        UART3_ErrorClear();

        while( size > processedSize )
        {
            while((U3STATbits.RXBE) != 0U)
            {
                /* Wait for receiver to be ready */
            }

            /* Error status */
            errorStatus = (U3STAT & (_U3STAT_RXFOIF_MASK | _U3STAT_FERIF_MASK | _U3STAT_PERIF_MASK));

            if(errorStatus != 0U)
            {
                break;
            }
            
            /* 8-bit and 7-bit mode */
            ((uint8_t*)(buffer))[processedSize] = (uint8_t)(U3RXB);
            processedSize++;
        }

        if(size == processedSize)
        {
            status = true;
        }
    }

    return status;
}

bool UART3_Write( void* buffer, const size_t size )
{
    bool status = false;
    size_t processedSize = 0;

    if(buffer != NULL)
    {
        while( size > processedSize )
        {
            /* Wait while TX buffer is full */
            while (U3STATbits.TXBF != 0U)
            {
                /* Wait for transmitter to be ready */
            }

            /* 7-bit and 8-bit mode */
            U3TXB = ((uint8_t*)(buffer))[processedSize];

            processedSize++;
        }

        status = true;
    }

    return status;
}

UART_ERROR UART3_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;

    errors = (U3STAT & (_U3STAT_RXFOIF_MASK | _U3STAT_FERIF_MASK | _U3STAT_PERIF_MASK));

    if(errors != UART_ERROR_NONE)
    {
        UART3_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool UART3_AutoBaudQuery( void )
{
    bool autobaudcheck = false;
    if(U3CONbits.ABDEN != 0U)
    {

      autobaudcheck = true;

    }
    return autobaudcheck;
}

void UART3_AutoBaudSet( bool enable )
{
    if( enable == true )
    {
        U3CONbits.ABDEN =  1U;
    }
}


int UART3_ReadByte( void )
{
    return(int)(U3RXB);
}

bool UART3_ReceiverIsReady( void )
{
    return (bool)(U3STATbits.RXBE == 0U);
}

void UART3_WriteByte( int data )
{
    while (U3STATbits.TXBF != 0U)
    {
        /* Do Nothing */
    }

    U3TXB = (uint32_t)data;
}

bool UART3_TransmitterIsReady( void )
{
    return (bool)(U3STATbits.TXBF == 0U);
}

bool UART3_TransmitComplete( void )
{
    bool transmitComplete = false;

    if(U3STATbits.TXMTIF != 0U)
    {
        transmitComplete = true;
    }

    return transmitComplete;
}