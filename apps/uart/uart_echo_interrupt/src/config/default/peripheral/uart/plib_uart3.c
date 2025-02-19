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

volatile static UART_OBJECT uart3Obj;

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

void U3E_InterruptHandler(void);
void U3RX_InterruptHandler(void);
void U3TX_InterruptHandler(void);

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

        /* Clear error interrupt flag */
        IFS3bits.U3EIF = 0;

        /* Clear up the receive interrupt flag so that RX interrupt is not
         * triggered for error bytes */
        IFS3bits.U3RXIF = 0;
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
    U3STAT = (_U3STAT_RXFOIE_MASK
            |_U3STAT_FERIE_MASK
            |_U3STAT_PERIE_MASK);

    /* BAUD Rate register Setup */
    U3BRG = 0x364;

    /* Disable Interrupts */
    IEC3bits.U3EIE = 0U;
    IEC3bits.U3RXIE = 0U;
    IEC3bits.U3TXIE = 0U;

    /* Initialize instance object */
    uart3Obj.rxBuffer = NULL;
    uart3Obj.rxSize = 0;
    uart3Obj.rxProcessedSize = 0;
    uart3Obj.rxBusyStatus = false;
    uart3Obj.rxCallback = NULL;
    uart3Obj.txBuffer = NULL;
    uart3Obj.txSize = 0;
    uart3Obj.txProcessedSize = 0;
    uart3Obj.txBusyStatus = false;
    uart3Obj.txCallback = NULL;
    uart3Obj.errors = UART_ERROR_NONE;

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

    if(uart3Obj.rxBusyStatus == true)
    {
        /* Transaction is in progress, so return without updating settings */
        return status;
    }
    if (uart3Obj.txBusyStatus == true)
    {
        /* Transaction is in progress, so return without updating settings */
        return status;
    }

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

    if(buffer != NULL)
    {
        /* Check if receive request is in progress */
        if(uart3Obj.rxBusyStatus == false)
        {
            /* Clear error flags and flush out error data that may have been received when no active request was pending */
            UART3_ErrorClear();

            uart3Obj.rxBuffer = buffer;
            uart3Obj.rxSize = size;
            uart3Obj.rxProcessedSize = 0;
            uart3Obj.rxBusyStatus = true;
            uart3Obj.errors = UART_ERROR_NONE;
            status = true;

            /* Enable UART3_FAULT Interrupt */
            IEC3bits.U3EIE = 1U;

            /* Enable UART3_RX Interrupt */
            IEC3bits.U3RXIE = 1U;
        }
    }

    return status;
}

bool UART3_Write( void* buffer, const size_t size )
{
    bool status = false;

    if(buffer != NULL)
    {
        /* Check if transmit request is in progress */
        if(uart3Obj.txBusyStatus == false)
        {
            uart3Obj.txBuffer = buffer;
            uart3Obj.txSize = size;
            uart3Obj.txProcessedSize = 0;
            uart3Obj.txBusyStatus = true;
            status = true;

            size_t txProcessedSize = uart3Obj.txProcessedSize;
            size_t txSize = uart3Obj.txSize;

            /* Initiate the transfer by writing as many bytes as we can */
            while((U3STATbits.TXBF == 0U) && (txSize > txProcessedSize) )
            {
                
                /* 7-bit and 8-bit mode */
                U3TXB = ((uint8_t*)uart3Obj.txBuffer)[txProcessedSize];
                txProcessedSize++;
            }

            uart3Obj.txProcessedSize = txProcessedSize;

            IEC3bits.U3TXIE = 1U;
        }
    }

    return status;
}

UART_ERROR UART3_ErrorGet( void )
{
    UART_ERROR errors = uart3Obj.errors;

    uart3Obj.errors = UART_ERROR_NONE;

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

void UART3_ReadCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart3Obj.rxCallback = callback;

    uart3Obj.rxContext = context;
}

bool UART3_ReadIsBusy( void )
{
    return uart3Obj.rxBusyStatus;
}

size_t UART3_ReadCountGet( void )
{
    return uart3Obj.rxProcessedSize;
}

