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

#include <xc.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "device.h"
#include "interrupts.h"
#include "plib_uart3.h"

void U3E_InterruptHandler(void);
void U3RX_InterruptHandler(void);
void U3TX_InterruptHandler(void);

// Section: UART3 Implementation

volatile static UART_RING_BUFFER_OBJECT uart3Obj;

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

#define UART3_READ_BUFFER_SIZE      (20U + 1U)
volatile static uint8_t UART3_ReadBuffer[UART3_READ_BUFFER_SIZE];

#define UART3_WRITE_BUFFER_SIZE      (128U + 1U)
volatile static uint8_t UART3_WriteBuffer[UART3_WRITE_BUFFER_SIZE];

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
        IFS3bits.U3RXIF = 0;

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
    uart3Obj.rdCallback = NULL;
    uart3Obj.rdInIndex = 0;
    uart3Obj.rdOutIndex = 0;
    uart3Obj.isRdNotificationEnabled = false;
    uart3Obj.isRdNotifyPersistently = false;
    uart3Obj.rdThreshold = 0;

    uart3Obj.wrCallback = NULL;
    uart3Obj.wrInIndex = 0;
    uart3Obj.wrOutIndex = 0;
    uart3Obj.isWrNotificationEnabled = false;
    uart3Obj.isWrNotifyPersistently = false;
    uart3Obj.wrThreshold = 0;

    uart3Obj.errors = UART_ERROR_NONE;


    uart3Obj.rdBufferSize = UART3_READ_BUFFER_SIZE;
    uart3Obj.wrBufferSize = UART3_WRITE_BUFFER_SIZE;

    /* Enable UART3_ERROR Interrupt */
    IEC3bits.U3EIE = 1U;

    /* Enable UART3_RX Interrupt */
    IEC3bits.U3RXIE = 1U;
    
    /* Turn ON UART3 */
    U3CON |= (_U3CON_ON_MASK
                 |_U3CON_TXEN_MASK
                 |_U3CON_RXEN_MASK);
}

bool UART3_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    bool status = false;
    uint32_t baud;
    uint32_t ctrlReg;
    uint32_t uxbrg;

    if (setup != NULL)
    {
        baud = setup->baudRate;

        if (baud == 0U)
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
            U3CONbits.CLKMOD = 1;
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

        uart3Obj.rdBufferSize = UART3_READ_BUFFER_SIZE;
        uart3Obj.wrBufferSize = UART3_WRITE_BUFFER_SIZE;

        U3CONbits.ON = 1U;

        /* Restore UTXEN, URXEN bits. */
        U3CON |= ctrlReg;

        status = true;
    }

    return status;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static inline bool UART3_RxPushByte(uint8_t rdByte)
{
    uint32_t nextIndex;
    bool isSuccess = false;

    // nextIndex is where head will point to after this write.
    nextIndex = uart3Obj.rdInIndex + 1U;

    if (nextIndex >= uart3Obj.rdBufferSize)
    {
        nextIndex = 0U;
    }

    if (nextIndex == uart3Obj.rdOutIndex)
    {
        /* Queue is full - Report it to the application. Application gets a chance to free up space by reading data out from the RX ring buffer */
        if(uart3Obj.rdCallback != NULL)
        {
            uintptr_t rdContext = uart3Obj.rdContext;

            uart3Obj.rdCallback(UART_EVENT_READ_BUFFER_FULL, rdContext);

            /* Read the indices again in case application has freed up space in RX ring buffer */
            nextIndex = uart3Obj.rdInIndex + 1U;

            if (nextIndex >= uart3Obj.rdBufferSize)
            {
                nextIndex = 0U;
            }
        }
    }

    /* Attempt to push the data into the ring buffer */
    if (nextIndex != uart3Obj.rdOutIndex)
    {
        uint32_t rdInIndex = uart3Obj.rdInIndex;

        UART3_ReadBuffer[rdInIndex] = (uint8_t)rdByte;

        uart3Obj.rdInIndex = nextIndex;

        isSuccess = true;
    }
    else
    {
        /* Queue is full. Data will be lost. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void UART3_ReadNotificationSend(void)
{
    uint32_t nUnreadBytesAvailable;

    if (uart3Obj.isRdNotificationEnabled == true)
    {
        nUnreadBytesAvailable = UART3_ReadCountGet();

        if(uart3Obj.rdCallback != NULL)
        {
            uintptr_t rdContext = uart3Obj.rdContext;

            if (uart3Obj.isRdNotifyPersistently == true)
            {
                if (nUnreadBytesAvailable >= uart3Obj.rdThreshold)
                {
                    uart3Obj.rdCallback(UART_EVENT_READ_THRESHOLD_REACHED, rdContext);
                }
            }
            else
            {
                if (nUnreadBytesAvailable == uart3Obj.rdThreshold)
                {
                    uart3Obj.rdCallback(UART_EVENT_READ_THRESHOLD_REACHED, rdContext);
                }
            }
        }
    }
}

size_t UART3_Read(uint8_t* pRdBuffer, const size_t size)
{
    size_t nBytesRead = 0;
    uint32_t rdOutIndex = 0;
    uint32_t rdInIndex = 0;

    /* Take a snapshot of indices to avoid creation of critical section */
    rdOutIndex = uart3Obj.rdOutIndex;
    rdInIndex = uart3Obj.rdInIndex;

    while (nBytesRead < size)
    {
        if (rdOutIndex != rdInIndex)
        {
            pRdBuffer[nBytesRead] = UART3_ReadBuffer[rdOutIndex];
            nBytesRead++;
            rdOutIndex++;

            if (rdOutIndex >= uart3Obj.rdBufferSize)
            {
                rdOutIndex = 0U;
            }
        }
        else
        {
            /* No more data available in the RX buffer */
            break;
        }
    }

    uart3Obj.rdOutIndex = rdOutIndex;

    return nBytesRead;
}

size_t UART3_ReadCountGet(void)
{
    size_t nUnreadBytesAvailable;
    uint32_t rdInIndex;
    uint32_t rdOutIndex;

    /* Take a snapshot of indices to avoid processing in critical section */
    rdInIndex = uart3Obj.rdInIndex;
    rdOutIndex = uart3Obj.rdOutIndex;

    if ( rdInIndex >=  rdOutIndex)
    {
        nUnreadBytesAvailable =  (size_t)(rdInIndex -  rdOutIndex);
    }
    else
    {
        nUnreadBytesAvailable =  (size_t)((uart3Obj.rdBufferSize -  rdOutIndex) + rdInIndex);
    }

    return nUnreadBytesAvailable;
}

size_t UART3_ReadFreeBufferCountGet(void)
{
    return (uart3Obj.rdBufferSize - 1U) - UART3_ReadCountGet();
}

size_t UART3_ReadBufferSizeGet(void)
{
    return (uart3Obj.rdBufferSize - 1U);
}

bool UART3_ReadNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = uart3Obj.isRdNotificationEnabled;

    uart3Obj.isRdNotificationEnabled = isEnabled;

    uart3Obj.isRdNotifyPersistently = isPersistent;

    return previousStatus;
}

