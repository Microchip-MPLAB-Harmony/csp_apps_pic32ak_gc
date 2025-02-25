/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi1_client.c

  Summary:
    SPI1 Client Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SPI peripheral.

*******************************************************************************/

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
#include <string.h>
#include "plib_spi1_client.h"
#include "peripheral/gpio/plib_gpio.h"
#include "interrupts.h"
// *****************************************************************************
// *****************************************************************************
// Section: SPI1 Slave Implementation
// *****************************************************************************
// *****************************************************************************
#define SPI1_BUSY_PIN                    GPIO_PIN_RB6
#define SPI1_CS_PIN                      GPIO_PIN_RB7


#define SPI1_READ_BUFFER_SIZE            256
#define SPI1_WRITE_BUFFER_SIZE           256

volatile static uint8_t SPI1_ReadBuffer[SPI1_READ_BUFFER_SIZE];
volatile static uint8_t SPI1_WriteBuffer[SPI1_WRITE_BUFFER_SIZE];


/* Global object to save SPI Exchange related data */
volatile static SPI_CLIENT_OBJECT spi1Obj;

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

//SPI SPIxCON1 MODE options
#define SPI1CON1_MSTEN_CLIENT_MODE          ((uint32_t)(_SPI1CON1_MSTEN_MASK & ((uint32_t)(0) << _SPI1CON1_MSTEN_POSITION))) 
#define SPI1CON1_MSTEN_HOST_MODE          ((uint32_t)(_SPI1CON1_MSTEN_MASK & ((uint32_t)(1) << _SPI1CON1_MSTEN_POSITION))) 

//SPI SPIxCON1 Clock Polarity options
#define SPI1CON1_CKP_IDLE_LOW_ACTIVE_HIGH           ((uint32_t)(_SPI1CON1_CKP_MASK & ((uint32_t)(0) << _SPI1CON1_CKP_POSITION))) 
#define SPI1CON1_CKP_IDLE_HIGH_ACTIVE_LOW           ((uint32_t)(_SPI1CON1_CKP_MASK & ((uint32_t)(1) << _SPI1CON1_CKP_POSITION))) 

//SPI SPIxCON1 Clock Edge options
#define SPI1CON1_CKE_IDLE_TO_ACTIVE           ((uint32_t)(_SPI1CON1_CKE_MASK & ((uint32_t)(0) << _SPI1CON1_CKE_POSITION))) 
#define SPI1CON1_CKE_ACTIVE_TO_IDLE           ((uint32_t)(_SPI1CON1_CKE_MASK & ((uint32_t)(1) << _SPI1CON1_CKE_POSITION))) 

//SPI SPIxCON1 Clock select options
#define SPI1CON1_MCLKEN_UPB_CLOCK           ((uint32_t)(_SPI1CON1_MCLKEN_MASK & ((uint32_t)(0) << _SPI1CON1_MCLKEN_POSITION))) 
#define SPI1CON1_MCLKEN_CLOCK_GEN_9           ((uint32_t)(_SPI1CON1_MCLKEN_MASK & ((uint32_t)(1) << _SPI1CON1_MCLKEN_POSITION))) 

/* Forward declarations */
static void SPI1_CS_Handler(GPIO_PIN pin, uintptr_t context);

static void mem_copy(volatile void* pDst, volatile void* pSrc, uint32_t nBytes)
{
    volatile uint8_t* pSource = (volatile uint8_t*)pSrc;
    volatile uint8_t* pDest = (volatile uint8_t*)pDst;

    for (uint32_t i = 0U; i < nBytes; i++)
    {
        pDest[i] = pSource[i];
    }
}

