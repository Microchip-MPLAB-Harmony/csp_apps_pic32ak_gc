/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi1_host.c

  Summary:
    SPI1 Host Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SPI peripheral.

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

#include "plib_spi1_host.h"
#include "interrupts.h"

/* Global object to save SPI Exchange related data */
volatile static SPI_OBJECT spi1Obj;

void SPI1RX_InterruptHandler (void);
void SPI1E_InterruptHandler (void);
void SPI1TX_InterruptHandler (void);

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

/**
* @brief  Macro to define SPI1 FIFO Size available
*/
#define SPI1_FIFO_SIZE               4U

/**
* @brief  Macro to define dummy data used for SPI transfer
*/
#define SPI1_DUMMY_DATA              0xffUL


// *****************************************************************************
// *****************************************************************************
// Section: SPI1 Implementation
// *****************************************************************************
// *****************************************************************************

void SPI1_Initialize ( void )
{
    /* Disable SPI1 Interrupts */
    IEC2bits.SPI1EIE = 0U;
    IEC1bits.SPI1RXIE = 0U;
    IEC2bits.SPI1TXIE = 0U;

    /* STOP and Reset the SPI */
    SPI1CON1 = 0x00UL;

    /* Clear SPI1 Interrupt flags */
    IFS2bits.SPI1EIF = 0U;
    IFS1bits.SPI1RXIF = 0U;
    IFS2bits.SPI1TXIF = 0U;

    /* BAUD Rate register Setup */
    SPI1BRG = 0x31UL;

    SPI1CON1 = (SPI1CON1_MSTEN_HOST_MODE
            |SPI1CON1_CKP_IDLE_HIGH_ACTIVE_LOW
            |SPI1CON1_CKE_IDLE_TO_ACTIVE
            |SPI1CON1_MCLKEN_UPB_CLOCK
            |_SPI1CON1_ENHBUF_MASK);

    /* Initialize global variables */
    spi1Obj.transferIsBusy = false;
    spi1Obj.callback = NULL;

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

bool SPI1_TransferSetup (SPI_TRANSFER_SETUP* setup, uint32_t spiSourceClock )
{
    uint32_t t_brg;
    uint32_t baudHigh;
    uint32_t baudLow;
    uint32_t errorHigh;
    uint32_t errorLow;

    if ((setup == NULL) || (setup->clockFrequency == 0U) || (setup->dataBits != SPI_DATA_BITS_8))
    {
        return false;
    }

    spiSourceClock =  SPI1_FrequencyGet();

    t_brg = (((spiSourceClock / (setup->clockFrequency)) / 2u) - 1u);
    baudHigh = spiSourceClock / (2u * (t_brg + 1u));
    baudLow = spiSourceClock / (2u * (t_brg + 2u));
    errorHigh = baudHigh - setup->clockFrequency;
    errorLow = setup->clockFrequency - baudLow;

    if (errorHigh > errorLow)
    {
        t_brg++;
    }

    if(t_brg > 0x1fffUL)
    {
        return false;
    }

    /* STOP and Reset the SPI */
    SPI1CON1bits.ON = 0U;

    SPI1BRG = t_brg;

    SPI1CON1 = (SPI1CON1 & (~(_SPI1CON1_MODE16_MASK | _SPI1CON1_MODE32_MASK | _SPI1CON1_CKP_MASK | _SPI1CON1_CKE_MASK))) |
                            ((uint32_t)setup->clockPolarity | (uint32_t)setup->clockPhase | (uint32_t)setup->dataBits);

    SPI1CON1bits.ON = 1U;

    return true;
}

bool SPI1_Write(void* pTransmitData, size_t txSize)
{
    return(SPI1_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool SPI1_Read(void* pReceiveData, size_t rxSize)
{
    return(SPI1_WriteRead(NULL, 0, pReceiveData, rxSize));
}

bool SPI1_IsTransmitterBusy (void)
{
    return ((SPI1STAT & _SPI1STAT_SRMT_MASK) == 0U)? true : false;
}


bool SPI1_IsBusy (void)
{
    uint32_t StatRead = SPI1STAT;
    return (((spi1Obj.transferIsBusy) != false) || (( StatRead & _SPI1STAT_SRMT_MASK) == 0U));
}

void SPI1_CallbackRegister (SPI_CALLBACK callback, uintptr_t context)
{
    spi1Obj.callback = callback;

    spi1Obj.context = context;
}

static void SPI_FIFO_Fill(void)
{
    uint8_t nDataCopiedToFIFO = 0U;

    while ((nDataCopiedToFIFO < SPI1_FIFO_SIZE) && (SPI1STATbits.SPITBF == 0U))
    {
        if (spi1Obj.txCount < spi1Obj.txSize)
        {
            SPI1BUF = ((uint8_t*)spi1Obj.txBuffer)[spi1Obj.txCount];
            spi1Obj.txCount++;
        }
        else if (spi1Obj.dummySize > 0U)
        {
            SPI1BUF = SPI1_DUMMY_DATA;
            spi1Obj.dummySize--;
        }
        else
        {
            break;
        }
        nDataCopiedToFIFO++;
    }
}

bool SPI1_WriteRead (void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    bool isRequestAccepted = false;

    /* Verify the request */
    if((spi1Obj.transferIsBusy == false) && (((txSize > 0U) && (pTransmitData != NULL)) || ((rxSize > 0U) && (pReceiveData != NULL))))
    {
        isRequestAccepted = true;
        spi1Obj.txBuffer = pTransmitData;
        spi1Obj.rxBuffer = pReceiveData;
        spi1Obj.rxCount = 0U;
        spi1Obj.txCount = 0U;
        spi1Obj.dummySize = 0U;

        if (pTransmitData != NULL)
        {
            spi1Obj.txSize = txSize;
        }
        else
        {
            spi1Obj.txSize = 0U;
        }

        if (pReceiveData != NULL)
        {
            spi1Obj.rxSize = rxSize;
        }
        else
        {
            spi1Obj.rxSize = 0U;
        }

        spi1Obj.transferIsBusy = true;

        size_t txSz = spi1Obj.txSize;
        if (spi1Obj.rxSize > txSz)
        {
            spi1Obj.dummySize = spi1Obj.rxSize - txSz;
        }

        /* Clear the receive overflow error if any */
        SPI1STATbits.SPIROV = 0U;

        /* Make sure there is no data pending in the RX FIFO */

        while ((SPI1STAT & _SPI1STAT_SPIRBE_MASK) == 0U)
        {
            (void)SPI1BUF;
        }
        /* Configure SPI to generate receive interrupt when receive buffer is empty */
        SPI1IMSKbits.RXMSK = 1U;
        SPI1IMSKbits.RXWIEN = 1U;

        /* Configure SPI to generate transmit interrupt when the transmit buffer is empty*/
        SPI1IMSKbits.SPITBEN = 1U;

        /* Disable the receive interrupt */
        IEC1bits.SPI1RXIE = 0U;

        /* Disable the transmit interrupt */
        IEC2bits.SPI1TXIE = 0U;

        /* Disable the error interrupt */
        IEC2bits.SPI1EIE = 0U;

        /* Clear the receive interrupt flag */
        IFS1bits.SPI1RXIF = 0U;

        /* Clear the transmit interrupt flag */
        IFS2bits.SPI1TXIF = 0U;

        /* Clear the error interrupt flag */
        IFS2bits.SPI1EIF = 0U;

        /* Start the first write here itself, rest will happen in ISR context */
        SPI_FIFO_Fill();

        if (rxSize > 0U)
        {
            if(rxSize < SPI1_FIFO_SIZE)
            {
                SPI1IMSKbits.RXMSK = rxSize;
            }
            else
            {
                SPI1IMSKbits.RXMSK = SPI1_FIFO_SIZE;
            }
            /* Enable receive interrupt to complete the transfer in ISR context.
             * Keep the transmit interrupt disabled. Transmit interrupt will be
             * enabled later if txCount < txSize, when rxCount = rxSize.
             */
            IEC1bits.SPI1RXIE = 1U;
        }
        else
        {
            if (spi1Obj.txCount != txSz)
            {
                /* Configure SPI to generate interrupt when transmit buffer is completely empty */
                SPI1IMSKbits.SPITBEN = 1U;

                /* ignore receive overflow for transmit only operation*/
                SPI1CON1bits.IGNROV = 1U;

                /* Enable transmit interrupt to complete the transfer in ISR context */
                IEC2bits.SPI1TXIE = 1U;

            }
            else
            {
                /* Enable error interrupt for SRMT(last byte transfer in shift register)*/
                SPI1IMSKbits.SRMTEN = 1U;
                IEC2bits.SPI1EIE = 1U;
            }

        }
    }

    return isRequestAccepted;
}

void __attribute__((used)) SPI1RX_InterruptHandler (void)
{
    uint32_t nRxPending;

    /* Check Receive Buffer Element Count for watermark interrupt */
    if ((SPI1STAT & _SPI1STAT_RXELM_MASK) != 0U)
    {
        size_t txCount = spi1Obj.txCount;
        size_t rxCount = spi1Obj.rxCount;
        while(SPI1STATbits.SPIRBE == false)
        {
            if (rxCount < spi1Obj.rxSize)
            {
                /* Receive buffer is not empty. Read the received data. */
                ((uint8_t*)spi1Obj.rxBuffer)[rxCount] = SPI1BUF;
                rxCount++;

                spi1Obj.rxCount = rxCount;

                if (rxCount == spi1Obj.rxSize)
                {
                    if (txCount < spi1Obj.txSize)
                    {
                        /* Reception of all bytes is complete. However, there are few more
                         * bytes to be transmitted as txCount != txSize. Finish the
                         * transmission of the remaining bytes from the transmit interrupt. */

                        /* Disable the receive interrupt */
                        IEC1bits.SPI1RXIE = 0U;

                        /* Generate TX interrupt when buffer is completely empty */
                        SPI1IMSKbits.SPITBEN = 1U;

                        /* Enable the transmit interrupt. Callback will be given from the
                         * transmit interrupt, when all bytes are shifted out. */
                        IEC2bits.SPI1TXIE = 1U;
                    }
                }
            }
        }
        if (rxCount < spi1Obj.rxSize)
        {
            /* More bytes pending to be received .. */
            SPI_FIFO_Fill();

            nRxPending = spi1Obj.rxSize - rxCount;

            if(nRxPending <= SPI1_FIFO_SIZE)
            {
                SPI1IMSKbits.RXMSK = nRxPending;
            }
            else
            {
                SPI1IMSKbits.RXMSK = SPI1_FIFO_SIZE;
            }
        }
        else
        {
            if(rxCount == spi1Obj.rxSize)
            {
                if (txCount == spi1Obj.txSize)
                {
                    /* Clear receiver overflow error if any */
                    SPI1STATbits.SPIROV = 0U;

                    /* Disable receive interrupt */
                    IEC1bits.SPI1RXIE = 0U;

                    /* Transfer complete. Give a callback */
                    spi1Obj.transferIsBusy = false;

                    if(spi1Obj.callback != NULL)
                    {
                        uintptr_t context = spi1Obj.context;
                        spi1Obj.callback(context);
                    }
                }
            }
        }
    }

    /* Clear SPI1 RX Interrupt flag */
    /* This flag should cleared only after reading buffer */
    IFS1bits.SPI1RXIF = 0U;
}

void __attribute__((used)) SPI1E_InterruptHandler(void)
{
    size_t txCount = spi1Obj.txCount;
    if (txCount == spi1Obj.txSize)
    {
        if ((SPI1STAT & _SPI1STAT_SRMT_MASK) != 0U)
        {
            /* Clear receiver overflow error if any */
            SPI1STATbits.SPIROV = 0U;

            /* Disable transmit interrupt */
            IEC2bits.SPI1TXIE = 0U;

            /* Transfer complete. Give a callback */
            spi1Obj.transferIsBusy = false;

            if(spi1Obj.callback != NULL)
            {
                uintptr_t context = spi1Obj.context;
                spi1Obj.callback(context);
            }
        }
    }

    IFS2bits.SPI1EIF = 0U;
}

void __attribute__((used)) SPI1TX_InterruptHandler (void)
{
    /* If there are more words to be transmitted, then transmit them here and keep track of the count */
    if((SPI1STAT & _SPI1STAT_SPITBE_MASK) == _SPI1STAT_SPITBE_MASK)
    {
        size_t txCount = spi1Obj.txCount;

        while ((txCount < spi1Obj.txSize) && (!SPI1STATbits.SPITBF))
        {
            SPI1BUF = ((uint8_t*)spi1Obj.txBuffer)[txCount];
            txCount++;

            spi1Obj.txCount = txCount;
            if (txCount == spi1Obj.txSize)
            {
                /* All bytes are submitted to the SPI module. Now, enable transmit
                 * interrupt for the shift register to empty  */
                SPI1IMSKbits.SPITBEN = 0U;
                SPI1IMSKbits.SRMTEN = 1U;

                /* Enable the error interrupt and disable the transmit interrupt*/
                IEC2bits.SPI1EIE = 1U;
                IEC2bits.SPI1TXIE = 0U;
            }

        }
    }
    /* Clear the transmit interrupt flag */
    IFS2bits.SPI1TXIF = 0U;
}

