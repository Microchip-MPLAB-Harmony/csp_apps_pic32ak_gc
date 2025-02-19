/*******************************************************************************
  adc1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_adc1.c
 
  Summary:
    adc1 PLIB Source File
 
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
#include "plib_adc_common.h"
#include "plib_adc1.h"

// Section: Macro Definitions

//ADC AD1CON MODE options
#define AD1CON_MODE_POWERED_DOWN          ((uint32_t)(_AD1CON_MODE_MASK & ((uint32_t)(0) << _AD1CON_MODE_POSITION))) 
#define AD1CON_MODE_STANDBY          ((uint32_t)(_AD1CON_MODE_MASK & ((uint32_t)(1) << _AD1CON_MODE_POSITION))) 
#define AD1CON_MODE_ON          ((uint32_t)(_AD1CON_MODE_MASK & ((uint32_t)(2) << _AD1CON_MODE_POSITION))) 

//ADC AD1CON RPTCNT set
#define AD1CON_RPTCNT_SET(value)              ((uint32_t)(_AD1CON_RPTCNT_MASK & ((uint32_t)(value) << _AD1CON_RPTCNT_POSITION))) 

//ADC AD1CH0CON MODE options
#define AD1CH0CON_MODE_SINGLE_SAMPLE          ((uint32_t)(_AD1CH0CON_MODE_MASK & ((uint32_t)(0) << _AD1CH0CON_MODE_POSITION))) 
#define AD1CH0CON_MODE_WINDOW          ((uint32_t)(_AD1CH0CON_MODE_MASK & ((uint32_t)(1) << _AD1CH0CON_MODE_POSITION))) 
#define AD1CH0CON_MODE_INTEGRATION          ((uint32_t)(_AD1CH0CON_MODE_MASK & ((uint32_t)(2) << _AD1CH0CON_MODE_POSITION))) 
#define AD1CH0CON_MODE_OVERSAMPLING          ((uint32_t)(_AD1CH0CON_MODE_MASK & ((uint32_t)(3) << _AD1CH0CON_MODE_POSITION))) 

//ADC AD1CH0CON ACCNUM options
#define AD1CH0CON_ACCNUM_4_SAMPLES          ((uint32_t)(_AD1CH0CON_ACCNUM_MASK & ((uint32_t)(0) << _AD1CH0CON_ACCNUM_POSITION))) 
#define AD1CH0CON_ACCNUM_16_SAMPLES          ((uint32_t)(_AD1CH0CON_ACCNUM_MASK & ((uint32_t)(1) << _AD1CH0CON_ACCNUM_POSITION))) 
#define AD1CH0CON_ACCNUM_64_SAMPLES          ((uint32_t)(_AD1CH0CON_ACCNUM_MASK & ((uint32_t)(2) << _AD1CH0CON_ACCNUM_POSITION))) 
#define AD1CH0CON_ACCNUM_256_SAMPLES          ((uint32_t)(_AD1CH0CON_ACCNUM_MASK & ((uint32_t)(3) << _AD1CH0CON_ACCNUM_POSITION))) 

//ADC AD1CH0CON TRG2SRC set
#define AD1CH0CON_TRG2SRC_SET(value)              ((uint32_t)(_AD1CH0CON_TRG2SRC_MASK & ((uint32_t)(value) << _AD1CH0CON_TRG2SRC_POSITION))) 

//ADC AD1CH0CON CMPMOD options
#define AD1CH0CON_CMPMOD_DISABLED          ((uint32_t)(_AD1CH0CON_CMPMOD_MASK & ((uint32_t)(0) << _AD1CH0CON_CMPMOD_POSITION))) 
#define AD1CH0CON_CMPMOD_OUT_OF_BOUNDS          ((uint32_t)(_AD1CH0CON_CMPMOD_MASK & ((uint32_t)(1) << _AD1CH0CON_CMPMOD_POSITION))) 
#define AD1CH0CON_CMPMOD_IN_BOUNDS          ((uint32_t)(_AD1CH0CON_CMPMOD_MASK & ((uint32_t)(2) << _AD1CH0CON_CMPMOD_POSITION))) 
#define AD1CH0CON_CMPMOD_GRATER_THAN          ((uint32_t)(_AD1CH0CON_CMPMOD_MASK & ((uint32_t)(3) << _AD1CH0CON_CMPMOD_POSITION))) 
#define AD1CH0CON_CMPMOD_LESS_THAN_OR_EQUAL          ((uint32_t)(_AD1CH0CON_CMPMOD_MASK & ((uint32_t)(4) << _AD1CH0CON_CMPMOD_POSITION))) 

//ADC AD1CH0CON PINSEL set
#define AD1CH0CON_PINSEL_SET(value)              ((uint32_t)(_AD1CH0CON_PINSEL_MASK & ((uint32_t)(value) << _AD1CH0CON_PINSEL_POSITION))) 

//ADC AD1CH0CON NINSEL set
#define AD1CH0CON_NINSEL_SET(value)              ((uint32_t)(_AD1CH0CON_NINSEL_MASK & ((uint32_t)(value) << _AD1CH0CON_NINSEL_POSITION))) 

//ADC AD1CH0CON SAMC set
#define AD1CH0CON_SAMC_SET(value)              ((uint32_t)(_AD1CH0CON_SAMC_MASK & ((uint32_t)(value) << _AD1CH0CON_SAMC_POSITION))) 

//ADC AD1CH0CON TRG1SRC set
#define AD1CH0CON_TRG1SRC_SET(value)              ((uint32_t)(_AD1CH0CON_TRG1SRC_MASK & ((uint32_t)(value) << _AD1CH0CON_TRG1SRC_POSITION))) 

//ADC AD1CH0CNT CNT set
#define AD1CH0CNT_CNT_SET(value)              ((uint32_t)(_AD1CH0CNT_CNT_MASK & ((uint32_t)(value) << _AD1CH0CNT_CNT_POSITION))) 


// Section: File specific functions

volatile static ADC_CHANNEL_OBJECT adc1ChannelObj[20];
volatile static ADC_CMP_OBJECT adc1CmpObj[20];


typedef enum {
    PWM1_TRIGGER1 = 4, 
    PWM1_TRIGGER2 = 5, 
    PWM2_TRIGGER1 = 6, 
    PWM2_TRIGGER2 = 7, 
    PWM3_TRIGGER1 = 8, 
    PWM3_TRIGGER2 = 9, 
    PWM4_TRIGGER1 = 10, 
    PWM4_TRIGGER2 = 11, 
}ADC1_PWM_TRIGGERS;

static uint16_t ADC1_TriggerSourceValueGet(ADC1_PWM_INSTANCE pwmInstance, ADC_PWM_TRIGGERS triggerNumber);

// Section: ISR declaration

void AD1CH0_InterruptHandler(void);

// Section: ADC1 Implementation

void ADC1_Initialize(void)
{
    AD1CON = AD1CON_RPTCNT_SET(18);
    //Clear CMP status
    AD1CMPSTAT = 0x0U;
    
    AD1CH0CON = (AD1CH0CON_MODE_SINGLE_SAMPLE
                            |AD1CH0CON_ACCNUM_4_SAMPLES
                            |AD1CH0CON_TRG2SRC_SET(0)
                            |AD1CH0CON_CMPMOD_DISABLED
                            |AD1CH0CON_PINSEL_SET(6)
                            |AD1CH0CON_NINSEL_SET(0)
                            |AD1CH0CON_SAMC_SET(0)
                            |AD1CH0CON_TRG1SRC_SET(1));
    AD1CH0CNT = AD1CH0CNT_CNT_SET(0);
    AD1CH0CMPLO = 0X0UL;
    AD1CH0CMPHI = 0X0UL;
    
    
    //Mode change to run mode and enable ADC
    AD1CON |= (AD1CON_MODE_ON | _AD1CON_ON_MASK);
    while(AD1CONbits.ADRDY == 0U){};
}

void ADC1_Deinitialize(void)
{
    uint32_t  __attribute__ ((unused)) dummy;
    
    ADC1_Disable();
    
    dummy = AD1CH0DATA;
    (void)dummy;
    _AD1CH0IF = 0U;
    _AD1CH0IE = 0U;
    
    AD1CON = 0x4A0000UL;
    AD1DATAOVR = 0x0UL;
    AD1STAT = 0x0UL;
    AD1CMPSTAT = 0x0UL;
    AD1CH0CON = 0x0UL;
    AD1CH0DATA = 0x0UL;
    AD1CH0CNT = 0x0UL;
    AD1CH0CMPLO = 0x0UL;
    AD1CH0CMPHI = 0x0UL;

    
}

void ADC1_SharedCoreCalibration (void) 
{
    AD1CONbits.CALREQ = 1U;    
    while(AD1CONbits.CALRDY == 0U)
    {
    }  
}

static uint16_t ADC1_TriggerSourceValueGet(ADC1_PWM_INSTANCE pwmInstance, ADC_PWM_TRIGGERS triggerNumber)
{
    uint16_t adcTriggerSourceValue = 0x0U;
    switch(pwmInstance)
    {
        case ADC1_PWM4:
                if(triggerNumber == ADC_PWM_TRIGGER_1)
                {
                    adcTriggerSourceValue = PWM4_TRIGGER1;
                }
                else if(triggerNumber == ADC_PWM_TRIGGER_2)
                {
                    adcTriggerSourceValue = PWM4_TRIGGER2;
                }
                else
                {
                }
                break;
         default:
                break;
    }
    return adcTriggerSourceValue;
}

void ADC1_PWMTriggerSourceSet(ADC1_CHANNEL channel, ADC1_PWM_INSTANCE pwmInstance, ADC_PWM_TRIGGERS triggerNumber)
{
    uint16_t adcTriggerValue;
    adcTriggerValue= ADC1_TriggerSourceValueGet(pwmInstance, triggerNumber);
    switch(channel)
    {
        case ADC1_CHANNEL0:
                AD1CH0CONbits.TRG1SRC = adcTriggerValue;
                break;
        default:
                break;
    }
}

void ADC1_ChannelCallbackRegister(ADC1_CHANNEL channel,ADC_CHANNEL_CALLBACK callback,uintptr_t context)
{
    adc1ChannelObj[channel].callback = callback;
    adc1ChannelObj[channel].context = context;
} 

void ADC1_ComparatorCallbackRegister(ADC1_CHANNEL channel,ADC_CMP_CALLBACK callback,uintptr_t context)
{
    adc1CmpObj[channel].callback = callback;
    adc1CmpObj[channel].context = context;
}

void AD1CH0_InterruptHandler(void)
{
    uint32_t valChannel0Data;
    //Read the ADC value from the ADCH0DATA
    valChannel0Data = AD1CH0DATA;
    
    if(adc1ChannelObj[0].callback != NULL)
    {
      adc1ChannelObj[0].callback(valChannel0Data, adc1ChannelObj[0].context);
    }

    //clear the CH 0 interrupt flag
    IFS4bits.AD1CH0IF = 0U;
}