void UART3_ReadThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0U)
    {
        uart3Obj.rdThreshold = nBytesThreshold;
    }
}

void UART3_ReadCallbackRegister( UART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    uart3Obj.rdCallback = callback;

    uart3Obj.rdContext = context;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static bool UART3_TxPullByte(uint16_t* pWrByte)
{
    bool isSuccess = false;
    uint32_t wrOutIndex = uart3Obj.wrOutIndex;
    uint32_t wrInIndex = uart3Obj.wrInIndex;

    if (wrOutIndex != wrInIndex)
    {

        *pWrByte = UART3_WriteBuffer[wrOutIndex];
        wrOutIndex++;
        if (wrOutIndex >= uart3Obj.wrBufferSize)
        {
            wrOutIndex = 0U;
        }

        uart3Obj.wrOutIndex = wrOutIndex;

        isSuccess = true;
    }

    return isSuccess;
}

static inline bool UART3_TxPushByte(uint8_t wrByte)
{
    uint32_t nextIndex;
    bool isSuccess = false;
    uint32_t wrOutIndex = uart3Obj.wrOutIndex;
    uint32_t wrInIndex = uart3Obj.wrInIndex;

    nextIndex = wrInIndex + 1U;

    if (nextIndex >= uart3Obj.wrBufferSize)
    {
        nextIndex = 0U;
    }
    if (nextIndex != wrOutIndex)
    {

        UART3_WriteBuffer[wrInIndex] = (uint8_t)wrByte;

        uart3Obj.wrInIndex = nextIndex;

        isSuccess = true;
    }
    else
    {
        /* Queue is full. Report Error. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void UART3_WriteNotificationSend(void)
{
    uint32_t nFreeWrBufferCount;

    if (uart3Obj.isWrNotificationEnabled == true)
    {
        nFreeWrBufferCount = UART3_WriteFreeBufferCountGet();

        if(uart3Obj.wrCallback != NULL)
        {
            uintptr_t wrContext = uart3Obj.wrContext;

            if (uart3Obj.isWrNotifyPersistently == true)
            {
                if (nFreeWrBufferCount >= uart3Obj.wrThreshold)
                {
                    uart3Obj.wrCallback(UART_EVENT_WRITE_THRESHOLD_REACHED, wrContext);
                }
            }
            else
            {
                if (nFreeWrBufferCount == uart3Obj.wrThreshold)
                {
                    uart3Obj.wrCallback(UART_EVENT_WRITE_THRESHOLD_REACHED, wrContext);
                }
            }
        }
    }
}

static size_t UART3_WritePendingBytesGet(void)
{
    size_t nPendingTxBytes;

    /* Take a snapshot of indices to avoid processing in critical section */

    uint32_t wrOutIndex = uart3Obj.wrOutIndex;
    uint32_t wrInIndex = uart3Obj.wrInIndex;

    if ( wrInIndex >=  wrOutIndex)
    {
        nPendingTxBytes =  (size_t)(wrInIndex - wrOutIndex);
    }
    else
    {
        nPendingTxBytes =  (size_t)((uart3Obj.wrBufferSize -  wrOutIndex) + wrInIndex);
    }

    return nPendingTxBytes;
}

size_t UART3_WriteCountGet(void)
{
    size_t nPendingTxBytes;

    nPendingTxBytes = UART3_WritePendingBytesGet();

    return nPendingTxBytes;
}

size_t UART3_Write(uint8_t* pWrBuffer, const size_t size )
{
    size_t nBytesWritten  = 0U;

    while (nBytesWritten < size)
    {

        if (UART3_TxPushByte(pWrBuffer[nBytesWritten]) == true)
        {
            nBytesWritten++;
        }
        else
        {
            /* Queue is full, exit the loop */
            break;
        }

    }

    /* Check if any data is pending for transmission */
    if (UART3_WritePendingBytesGet() > 0U)
    {
        /* Enable TX interrupt as data is pending for transmission */
        IEC3bits.U3TXIE = 1U;
    }

    return nBytesWritten;
}

size_t UART3_WriteFreeBufferCountGet(void)
{
    return (uart3Obj.wrBufferSize - 1U) - UART3_WriteCountGet();
}

size_t UART3_WriteBufferSizeGet(void)
{
    return (uart3Obj.wrBufferSize - 1U);
}

bool UART3_TransmitComplete(void)
{
    bool status = false;

    if(U3STATbits.TXMTIF != 0U)
    {
        status = true;
    }
    return status;
}

bool UART3_WriteNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = uart3Obj.isWrNotificationEnabled;

    uart3Obj.isWrNotificationEnabled = isEnabled;

    uart3Obj.isWrNotifyPersistently = isPersistent;

    return previousStatus;
}

void UART3_WriteThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0U)
    {
        uart3Obj.wrThreshold = nBytesThreshold;
    }
}

