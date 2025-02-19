/*******************************************************************************
  uart1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_uart1.c
 
  Summary:
    uart1 PLIB Source File
 
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
#include "plib_uart1.h"


// Section: Macro Definitions

//UART UxCON MODE options
#define U1CON_MODE_8_BIT_NONE          ((uint32_t)(_U1CON_MODE_MASK & ((uint32_t)(0) << _U1CON_MODE_POSITION))) 
#define U1CON_MODE_7_BIT          ((uint32_t)(_U1CON_MODE_MASK & ((uint32_t)(1) << _U1CON_MODE_POSITION))) 
#define U1CON_MODE_8_BIT_ODD          ((uint32_t)(_U1CON_MODE_MASK & ((uint32_t)(2) << _U1CON_MODE_POSITION))) 
#define U1CON_MODE_8_BIT_EVEN          ((uint32_t)(_U1CON_MODE_MASK & ((uint32_t)(3) << _U1CON_MODE_POSITION))) 

//UART UxCON STP options
#define U1CON_STP_1_SENT_1_RECEIVE           ((uint32_t)(_U1CON_STP_MASK & ((uint32_t)(0) << _U1CON_STP_POSITION))) 
#define U1CON_STP_1_5_SENT_1_5_RECEIVE           ((uint32_t)(_U1CON_STP_MASK & ((uint32_t)(1) << _U1CON_STP_POSITION))) 
#define U1CON_STP_2_SENT_2_RECEIVE           ((uint32_t)(_U1CON_STP_MASK & ((uint32_t)(2) << _U1CON_STP_POSITION))) 
#define U1CON_STP_2_SENT_1_RECEIVE           ((uint32_t)(_U1CON_STP_MASK & ((uint32_t)(3) << _U1CON_STP_POSITION))) 

//UART UxCON CLKSEL options
#define U1CON_CLKSEL_UPB_CLOCK        ((uint32_t)(_U1CON_CLKSEL_MASK & ((uint32_t)(0) << _U1CON_CLKSEL_POSITION))) 
#define U1CON_CLKSEL_CLOCK_GEN_8        ((uint32_t)(_U1CON_CLKSEL_MASK & ((uint32_t)(1) << _U1CON_CLKSEL_POSITION))) 

//UART UxCON FLO options
#define U1CON_FLO_NONE        ((uint32_t)(_U1CON_FLO_MASK & ((uint32_t)(0) << _U1CON_FLO_POSITION))) 

#define UART_MAX_BAUD 0xFFFFFUL
#define UART_MIN_FRACTIONAL_BAUD 16U


// Section: UART1 Implementation

void static UART1_ErrorClear( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint8_t dummyData = 0u;

    errors = (UART_ERROR)(U1STAT & (_U1STAT_RXFOIF_MASK | _U1STAT_FERIF_MASK | _U1STAT_PERIF_MASK));

    if(errors != UART_ERROR_NONE)
    {
        /* If it's a overrun error then clear it to flush FIFO */
        if(U1STATbits.RXFOIF != 0U)
        {
            U1STATbits.RXFOIF = 0U;
        }

        /* Read existing error bytes from FIFO to clear parity and framing error flags */
        while(U1STATbits.RXBE != 1U)
        {
            dummyData = (uint8_t)U1RXB;
        }

    }

    // Ignore the warning
    (void)dummyData;
}

void UART1_Initialize( void )
{
    /*
    Baud Rate:      114285.714
    */
    U1CON = (U1CON_MODE_8_BIT_NONE
            |U1CON_STP_1_SENT_1_RECEIVE
            |_U1CON_CLKMOD_MASK
            |U1CON_FLO_NONE
            |U1CON_CLKSEL_UPB_CLOCK);

    /* BAUD Rate register Setup */
    U1BRG = 0x23;

    /* Turn ON UART1 */
    U1CON |= (_U1CON_ON_MASK
                 |_U1CON_TXEN_MASK
                 |_U1CON_RXEN_MASK);
}

