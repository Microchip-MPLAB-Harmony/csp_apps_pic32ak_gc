/*******************************************************************************
  sent1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_sent1.c
 
  Summary:
    sent1 PLIB Source File
 
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

#include <stddef.h>
#include <stdbool.h>
#include "device.h"
#include "plib_sent1_tx.h"

/*indicates the status of data transmission*/
static volatile bool bDataTransmitted = true;


#define SENT1CON1_NIBCNT_ONE      ((uint32_t)(_SENT1CON1_NIBCNT_MASK & ((uint32_t)(1) << _SENT1CON1_NIBCNT_POSITION))) 
#define SENT1CON1_NIBCNT_TWO      ((uint32_t)(_SENT1CON1_NIBCNT_MASK & ((uint32_t)(2) << _SENT1CON1_NIBCNT_POSITION))) 
#define SENT1CON1_NIBCNT_THREE      ((uint32_t)(_SENT1CON1_NIBCNT_MASK & ((uint32_t)(3) << _SENT1CON1_NIBCNT_POSITION))) 
#define SENT1CON1_NIBCNT_FOUR      ((uint32_t)(_SENT1CON1_NIBCNT_MASK & ((uint32_t)(4) << _SENT1CON1_NIBCNT_POSITION))) 
#define SENT1CON1_NIBCNT_FIVE      ((uint32_t)(_SENT1CON1_NIBCNT_MASK & ((uint32_t)(5) << _SENT1CON1_NIBCNT_POSITION))) 
#define SENT1CON1_NIBCNT_SIX      ((uint32_t)(_SENT1CON1_NIBCNT_MASK & ((uint32_t)(6) << _SENT1CON1_NIBCNT_POSITION))) 

// Section: SENT1 Module APIs

void SENT1_Initialize(void)
{
    SENT1CON1 = (SENT1CON1_NIBCNT_TWO
			|_SENT1CON1_CRCEN_MASK);
	//Tick Time
	SENT1CON2 = 0xbUL;
	//Frame Time
	SENT1CON3 = 0x0UL;
	
}

void SENT1_Deinitialize(void)
{
    SENT1_Disable(); 
	
	IFS3bits.SENT1IF = 0U;
	IEC3bits.SENT1IE = 0U;
	
	IFS3bits.SENT1EIF = 0U;
	IEC3bits.SENT1EIE = 0U;
	
	/* De-initializing registers to POR values */
	SENT1CON1 = 0x6UL;
	SENT1CON2 = 0xFFFFUL;
	SENT1CON3 = 0xFFFFUL;
}

void SENT1_Enable(void)
{
  SENT1CON1bits.ON = 1U;
}

void SENT1_Disable(void)
{
  SENT1CON1bits.ON = 0U;
}

void SENT1_TransmitModeSet(SENT_TRANSMIT_MODE mode)
{
  SENT1CON1bits.TXM = mode;
}

void SENT1_Transmit(const SENT_DATA_TRANSMIT *sentData)
{
    SENT1DATbits.STAT = sentData->status;
    SENT1DATbits.DATA1 = sentData->data1;
    SENT1DATbits.DATA2 = sentData->data2;
    if(SENT1CON1bits.TXM == 1U)
    {
        SENT1STATbits.SYNCTXEN = 1U;
    }
}

bool SENT1_IsTransmissionComplete(void)
{
    while(IFS3bits.SENT1IF == 0U)
    {
      
    }
	IFS3bits.SENT1IF = 0U;
    return true;
}

SENT_TRANSMIT_STATUS SENT1_TransmitStatusGet(void)
{
    return (SENT1STAT);
}


/**
 End of File
*/