void SPI1_Initialize ( void )
{
    /* Disable SPI1 Interrupts */
    IEC2bits.SPI1EIE = 0U;
    IEC1bits.SPI1RXIE = 0U;
    IEC2bits.SPI1TXIE = 0U;

    /* STOP and Reset the SPI */
    SPI1CON1 = 0x00U;

    /* Clear SPI1 Interrupt flags */
    IFS2bits.SPI1EIF = 0U;
    IFS1bits.SPI1RXIF = 0U;
    IFS2bits.SPI1TXIF = 0U;

    SPI1CON1 = (SPI1CON1_MSTEN_CLIENT_MODE
            |SPI1CON1_CKP_IDLE_LOW_ACTIVE_HIGH
            |SPI1CON1_CKE_IDLE_TO_ACTIVE
            |SPI1CON1_MCLKEN_UPB_CLOCK
            |_SPI1CON1_SSEN_MASK
            |_SPI1CON1_ENHBUF_MASK);
            
    /* Enable generation of interrupt on receiver overflow */
    SPI1IMSKbits.SPIROVEN = 1U;

    spi1Obj.rdInIndex = 0U;
    spi1Obj.wrOutIndex = 0U;
    spi1Obj.nWrBytes = 0U;
    spi1Obj.errorStatus = SPI_CLIENT_ERROR_NONE;
    spi1Obj.callback = NULL ;
    spi1Obj.transferIsBusy = false ;
    spi1Obj.csInterruptPending = false;
    spi1Obj.rxInterruptActive = false;

    /* Set the Busy Pin to ready state */
    GPIO_PinWrite((GPIO_PIN)SPI1_BUSY_PIN, 0);

    /* Register callback and enable notifications on Chip Select logic level change */
    (void)GPIO_PinInterruptCallbackRegister(SPI1_CS_PIN, SPI1_CS_Handler, 0U);
    GPIO_PinInterruptEnable(SPI1_CS_PIN);

    /* Enable watermark interrupt for one byte reception*/
    SPI1IMSKbits.RXMSK = 1U;
    SPI1IMSKbits.RXWIEN = 1U;
    SPI1IMSKbits.SPITBEN = 1U;

    /* Enable SPI1 RX and Error Interrupts. TX interrupt will be enabled when a SPI write is submitted. */
    IEC1bits.SPI1RXIE = 1U;
    IEC2bits.SPI1EIE = 1U;

    /* Enable SPI1 */
    SPI1CON1bits.ON = 1U;
}

void SPI1_Deinitialize ( void )
{
    /* Disable SPI1 Interrupts */
    IEC2bits.SPI1EIE = 0U;
    IEC1bits.SPI1RXIE = 0U;
    IEC2bits.SPI1TXIE = 0U;

    /* STOP the SPI */
    SPI1CON1bits.ON = 0U;

    /* Clear SPI1 Interrupt flags */
    IFS2bits.SPI1EIF = 0U;
    IFS1bits.SPI1RXIF = 0U;
    IFS2bits.SPI1TXIF = 0U;

    SPI1CON1 = 0x0UL;
    SPI1CON2 = 0x0UL;
    SPI1BRG = 0x0UL;
    SPI1IMSK = 0x0UL;
    SPI1STAT = 0x28UL;

}

/* For 16-bit/32-bit mode, the "size" must be specified in terms of 16-bit/32-bit words */
size_t SPI1_Read(void* pRdBuffer, size_t size)
{
    size_t rdSize = size;
    size_t rdInIndex = spi1Obj.rdInIndex;

    if (rdSize > rdInIndex)
    {
        rdSize = rdInIndex;
    }

   (void) mem_copy(pRdBuffer, SPI1_ReadBuffer, rdSize);

    return rdSize;
}

/* For 16-bit/32-bit mode, the "size" must be specified in terms of 16-bit/32-bit words */
size_t SPI1_Write(void* pWrBuffer, size_t size )
{
    uint32_t wrSize = size;
    uint32_t wrOutIndex = 0U;

    IEC2bits.SPI1TXIE = 0U;

    if (wrSize > (uint32_t)SPI1_WRITE_BUFFER_SIZE)
    {
        wrSize = SPI1_WRITE_BUFFER_SIZE;
    }

    (void) mem_copy(SPI1_WriteBuffer, pWrBuffer, wrSize);

    spi1Obj.nWrBytes = wrSize;

    /* Fill up the FIFO as long as there are empty elements */
    while ((!(SPI1STAT & _SPI1STAT_SPITBF_MASK)) && (wrOutIndex < wrSize))
    {
        SPI1BUF = SPI1_WriteBuffer[wrOutIndex];
        wrOutIndex++;
    }

    spi1Obj.wrOutIndex = wrOutIndex;

    /* Enable TX interrupt */
    IEC2bits.SPI1TXIE = 1U;

    return wrSize;
}

/* For 16-bit/32-bit mode, the return value is in terms of 16-bit/32-bit words */
size_t SPI1_ReadCountGet(void)
{
    return spi1Obj.rdInIndex;
}

/* For 16-bit/32-bit mode, the return value is in terms of 16-bit/32-bit words */
size_t SPI1_ReadBufferSizeGet(void)
{
    return SPI1_READ_BUFFER_SIZE;
}

/* For 16-bit/32-bit mode, the return value is in terms of 16-bit/32-bit words */
size_t SPI1_WriteBufferSizeGet(void)
{
    return SPI1_WRITE_BUFFER_SIZE;
}

void SPI1_CallbackRegister(SPI_CLIENT_CALLBACK callBack, uintptr_t context )
{
    spi1Obj.callback = callBack;

    spi1Obj.context = context;
}

/* The status is returned as busy when CS is asserted */
bool SPI1_IsBusy(void)
{
    return spi1Obj.transferIsBusy;
}