bool UART1_SerialSetup(UART_SERIAL_SETUP *setup, uint32_t srcClkFreq)
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


        srcClkFreq = UART1_FrequencyGet();

        
        /* Turn OFF UART1. Save UTXEN, URXEN bits as these are cleared upon disabling UART */
        ctrlReg = U1CON & (_U1CON_TXEN_MASK | _U1CON_RXEN_MASK );
        U1CONbits.ON = 0U;
        
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
            U1CONbits.BRGS = 1U;
            
            if(uxbrg > UART_MAX_BAUD)
            {
                /* Calculate BRG value for low speed mode*/
                uxbrg = (srcClkFreq/(16U*baud)) - 1U;
                U1CONbits.BRGS = 0U;
                if(uxbrg > UART_MAX_BAUD)
                {
                    /* Baud rate cannot be achieved with current clock source value */
                    return status;
                }
            }
        }
        else
        {
            U1CONbits.CLKMOD = 1U;
        }
        
        if(setup->dataWidth == UART_DATA_8_BIT)
        {
            /* Configure UART1 mode with parity if mode is 8 bit */
            U1CONbits.MODE = setup->parity;
        }
        else
        {
            /* Configure UART1 mode to 7 bit */
            U1CONbits.MODE = setup->dataWidth;
        }

        /* Configure UART1 mode */
        U1CONbits.STP = setup->stopBits;

        /* Configure UART1 Baud Rate */
        U1BRG = uxbrg;

        /* Restore UTXEN, URXEN bits. */
        U1CON |= ctrlReg;
        
        U1CONbits.ON = 1U;

        status = true;
    }

    return status;
}

bool UART1_Read(void* buffer, const size_t size)
{
    bool status = false;
    uint32_t errorStatus;
    size_t processedSize = 0;

    if(buffer != NULL)
    {

        /* Clear error flags and flush out error data that may have been received when no active request was pending */
        UART1_ErrorClear();

        while( size > processedSize )
        {
            while((U1STATbits.RXBE) != 0U)
            {
                /* Wait for receiver to be ready */
            }

            /* Error status */
            errorStatus = (U1STAT & (_U1STAT_RXFOIF_MASK | _U1STAT_FERIF_MASK | _U1STAT_PERIF_MASK));

            if(errorStatus != 0U)
            {
                break;
            }
            
            /* 8-bit and 7-bit mode */
            ((uint8_t*)(buffer))[processedSize] = (uint8_t)(U1RXB);
            processedSize++;
        }

        if(size == processedSize)
        {
            status = true;
        }
    }

    return status;
}

bool UART1_Write( void* buffer, const size_t size )
{
    bool status = false;
    size_t processedSize = 0;

    if(buffer != NULL)
    {
        while( size > processedSize )
        {
            /* Wait while TX buffer is full */
            while (U1STATbits.TXBF != 0U)
            {
                /* Wait for transmitter to be ready */
            }

            /* 7-bit and 8-bit mode */
            U1TXB = ((uint8_t*)(buffer))[processedSize];

            processedSize++;
        }

        status = true;
    }

    return status;
}

UART_ERROR UART1_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;

    errors = (U1STAT & (_U1STAT_RXFOIF_MASK | _U1STAT_FERIF_MASK | _U1STAT_PERIF_MASK));

    if(errors != UART_ERROR_NONE)
    {
        UART1_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool UART1_AutoBaudQuery( void )
{
    bool autobaudcheck = false;
    if(U1CONbits.ABDEN != 0U)
    {

      autobaudcheck = true;

    }
    return autobaudcheck;
}

void UART1_AutoBaudSet( bool enable )
{
    if( enable == true )
    {
        U1CONbits.ABDEN =  1U;
    }
}


int UART1_ReadByte( void )
{
    return(int)(U1RXB);
}

bool UART1_ReceiverIsReady( void )
{
    return (bool)(U1STATbits.RXBE == 0U);
}

void UART1_WriteByte( int data )
{
    while (U1STATbits.TXBF != 0U)
    {
        /* Do Nothing */
    }

    U1TXB = (uint32_t)data;
}

bool UART1_TransmitterIsReady( void )
{
    return (bool)(U1STATbits.TXBF == 0U);
}

bool UART1_TransmitComplete( void )
{
    bool transmitComplete = false;

    if(U1STATbits.TXMTIF != 0U)
    {
        transmitComplete = true;
    }

    return transmitComplete;
}