void UART3_WriteCallbackRegister( UART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    uart3Obj.wrCallback = callback;

    uart3Obj.wrContext = context;
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

    /* Turning off ABAUD if it was on can lead to unpredictable behavior, so that
       direction of control is not allowed in this function.                      */
}

void U3E_InterruptHandler(void)
{
    /* Save the error to be reported later */
    uart3Obj.errors = (U3STAT & (_U3STAT_RXFOIF_MASK | _U3STAT_FERIF_MASK | _U3STAT_PERIF_MASK));

    /* Disable the fault interrupt */
    IEC3bits.U3EIE = 0U;

    /* Disable the receive interrupt */
    IEC3bits.U3RXIE = 0U;
    
    UART3_ErrorClear();

    /* Client must call UARTx_ErrorGet() function to clear the errors */
    if( uart3Obj.rdCallback != NULL )
    {
        uintptr_t rdContext = uart3Obj.rdContext;

        uart3Obj.rdCallback(UART_EVENT_READ_ERROR, rdContext);
    }
}


void U3RX_InterruptHandler(void)
{
    /* Keep reading until there is a character available in the RX FIFO */
    while(U3STATbits.RXBE != 1U)
    {
        if (UART3_RxPushByte((uint8_t )(U3RXB)) == true)
        {
            UART3_ReadNotificationSend();
        }
        else
        {
            /* UART RX buffer is full */
        }
    }

    /* Clear UART3 RX Interrupt flag */
    IFS3bits.U3RXIF = 0U;
}

void U3TX_InterruptHandler(void)
{
    uint16_t wrByte;

    /* Check if any data is pending for transmission */
    if (UART3_WritePendingBytesGet() > 0U)
    {
        /* Clear UART3TX Interrupt flag */
        IFS3bits.U3TXIF = 0U;
        
        /* Keep writing to the TX FIFO as long as there is space */
        while(U3STATbits.TXBF == 0U)
        {
            if (UART3_TxPullByte(&wrByte) == true)
            {
                U3TXB = (uint8_t)wrByte;

                /* Send notification */
                UART3_WriteNotificationSend();
            }
            else
            {
                /* Nothing to transmit. Disable the data register empty interrupt. */
                IEC3bits.U3TXIE = 0U;
                break;
            }
        }
    }
    else
    {
        /* Nothing to transmit. Disable the data register empty interrupt. */
        IEC3bits.U3TXIE = 0U;
    }
}