bool UART3_ReadAbort(void)
{
    if (uart3Obj.rxBusyStatus == true)
    {
        /* Disable the fault interrupt */
        IEC3bits.U3EIE = 0U;

        /* Disable the receive interrupt */
        IEC3bits.U3RXIE = 0U;

        uart3Obj.rxBusyStatus = false;

        /* If required application should read the num bytes processed prior to calling the read abort API */
        uart3Obj.rxSize = 0U;
        uart3Obj.rxProcessedSize = 0U;
    }

    return true;
}

void UART3_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart3Obj.txCallback = callback;

    uart3Obj.txContext = context;
}

bool UART3_WriteIsBusy( void )
{
    return uart3Obj.txBusyStatus;
}

size_t UART3_WriteCountGet( void )
{
    return uart3Obj.txProcessedSize;
}

void U3E_InterruptHandler(void)
{
    /* Save the error to be reported later */
    uart3Obj.errors = (U3STAT & (_U3STAT_RXFOIF_MASK | _U3STAT_FERIF_MASK | _U3STAT_PERIF_MASK));

    /* Disable the fault interrupt */
    IEC3bits.U3EIE = 0U;

    /* Disable the receive interrupt */
    IEC3bits.U3RXIE = 0U;

    /* Clear size and rx status */
    uart3Obj.rxBusyStatus = false;

    UART3_ErrorClear();

    /* Client must call UARTx_ErrorGet() function to get the errors */
    if( uart3Obj.rxCallback != NULL )
    {
        uintptr_t rxContext = uart3Obj.rxContext;

        uart3Obj.rxCallback(rxContext);
    }
}

void U3RX_InterruptHandler(void)
{
    if(uart3Obj.rxBusyStatus == true)
    {
        size_t rxProcessedSize = uart3Obj.rxProcessedSize;
        size_t rxSize = uart3Obj.rxSize;

        while((U3STATbits.RXBE != 1U) && (rxSize > rxProcessedSize))
        {

            /* 8-bit mode */
            ((uint8_t*)uart3Obj.rxBuffer)[rxProcessedSize] = (uint8_t )(U3RXB);
            rxProcessedSize++;
        }

        uart3Obj.rxProcessedSize = rxProcessedSize;

        /* Clear UART3 RX Interrupt flag */
        IFS3bits.U3RXIF = 0U;
        
        /* Check if the buffer is done */
        if(rxProcessedSize >= uart3Obj.rxSize)
        {
            uart3Obj.rxBusyStatus = false;

            /* Disable the fault interrupt */
            IEC3bits.U3EIE = 0U;

            /* Disable the receive interrupt */
            IEC3bits.U3RXIE = 0U;

            if(uart3Obj.rxCallback != NULL)
            {
                uintptr_t rxContext = uart3Obj.rxContext;

                uart3Obj.rxCallback(rxContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
    }
}

void U3TX_InterruptHandler(void)
{
    if(uart3Obj.txBusyStatus == true)
    {
        size_t txSize = uart3Obj.txSize;
        size_t txProcessedSize = uart3Obj.txProcessedSize;

        /* Clear UART3 TX Interrupt flag */
        IFS3bits.U3TXIF = 0u;
        
        while((U3STATbits.TXBF == 0U) && (txSize > txProcessedSize) )
        {
            /* 8-bit mode */
            U3TXB = ((uint8_t*)uart3Obj.txBuffer)[txProcessedSize];
            txProcessedSize++;
        }

        uart3Obj.txProcessedSize = txProcessedSize;

        /* Check if the buffer is done */
        if(txProcessedSize >= uart3Obj.txSize)
        {
            uart3Obj.txBusyStatus = false;

            /* Disable the transmit interrupt, to avoid calling ISR continuously */
            IEC3bits.U3TXIE = 0U;

            if(uart3Obj.txCallback != NULL)
            {
                uintptr_t txContext = uart3Obj.txContext;

                uart3Obj.txCallback(txContext);
            }
        }
    }
    else
    {
        // Nothing to process
        ;
    }
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