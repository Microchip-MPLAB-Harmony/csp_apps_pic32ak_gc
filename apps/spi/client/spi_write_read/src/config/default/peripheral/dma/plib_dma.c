/*******************************************************************************
  Direct Memory Access Controller (DMA) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_dma.c

  Summary
    Source for DMA peripheral library interface Implementation.

  Description
    This file defines the interface to the DMA peripheral library. This
    library provides access to and control of the DMA controller.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2014 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_dma.h"
#include "interrupts.h"

//SPI DMAxCH Data Size Selection options
#define DMA0CH_SIZE_ONE_BYTE_WORD          ((uint32_t)(_DMA0CH_SIZE_MASK & ((uint32_t)(0) << _DMA0CH_SIZE_POSITION))) 
#define DMA0CH_SIZE_16_BIT_WORD          ((uint32_t)(_DMA0CH_SIZE_MASK & ((uint32_t)(1) << _DMA0CH_SIZE_POSITION))) 
#define DMA0CH_SIZE_32_BIT_WORD          ((uint32_t)(_DMA0CH_SIZE_MASK & ((uint32_t)(2) << _DMA0CH_SIZE_POSITION))) 

//SPI DMAxCH Transfer Mode Selection options
#define DMA0CH_TRMODE_ONE_SHOT          ((uint32_t)(_DMA0CH_TRMODE_MASK & ((uint32_t)(0) << _DMA0CH_TRMODE_POSITION))) 
#define DMA0CH_TRMODE_REPEATED_ONE_SHOT          ((uint32_t)(_DMA0CH_TRMODE_MASK & ((uint32_t)(1) << _DMA0CH_TRMODE_POSITION))) 
#define DMA0CH_TRMODE_CONTINUOUS          ((uint32_t)(_DMA0CH_TRMODE_MASK & ((uint32_t)(2) << _DMA0CH_TRMODE_POSITION))) 
#define DMA0CH_TRMODE_REPEATED_CONTINUOUS          ((uint32_t)(_DMA0CH_TRMODE_MASK & ((uint32_t)(3) << _DMA0CH_TRMODE_POSITION))) 

// DMAxCH Source Address Mode Selection Options
#define DMA0CH_SAMODE_UNCHANGED          ((uint16_t)(_DMA0CH_SAMODE_MASK & ((uint16_t)(0) << _DMA0CH_SAMODE_POSITION)))
#define DMA0CH_SAMODE_INCREMENTED          ((uint16_t)(_DMA0CH_SAMODE_MASK & ((uint16_t)(1) << _DMA0CH_SAMODE_POSITION)))
#define DMA0CH_SAMODE_DECREMENTED          ((uint16_t)(_DMA0CH_SAMODE_MASK & ((uint16_t)(2) << _DMA0CH_SAMODE_POSITION)))

// DMAxCH Destination Address Mode Selection Options
#define DMA0CH_DAMODE_UNCHANGED          ((uint16_t)(_DMA0CH_DAMODE_MASK & ((uint16_t)(0) << _DMA0CH_DAMODE_POSITION)))
#define DMA0CH_DAMODE_INCREMENTED          ((uint16_t)(_DMA0CH_DAMODE_MASK & ((uint16_t)(1) << _DMA0CH_DAMODE_POSITION)))
#define DMA0CH_DAMODE_DECREMENTED          ((uint16_t)(_DMA0CH_DAMODE_MASK & ((uint16_t)(2) << _DMA0CH_DAMODE_POSITION)))


// Section: Global Data

void DMA0_InterruptHandler (void);

volatile static DMA_CHANNEL_OBJECT  dmaChannelObj[DMA_NUMBER_OF_CHANNELS];

// Section: DMA PLib Interface Implementations

void DMA_Initialize( void )
{
    /* Enable the DMA module */
    DMACONbits.ON = 1U;
    
    /* Initialize the available channel objects */

    dmaChannelObj[DMA_CHANNEL_0].inUse      =    false;
    dmaChannelObj[DMA_CHANNEL_0].callback   =    NULL;
    dmaChannelObj[DMA_CHANNEL_0].context    =    (uintptr_t)NULL;

    
    DMALOW = 0x4000UL;

    DMAHIGH = 0x8000UL;

    DMA0CH = (DMA0CH_SAMODE_UNCHANGED
         | DMA0CH_DAMODE_UNCHANGED
         | DMA0CH_TRMODE_ONE_SHOT
         | DMA0CH_SIZE_ONE_BYTE_WORD 
         | _DMA0CH_DONEEN_MASK);

    DMA0SEL = (uint32_t)(0x0 << _DMA0SEL_CHSEL_POSITION);

/* Enable DMA channel interrupts */
    // Clearing Channel 0 Interrupt Flag;
    _DMA0IF = 0U;  
    // Enabling Channel 0 Interrupt
    _DMA0IE = 1U;

}

void DMA_Deinitialize( void )
{
    /* Disable DMA channel interrupts */
    // Clearing Channel 0 Interrupt Flag;
    _DMA0IF = 0U;  
    // Enabling Channel 0 Interrupt
    _DMA0IE = 0U;

    //Disable DMA Channel 0
    DMA0CHbits.CHEN = 0U;
    /* Disable the DMA module */
    DMACONbits.ON = 0U;

    DMACON = 0x0UL;

    DMABUF = 0x0UL;

    DMALOW = 0x0UL;

    DMAHIGH = 0x0UL;

    DMA0CH = 0x0UL;
    DMA0SEL = 0x0UL;
    DMA0STAT = 0x0UL;
    DMA0SRC = 0x0UL;
    DMA0DST = 0x0UL;
    DMA0CNT = 0x1UL;
    DMA0MSK = 0x0UL;
    DMA0PAT = 0x0UL;


}