/* Drive the GPIO pin to indicate to SPI Master that the client is ready now */
void SPI1_Ready(void)
{
    GPIO_PinWrite((GPIO_PIN)SPI1_BUSY_PIN, 0);
}

SPI_CLIENT_ERROR SPI1_ErrorGet(void)
{
    SPI_CLIENT_ERROR errorStatus = spi1Obj.errorStatus;

    spi1Obj.errorStatus = SPI_CLIENT_ERROR_NONE;

    return errorStatus;
}

static void __attribute__((used)) SPI1_CS_Handler(GPIO_PIN pin, uintptr_t context)
{
    bool activeState = false;

    if (GPIO_PinRead((GPIO_PIN)SPI1_CS_PIN) == activeState)
    {
        /* CS is asserted */
        spi1Obj.transferIsBusy = true;

        /* Drive busy line to active state */
        GPIO_PinWrite((GPIO_PIN)SPI1_BUSY_PIN, 1);
    }
    else
    {
        /* Give application callback only if RX interrupt is not preempted and RX interrupt is not pending to be serviced */

        bool rxInterruptActive = spi1Obj.rxInterruptActive;

        if ((IFS1bits.SPI1RXIF == 0U) && (rxInterruptActive == false))
        {
            /* CS is de-asserted */
            spi1Obj.transferIsBusy = false;

            spi1Obj.wrOutIndex = 0U;
            spi1Obj.nWrBytes = 0U;

            if(spi1Obj.callback != NULL)
            {
                uintptr_t context_val = spi1Obj.context;

                spi1Obj.callback(context_val);
            }

            /* Clear the read index. Application must read out the data by calling SPI1_Read API in the callback */
            spi1Obj.rdInIndex = 0U;
        }
        else
        {
            /* If CS interrupt is serviced by either preempting the RX interrupt or RX interrupt is pending to be serviced,
             * then delegate the responsibility of giving application callback to the RX interrupt handler */

            spi1Obj.csInterruptPending = true;
        }
    }
}

void __attribute__((used)) SPI1E_InterruptHandler (void)
{
    spi1Obj.errorStatus =(SPI1STAT & _SPI1STAT_SPIROV_MASK);

    /* Clear the receive overflow flag */
    SPI1STATbits.SPIROV = 0U;

    IFS2bits.SPI1EIF = 0U;
}

void __attribute__((used)) SPI1TX_InterruptHandler (void)
{
    size_t wrOutIndex = spi1Obj.wrOutIndex;
    size_t nWrBytes = spi1Obj.nWrBytes;

    /* Fill up the FIFO as long as there are empty elements */
    while ((!(SPI1STAT & _SPI1STAT_SPITBF_MASK)) && (wrOutIndex < nWrBytes))
    {
        SPI1BUF = SPI1_WriteBuffer[wrOutIndex];
        wrOutIndex++;
    }

    spi1Obj.wrOutIndex = wrOutIndex;

    /* Clear the transmit interrupt flag */
    IFS2bits.SPI1TXIF = 0U;

    if (spi1Obj.wrOutIndex == nWrBytes)
    {
        /* Nothing to transmit. Disable transmit interrupt. The last byte sent by the master will be shifted out automatically*/
        IEC2bits.SPI1TXIE = 0U;
    }
}

void __attribute__((used)) SPI1RX_InterruptHandler (void)
{
    uint32_t receivedData = 0;

    spi1Obj.rxInterruptActive = true;

    size_t rdInIndex = spi1Obj.rdInIndex;

    while (!(SPI1STAT & _SPI1STAT_SPIRBE_MASK))
    {
        /* Receive buffer is not empty. Read the received data. */
        receivedData = SPI1BUF;

        if (rdInIndex < (uint32_t)SPI1_READ_BUFFER_SIZE)
        {
            SPI1_ReadBuffer[rdInIndex] = (uint8_t)receivedData;
            rdInIndex++;
        }
    }

    spi1Obj.rdInIndex = rdInIndex;

    /* Clear the receive interrupt flag */
    IFS1bits.SPI1RXIF = 0U;

    spi1Obj.rxInterruptActive = false;

    /* Check if CS interrupt occured before the RX interrupt and that CS interrupt delegated the responsibility to give
     * application callback to the RX interrupt */

    if (spi1Obj.csInterruptPending == true)
    {
        spi1Obj.csInterruptPending = false;
        spi1Obj.transferIsBusy = false;

        spi1Obj.wrOutIndex = 0U;
        spi1Obj.nWrBytes = 0U;

        if(spi1Obj.callback != NULL)
        {
            uintptr_t context = spi1Obj.context;

            spi1Obj.callback(context);
        }

        /* Clear the read index. Application must read out the data by calling SPI1_Read API in the callback */
        spi1Obj.rdInIndex = 0U;
    }
}
