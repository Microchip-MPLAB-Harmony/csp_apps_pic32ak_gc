/*******************************************************************************
  sent2 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_sent2.c
 
  Summary:
    sent2 PLIB Source File
 
  Description:
    None
 
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

// Section: Included Files

#include <stddef.h>
#include <stdbool.h>
#include "device.h"
#include "plib_sent2_rx.h"

static volatile bool bDataReceived = false;


#define SENT2CON1_NIBCNT_ONE      ((uint32_t)(_SENT2CON1_NIBCNT_MASK & ((uint32_t)(1) << _SENT2CON1_NIBCNT_POSITION))) 
#define SENT2CON1_NIBCNT_TWO      ((uint32_t)(_SENT2CON1_NIBCNT_MASK & ((uint32_t)(2) << _SENT2CON1_NIBCNT_POSITION))) 
#define SENT2CON1_NIBCNT_THREE      ((uint32_t)(_SENT2CON1_NIBCNT_MASK & ((uint32_t)(3) << _SENT2CON1_NIBCNT_POSITION))) 
#define SENT2CON1_NIBCNT_FOUR      ((uint32_t)(_SENT2CON1_NIBCNT_MASK & ((uint32_t)(4) << _SENT2CON1_NIBCNT_POSITION))) 
#define SENT2CON1_NIBCNT_FIVE      ((uint32_t)(_SENT2CON1_NIBCNT_MASK & ((uint32_t)(5) << _SENT2CON1_NIBCNT_POSITION))) 
#define SENT2CON1_NIBCNT_SIX      ((uint32_t)(_SENT2CON1_NIBCNT_MASK & ((uint32_t)(6) << _SENT2CON1_NIBCNT_POSITION))) 

// Section: SENT2 Module APIs

void SENT2_Initialize(void)
{
    SENT2CON1 = (SENT2CON1_NIBCNT_TWO
			|_SENT2CON1_CRCEN_MASK
			|_SENT2CON1_RCVEN_MASK);
			
	/* SYNC MAX */
	SENT2CON2 = 0x73UL;
	/* SYNC MIN */
	SENT2CON3 = 0x4dUL;
	
}

void SENT2_Deinitialize(void)
{
    SENT2_Disable(); 
	
	IFS3bits.SENT2IF= 0U;
	IEC3bits.SENT2IE = 0U;
	
	IFS3bits.SENT2EIF = 0U;
	IEC3bits.SENT2EIE = 0U;
	
	/* De-initializing registers to POR values */
	SENT2CON1 = 0x6UL;
	SENT2CON2 = 0xFFFFUL;
	SENT2CON3 = 0xFFFFUL;
}

void SENT2_Enable(void)
{
  SENT2CON1bits.ON = 1U;
}

void SENT2_Disable(void)
{
  SENT2CON1bits.ON = 0U;
}

SENT_DATA_RECEIVE SENT2_Receive(void)
{
    SENT_DATA_RECEIVE sentData = {0};
    sentData.status =   SENT2DATbits.STAT;
    sentData.data1  =   SENT2DATbits.DATA1;
    sentData.data2  =   SENT2DATbits.DATA2;
    return sentData;
}

bool SENT2_IsDataReceived(void)
{
   while(IFS3bits.SENT2IF == 0U)
   {
   }
   IFS3bits.SENT2IF = 0U;
   return true;
}

SENT_RECEIVE_STATUS SENT2_ReceiveStatusGet(void)
{
    return (SENT_RECEIVE_STATUS)SENT2STAT;
}

SENT_ERROR_CODE ReceiveErrorGet(void)
{
	if(IFS3bits.SENT2EIF == 0x1)
	{
		SENT_ERROR_CODE errorCode = NO_ERROR;
		
		if(SENT1STATbits.CRCERR == 0x1)
		{
			SENT1STATbits.CRCERR = 0;
			errorCode = CRC_ERROR;
		}
		else if(SENT1STATbits.FRMERR == 0x1)
		{
			SENT1STATbits.FRMERR = 0;
			errorCode = FRAME_ERROR;
		}
		else if(SENT1STATbits.RXIDLE == 0x1)
		{
			SENT1STATbits.RXIDLE = 0;
			errorCode = RX_IDLE_ERROR;
		}
		else
		{
			//Nothing to process
		}
		IFS3bits.SENT2EIF = 0;
		return errorCode;
	}
	return NO_ERROR;
}
			

/**
 End of File
*/