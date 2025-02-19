/*******************************************************************************
  cmp1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_cmp1.c
 
  Summary:
    cmp1 PLIB Source File
 
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
#include "plib_cmp_common.h"
#include "plib_cmp1.h"

//CMP DACCTRL2 SSTIME set
#define DACCTRL2_SSTIME_SET(value)              ((uint32_t)(_DACCTRL2_SSTIME_MASK & ((uint32_t)(value) << _DACCTRL2_SSTIME_POSITION))) 

//CMP DACCTRL2 TMODTIME set
#define DACCTRL2_TMODTIME_SET(value)              ((uint32_t)(_DACCTRL2_TMODTIME_MASK & ((uint32_t)(value) << _DACCTRL2_TMODTIME_POSITION))) 

//CMP DAC1CON TMCB set
#define DAC1CON_TMCB_SET(value)              ((uint32_t)(_DAC1CON_TMCB_MASK & ((uint32_t)(value) << _DAC1CON_TMCB_POSITION))) 

//CMP DAC1CON IRQM options
#define DAC1CON_IRQM_DISABLED          ((uint32_t)(_DAC1CON_IRQM_MASK & ((uint32_t)(0) << _DAC1CON_IRQM_POSITION))) 
#define DAC1CON_IRQM_RISING          ((uint32_t)(_DAC1CON_IRQM_MASK & ((uint32_t)(1) << _DAC1CON_IRQM_POSITION))) 
#define DAC1CON_IRQM_FALLING          ((uint32_t)(_DAC1CON_IRQM_MASK & ((uint32_t)(2) << _DAC1CON_IRQM_POSITION))) 
#define DAC1CON_IRQM_RISING_OR_FALLING          ((uint32_t)(_DAC1CON_IRQM_MASK & ((uint32_t)(3) << _DAC1CON_IRQM_POSITION))) 

//CMP DAC1CON INSEL options
#define DAC1CON_INSEL_CMPxA          ((uint32_t)(_DAC1CON_INSEL_MASK & ((uint32_t)(0) << _DAC1CON_INSEL_POSITION))) 
#define DAC1CON_INSEL_CMPxB          ((uint32_t)(_DAC1CON_INSEL_MASK & ((uint32_t)(1) << _DAC1CON_INSEL_POSITION))) 
#define DAC1CON_INSEL_CMPxC          ((uint32_t)(_DAC1CON_INSEL_MASK & ((uint32_t)(2) << _DAC1CON_INSEL_POSITION))) 
#define DAC1CON_INSEL_CMPxD          ((uint32_t)(_DAC1CON_INSEL_MASK & ((uint32_t)(3) << _DAC1CON_INSEL_POSITION))) 

//CMP DAC1CON HYSSEL options
#define DAC1CON_HYSSEL_NO_HYS          ((uint32_t)(_DAC1CON_HYSSEL_MASK & ((uint32_t)(0) << _DAC1CON_HYSSEL_POSITION))) 
#define DAC1CON_HYSSEL_15_mV          ((uint32_t)(_DAC1CON_HYSSEL_MASK & ((uint32_t)(1) << _DAC1CON_HYSSEL_POSITION))) 
#define DAC1CON_HYSSEL_30_mV          ((uint32_t)(_DAC1CON_HYSSEL_MASK & ((uint32_t)(2) << _DAC1CON_HYSSEL_POSITION))) 
#define DAC1CON_HYSSEL_45_mV          ((uint32_t)(_DAC1CON_HYSSEL_MASK & ((uint32_t)(3) << _DAC1CON_HYSSEL_POSITION))) 

//CMP DAC1DAT DACDATAH set
#define DAC1DAT_DACDAT_SET(value)              ((uint32_t)(_DAC1DAT_DACDAT_MASK & ((uint32_t)(value) << _DAC1DAT_DACDAT_POSITION))) 

//CMP DAC1DAT DACDATAL set
#define DAC1DAT_DACLOW_SET(value)              ((uint32_t)(_DAC1DAT_DACLOW_MASK & ((uint32_t)(value) << _DAC1DAT_DACLOW_POSITION))) 

//CMP DAC1SLPCON HCFSEL set
#define DAC1SLPCON_HCFSEL_SET(value)              ((uint32_t)(_DAC1SLPCON_HCFSEL_MASK & ((uint32_t)(value) << _DAC1SLPCON_HCFSEL_POSITION))) 

//CMP DAC1SLPCON SLPSTOPA set
#define DAC1SLPCON_SLPSTOPA_SET(value)              ((uint32_t)(_DAC1SLPCON_SLPSTOPA_MASK & ((uint32_t)(value) << _DAC1SLPCON_SLPSTOPA_POSITION))) 

//CMP DAC1SLPCON SLPSTOPB set
#define DAC1SLPCON_SLPSTOPB_SET(value)              ((uint32_t)(_DAC1SLPCON_SLPSTOPB_MASK & ((uint32_t)(value) << _DAC1SLPCON_SLPSTOPB_POSITION))) 

//CMP DAC1SLPCON SLPSTRT set
#define DAC1SLPCON_SLPSTRT_SET(value)              ((uint32_t)(_DAC1SLPCON_SLPSTRT_MASK & ((uint32_t)(value) << _DAC1SLPCON_SLPSTRT_POSITION))) 

#define CMP_FPDMDAC_ADDRESS 0x7F20B0UL

// Section: Global Data

// Section: CMP1 Module APIs

void CMP1_Initialize(void)
{           
    // Comparator Register settings
    DACCTRL1bits.FCLKDIV = 0U; 
    DAC1CON =  (DAC1CON_TMCB_SET(0)
              |DAC1CON_IRQM_DISABLED
              |_DAC1CON_DACOEN_MASK
              |DAC1CON_INSEL_CMPxA
              |DAC1CON_HYSSEL_NO_HYS);
    
    //DAC Settings
    DAC1DAT = (DAC1DAT_DACDAT_SET(0xf32UL)
              |DAC1DAT_DACLOW_SET(0xcdUL));
    
    DAC1SLPCON = (DAC1SLPCON_HCFSEL_SET(0)
              |DAC1SLPCON_SLPSTOPA_SET(0)
              |DAC1SLPCON_SLPSTOPB_SET(0)
              |DAC1SLPCON_SLPSTRT_SET(0));
    
    DAC1SLPDAT = 0x0UL;
    
    DAC1CONbits.DACEN = 1U;
    DACCTRL1bits.ON = 1U;
}

void CMP1_Deinitialize(void)
{ 
    DACCTRL1bits.ON = 0U;
    
    
    // Comparator Register settings
    DACCTRL1 = 0x3F7F0000UL;
    DACCTRL2 = 0x8A0001UL;
    DAC1CON = 0x0UL;
    
    //DAC Settings
    DAC1DAT = 0x0UL;
    DAC1SLPCON = 0x0UL;
    DAC1SLPDAT = 0x0UL;
}

void CMP1_Calibrate(void)
{
    uint32_t *fpdmdac = (uint32_t*)CMP_FPDMDAC_ADDRESS;
    DACCTRL1bits.POSINLADJ = ((*fpdmdac) & _DACCTRL1_POSINLADJ_MASK) >> _DACCTRL1_POSINLADJ_POSITION;
    DACCTRL1bits.NEGINLADJ = ((*fpdmdac) & _DACCTRL1_NEGINLADJ_MASK) >> _DACCTRL1_NEGINLADJ_POSITION;
    DACCTRL1bits.DNLADJ = ((*fpdmdac) & _DACCTRL1_DNLADJ_MASK) >> _DACCTRL1_DNLADJ_POSITION;
    DACCTRL1bits.RREN = 1U;
}


/**
 End of File
*/