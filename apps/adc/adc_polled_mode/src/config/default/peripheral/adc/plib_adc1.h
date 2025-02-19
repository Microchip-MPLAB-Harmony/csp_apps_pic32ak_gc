/*******************************************************************************
  adc1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_adc1.h
 
  Summary:
    adc1 PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    adc1 peripheral.
 
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

#ifndef PLIB_ADC1_H
#define PLIB_ADC1_H

// Section: Included Files

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "plib_adc_common.h"

// /cond IGNORE_THIS
/* Provide C++ Compatibility */
#ifdef __cplusplus

    extern "C" {

#endif
// /endcond

// Section: Data Types

/**
 @enum     ADC1_CHANNEL
 @brief    Defines the ADC channles that are selected
*/
typedef enum
{
    ADC1_CHANNEL0 = 0U,   
    ADC1_MAX_CHANNELS = 1
} ADC1_CHANNEL;

/**
 @enum     ADC_PWM_INSTANCE
 @brief    Defines the ADC PWM trigger sources that are 
           available for the module to use.
*/
typedef enum 
{
    ADC1_PWM1 = 1,
    ADC1_PWM2 = 2,
    ADC1_PWM3 = 3,
    ADC1_PWM4 = 4,
} ADC1_PWM_INSTANCE;


// Section: Driver Interface Functions

/**
 * @brief    Initializes ADC1 module, using the given initialization data
 *           This function must be called before any other ADC1 function is called
 * @param    none
 * @return   none  
 */
void ADC1_Initialize (void);

/**
 * @brief    Deinitializes the ADC1 to POR values
 * @param    none
 * @return   none  
 */
void ADC1_Deinitialize(void);

/**
 * @brief    This inline function enables the ADC1 module
 * @pre      none
 * @param    none
 * @return   none  
 */
inline static void ADC1_Enable(void)
{
    AD1CONbits.ON = 1;
    AD1CONbits.MODE = 1;
    while(AD1CONbits.ADRDY == 0U){};
}

/**
 * @brief    This inline function disables the ADC1 module
 * @pre      none
 * @param    none
 * @return   none  
 */
inline static void ADC1_Disable(void)
{
   AD1CONbits.ON = 0;
}

/**
 * @brief    This inline function sets software common trigger
 * @pre      none
 * @param    none
 * @return   none  
 */
inline static void ADC1_SoftwareTriggerEnable(void)
{
   AD1SWTRG = 0xFFFFFFFFU;
}

/**
 * @brief       This inline function sets individual software trigger
 * @pre         none
 * @param[in]   channel - Channel for conversion
 * @return      none  
 */
inline static void ADC1_ChannelSoftwareTriggerEnable(ADC1_CHANNEL channel)
{
    switch(channel)
    {
        case ADC1_CHANNEL0:
                AD1SWTRGbits.CH0TRG = 0x1U;
                break;
        default:
                break;
    }
}

/**
 * @brief      Returns the conversion value for the channel selected
 * @pre        This inline function returns the conversion value only after the conversion is complete. 
 *             Conversion completion status can be checked using 
 *             \ref ADC1_IsConversionComplete(channel) function.
 * @param[in]  channel - Selected channel  
 * @return     Returns the analog to digital converted value  
 */
inline static uint32_t ADC1_ChannelResultGet(ADC1_CHANNEL channel)
{
    uint32_t result = 0x0U;

    switch(channel)
    {
        case ADC1_CHANNEL0:
                result = AD1CH0DATA;
                break;
        default:
                break;
    }
    return result;
}

/**
 * @brief      This inline function returns the status of conversion.This function is used to 
 *             determine if conversion is completed. When conversion is complete 
 *             the function returns true otherwise false.
 * @pre        \ref ADC1_SoftwareTriggerEnable() function should have been 
 *             called before calling this function.
 * @param[in]  channel - Selected channel  
 * @return     true - Conversion is complete.
 * @return     false - Conversion is not complete.  
 */
inline static bool ADC1_ChannelResultIsReady(ADC1_CHANNEL channel)
{
    bool status = false;

    switch(channel)
    {
        case ADC1_CHANNEL0:
                status = AD1STATbits.CH0RDY;
                break;
        default:
                break;
    }
    return status;
}

/**
 * @brief      This inline function enables individual channel interrupt
 * @param[in]  channel - Selected channel  
 * @return     none  
 */
inline static void ADC1_ChannelResultInterruptEnable(ADC1_CHANNEL channel)
{
    switch(channel)
    {
        case ADC1_CHANNEL0:
                IEC4bits.AD1CH0IE = 1;
                break;
        default:
                break;
    }
}

/**
 * @brief      This inline function disables individual channel interrupt
 * @param[in]  channel - Selected channel  
 * @return     none  
 */
inline static void ADC1_ChannelResultInterruptDisable(ADC1_CHANNEL channel)
{
    switch(channel)
    {
        case ADC1_CHANNEL0:
                IEC4bits.AD1CH0IE = 0;
                break;
        default:
                break;
    }
}

/**
 * @brief      This inline function clears individual channel interrupt flag
 * @pre        The flag is not cleared without reading the data from buffer.
 *             Hence call \ref ADC1_ConversionResultGet() function to read data 
 *             before calling this function
 * @param[in]  channel - Selected channel  
 * @return     none  
 */
inline static void ADC1_ChannelResultFlagClear(ADC1_CHANNEL channel)
{
    switch(channel)
    {
        case ADC1_CHANNEL0:
                IFS4bits.AD1CH0IF = 0;
                break;
        default:
                break;
    }
}

/**
 * @brief      This inline function returns the status of the comparator
 * @pre        none
 * @param[in]  channel - Selected channel  
 * @return     compare status  
 */
inline static bool ADC1_CompareStatusGet(ADC1_CHANNEL channel)
{
    bool status;
    switch(channel)
    {
        case ADC1_CHANNEL0:
                status = AD1CMPSTATbits.CH0CMP;
                //Clear status flag
                AD1CMPSTATbits.CH0CMP = 0U;
                //clear the CMP 0 interrupt flag
                IFS4bits.AD1CMP0IF = 0U;
                break;
        default:
                break;
    }
    return status;
}

/**
 * @brief      This inline function allows selection of priority for individual channel interrupt
 * @param[in]  channel - Selected channel 
 * @param[in]  priorityValue  -  The numerical value of interrupt priority
 * @return     none  
 */
inline static void ADC1_IndividualChannelInterruptPrioritySet(ADC1_CHANNEL channel, INTERRUPT_PRIORITY priorityValue)
{
	switch(channel)
	{
		case ADC1_CHANNEL0:
				_AD1CH0IP = priorityValue;
				break;
		default:
				break;
	}
}

/**
 * @brief      This function can be used to define custom callback for ADC1 Channel event. 
 *             Read the conversion result of the corresponding channel in the custom callback.
 * @pre        none
 * @param[in]  channel - Selected channel.  
 * @param[in]  callback - Address of the channel callback function.  
 * @param[in]  context - A value (usually a pointer) passed (unused) into the function identified by the callback parameter.  
 * @return     none  
 */
void ADC1_ChannelCallbackRegister(ADC1_CHANNEL channel,ADC_CHANNEL_CALLBACK callback,uintptr_t context);

/**
 * @brief      This function can be used to define custom callback for ADC1 Comparator event
 * @pre        none
 * @param[in]  channel - Selected channel.  
 * @param[in]  callback - Address of the comparator callback function.  
 * @param[in]  context - A value (usually a pointer) passed (unused) into the function identified by the callback parameter.  
 * @return     none  
 */
void ADC1_ComparatorCallbackRegister(ADC1_CHANNEL channel,ADC_CMP_CALLBACK callback,uintptr_t context);

/**
 * @brief    Calibrates the ADC1 Core
 * @pre      none 
 * @param    none
 * @return   none  
 */
void ADC1_SharedCoreCalibration(void);

/**
 * @brief    Sets Trigger source as PWM Trigger 
 * @pre      PWM must be enabled and configured 
 * @param[in]  channel - Selected channel. 
 * @param[in]  pwmInstance - Instance of PWM. Refer Datasheet for available PWMs as trigger source
 * @param[in]  triggerNumber - Selection between Trigger 1 or Trigger 2 
 * @return   none  
 */
void ADC1_PWMTriggerSourceSet(ADC1_CHANNEL channel, ADC1_PWM_INSTANCE pwmInstance, ADC_PWM_TRIGGERS triggerNumber);

#endif //PLIB_ADC1_H
    
/**
 End of File
*/