bool DMA_ChannelTransfer(DMA_CHANNEL channel, const void *srcAddr, const void *destAddr, uint32_t blockSize)
{
    bool returnStatus = false;
    const uint32_t *XsrcAddr  = (const uint32_t *)srcAddr;
    const uint32_t *XdestAddr = (const uint32_t *)destAddr;  
    
    if(dmaChannelObj[channel].inUse == false)
    {
        switch (channel) 
        {
            case DMA_CHANNEL_0:
                DMA0SRC = (uint32_t)XsrcAddr;
                DMA0DST = (uint32_t)XdestAddr;
                DMA0CNT = blockSize;
                dmaChannelObj[channel].inUse = true;
                returnStatus = true;

                //Enable DMA Channel 0
                DMA0CHbits.CHEN = 1;
                break;
                
            default:break;
        }
    }

    return returnStatus;
}

void DMA_ChannelPatternMatchSetup(DMA_CHANNEL channel, uint32_t patternMatchMask, uint32_t patternMatchData)
{
    switch (channel) 
    {
        case DMA_CHANNEL_0:
        DMA0MSK = patternMatchMask;
        DMA0PAT = patternMatchData;
        
        /* Enable Pattern Match */
        DMA0CHbits.MATCHEN = 1U;
        break;
        
        default:break;
    }
}

void DMA_ChannelEnable(DMA_CHANNEL channel) 
{
    switch (channel) 
    {
        case DMA_CHANNEL_0:
            DMA0CHbits.CHEN = 1U;
            dmaChannelObj[channel].inUse = true;
            break;
            
        default:break;
    }
}

void DMA_ChannelDisable (DMA_CHANNEL channel)
{
    switch (channel) 
    {
        case DMA_CHANNEL_0:
            DMA0CHbits.CHEN = 0U;
            dmaChannelObj[channel].inUse = false;
            break;
            
        default: break;
    }
}

void DMA_ChannelPatternMatchEnable(DMA_CHANNEL channel)
{
    switch (channel) 
    {
        case DMA_CHANNEL_0:
            DMA0CHbits.MATCHEN = 1U;
            break;
            
        default:break;
    }
}

void DMA_ChannelPatternMatchDisable(DMA_CHANNEL channel)
{
    switch (channel) 
    {
        case DMA_CHANNEL_0:
            DMA0CHbits.MATCHEN = 0U;
            break;
            
        default:break;
    }
}

bool DMA_IsSoftwareRequestPending(DMA_CHANNEL channel)
{
    bool status = false;
    switch (channel) 
    {
        case DMA_CHANNEL_0:
                status = DMA0CHbits.CHREQ;
                break;
                
        default: break;
    }
    return status;
}
 
void DMA_ChannelSoftwareTriggerEnable(DMA_CHANNEL channel) 
{
    switch (channel) 
    {
        case DMA_CHANNEL_0:
            DMA0CHbits.CHREQ = 1U;
            break;
            
        default:break;
    }
}
 
uint32_t DMA_ChannelGetTransferredCount(DMA_CHANNEL channel) 
{
    uint32_t  count = 0;
    switch (channel) 
    {
        case DMA_CHANNEL_0:
            count = DMA0CNT;
            break;
            
        default:break;
    }
    return count;
}

bool DMA_ChannelIsBusy (DMA_CHANNEL channel)
{
    bool busy_check = false;
    switch (channel) 
    {
        case DMA_CHANNEL_0:
            if ((DMA0STATbits.DONE == 0) && (dmaChannelObj[0].inUse == true))
            {
                busy_check = true;
            }
            break;    
                    
        default:
            break;
    }
    return busy_check;
}

void DMA_ChannelCallbackRegister(DMA_CHANNEL channel, const DMA_CHANNEL_CALLBACK eventHandler, uintptr_t contextHandle)
{
    dmaChannelObj[channel].callback  = eventHandler;

    dmaChannelObj[channel].context   = contextHandle;
}

void __attribute__((used)) DMA0_InterruptHandler (void)
{
    volatile DMA_CHANNEL_OBJECT *chanObj;
    DMA_TRANSFER_EVENT dmaEvent = DMA_TRANSFER_EVENT_NONE;

    /* Clear the interrupt flag*/
    _DMA0IF = 0U;
    
    /* Find out the channel object */
    chanObj = &dmaChannelObj[0];
    
    if(DMA0STATbits.OVERRUN == 1U)  
    {
        dmaEvent = DMA_OVERRUN_ERROR;
        DMA0STATbits.OVERRUN = 0;
    }
    else if(DMA0STATbits.MATCH == 1U)
    {
        dmaEvent = DMA_PATTERN_MATCH;
        DMA0STATbits.MATCH = 0U;
    }
    else if(DMA0STATbits.DONE == 1U)
    {
        dmaEvent = DMA_TRANSFER_EVENT_COMPLETE;
        DMA0STATbits.DONE = 0U;
    }
    else if(DMA0STATbits.HALF == 1U)
    {
        dmaEvent = DMA_TRANSFER_EVENT_HALF_COMPLETE;
        DMA0STATbits.HALF = 0U;
    }
    else 
    {
        // nothing to process
    }
    
    if((chanObj->callback != NULL) && (dmaEvent != DMA_TRANSFER_EVENT_NONE))
    {
        uintptr_t context = chanObj->context;

        chanObj->callback(dmaEvent, context);
    }
}

