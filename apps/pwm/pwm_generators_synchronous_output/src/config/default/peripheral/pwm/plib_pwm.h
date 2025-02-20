/*******************************************************************************
  PWM PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_pwm.h

  Summary:
    PWM PLIB Header File

  Description:
    This file has prototype of all the interfaces provided for particular
    PWM peripheral.

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

#ifndef PWM_H
#define PWM_H

// Section: Included Files

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// *****************************************************************************
// *****************************************************************************
// Section: Enum Declarations
// *****************************************************************************
// *****************************************************************************

/**
 @enum     PWM_GENERATOR
 @brief    Defines the PWM generators that are selected from the MCC Harmony User 
           Interface for the PWM output controls.
 @details  The `PWM_GENERATOR` enum represents the different PWM generators available for configuration 
           through the MCC Harmony User Interface. These values correspond to specific hardware PWM generators 
           that can be used for generating PWM outputs. The generated list in the help documentation may not represent 
           all the channels but is based on the user's configuration in the MCC Harmony tool.
           The user should configure the PWM generators according to their application requirements.
           The generated enum list may vary depending on the specific PWM Generators are enabled in User Interface.
*/
typedef enum
{
    PWM_GENERATOR_1 = 1,      /**< PWM generator 1*/
    PWM_GENERATOR_2 = 2,      /**< PWM generator 2*/
    PWM_GENERATOR_3 = 3,      /**< PWM generator 3*/
    PWM_GENERATOR_4 = 4,      /**< PWM generator 4*/
    PWM_MAX_GENERATOR
}PWM_GENERATOR;

/**
 @enum     PWM_GENERATOR_INTERRUPT
 @brief    Defines the PWM generator interrupts that are available for the module to use.
 @details  Defines the PWM generator interrupts that are available for the module to use.
*/
typedef enum 
{
    PWM_GENERATOR_INTERRUPT_FAULT         =  1,      /**< PWM Generator Fault Interrupt */ 
    PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT =  2,      /**< PWM Generator Current Limit Interrupt */
    PWM_GENERATOR_INTERRUPT_FEED_FORWARD  =  3,      /**< PWM Generator Feed Forward Interrupt */
    PWM_GENERATOR_INTERRUPT_SYNC          =  4,      /**< PWM Generator Sync Interrupt */
}PWM_GENERATOR_INTERRUPT;

/**
 @enum     PWM_TRIGGER_COMPARE
 @brief    Defines the PWM generator Trigger Compare registers 
           that are available for the module to use.
 @details  This enum defines the registers used to trigger compare events for the PWM generator. 
           The trigger compare registers (A, B, C) are used to set the conditions for when the PWM generator should update or trigger an event.
           These registers are essential for controlling the timing and synchronization of the PWM signals and the system's response to those signals.
*/
typedef enum 
{
    PWM_TRIGGER_COMPARE_A = 1,         /**< PWM Trigger Compare A Register */
    PWM_TRIGGER_COMPARE_B = 2,         /**< PWM Trigger Compare B Register */
    PWM_TRIGGER_COMPARE_C = 3,         /**< PWM Trigger Compare C Register */
}PWM_TRIGGER_COMPARE;

/**
 @enum     PWM_MODES
 @brief    Defines the PWM generator operating modes that are available.
 @details  The `PWM_MODES` enum specifies various operating modes for the PWM generator. 
           These modes control how the PWM signal is generated, including independent edge modes, 
           center-aligned modes, and dual-edge modes. 
           Each mode provides different timing characteristics for the PWM signal, 
           which may be useful for applications requiring precise control of the signal's rise, fall, or center alignment.
           The modes available in this enum can be selected based on the specific requirements of the application.
*/
typedef enum 
{
    PWM_MODE_INDEPENDENT_EDGE =  0x0,       /**< Independent Edge mode*/
    PWM_MODE_VARIABLE_PHASE =  0x1,       /**< Variable Phase mode*/
    PWM_MODE_INDEPENDENT_EDGE_DUAL_OUTPUT =  0x2,       /**< Independent Edge, dual output mode*/
    PWM_MODE_CENTER_ALIGNED =  0x4,       /**< Center-Aligned mode*/
    PWM_MODE_DOUBLE_UPDATE_CENTER_ALIGNED =  0x5,       /**< Double-Update Center-Aligned mode*/
    PWM_MODE_DUAL_EDGE_CENTER_ALIGNED_ONE_UPDATE_CYCLE =  0x6,       /**< Dual Edge Center-Aligned;one update/cycle mode*/
    PWM_MODE_DUAL_EDGE_CENTER_ALIGNED_TWO_UPDATES_CYCLE =  0x7,       /**< Dual Edge Center-Aligned;two updates/cycle mode*/
}PWM_MODES;

/**
 @enum     PWM_OUTPUT_MODES
 @brief    Defines the PWM generator output modes that are available.
 @details  The `PWM_HS_OUTPUT_MODES` enum defines the different output configurations for the PWM generator. 
           The output modes control whether the PWM signals are generated in complementary, independent, or push-pull mode. 
           These output modes are essential for driving external devices or circuits, and the choice of output mode 
           depends on the hardware and the requirements of the system being controlled.
*/
typedef enum 
{
    COMPLEMENTARY_OUTPUT_MODE = 0x0,         /**< Complementary mode output mode*/
    INDEPENDENT_OUTPUT_MODE = 0x1,           /**< Independent mode output mode*/
    PUSH_PULL_OUTPUT_MODE = 0x2,             /**< Push-Pull mode output mode*/
}PWM_OUTPUT_MODES;

/**
 @enum     PWM_SOURCE_SELECT    
 @brief    Defines the PWM generator Master or Independent source selection.
 @details  The `PWM_SOURCE_SELECT` enum allows the user to select whether the PWM generator will operate in a master or independent source. 
*/
typedef enum 
{
    PWM_SOURCE_SELECT_INDEPENDENT = 0,         /**< PWM select Independent PWM as source */
    PWM_SOURCE_SELECT_MASTER = 1,             /**< PWM select Master as source */
}PWM_SOURCE_SELECT;

/**
 * @brief    Callback function type for  PWM generator End of Conversion (EOC) event.
 * 
 * @details  This typedef defines a function pointer type for a callback function that is
 *           used to handle PWM generator End of Conversion (EOC) event. The callback function 
 *           is called when an Individual PWM Generator EOC event.
 * 
 * @param[in]    pin     - The external interrupt pin that triggered the callback.
 * @param[in]    context - A user-defined context or data passed to the callback function.
 * 
 * @note     The `pin` parameter indicates which external interrupt pin triggered the
 *           callback, and the `context` parameter allows for user-specific data to 
 *           be passed to the callback function.
 * 
 * @b Example:
 * @code
 * void my_callback(PWM_GENERATOR genNum, uintptr_t context) {
 *      
 * }
 * 
 * PWM_GENERATOR_EOC_EVENT_CALLBACK callback = my_callback;
 * @endcode
 */
typedef  void (*PWM_GENERATOR_EOC_EVENT_CALLBACK) (PWM_GENERATOR genNum, uintptr_t context);

/**
 @struct   EXT_INT_PIN_CALLBACK_OBJ
 @brief    This structure holds the callback and context information for handling
           events on a specified PWM Generator.
*/
typedef struct
{
    PWM_GENERATOR_EOC_EVENT_CALLBACK            callback;
    
    uintptr_t                                   context;
    
}PWM_GENERATOR_EOC_EVENT_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: PWM Peripheral APIs
// *****************************************************************************
// *****************************************************************************

/**
 * @brief    Initializes PWM Peripheral with the given configuration.
 * @details  This function configures the PWM peripheral using predefined initialization parameters.
 * @param    none
 * @return   none  
 * @remarks  none
 */
void PWM_Initialize(void);

/**
 * @brief    Deinitializes the PWM to its Power-On Reset (POR) state.
 * @details  This function resets the PWM module, setting it back to its POR values. Any configuration
 *           made previously will be lost, and the PWM will be in a state similar to after power-up.
 * @param    none
 * @return   none  
 * @remarks  none
 */
void PWM_Deinitialize(void);

/**
 * @brief      Enables the specific PWM generator.
 * @details    This function activates a PWM generator specified by the argument `genNum`, allowing it to start 
 *             generating PWM signals.
 * @param[in]  genNum - PWM generator number  
 * @return     none  
 * @remarks  none
 */
inline static void PWM_GeneratorEnable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1CONbits.ON = 1U;
                break;
        case PWM_GENERATOR_2:
                PG2CONbits.ON = 1U;
                break;
        case PWM_GENERATOR_3:
                PG3CONbits.ON = 1U;
                break;
        case PWM_GENERATOR_4:
                PG4CONbits.ON = 1U;
                break;
        default:break;
    }     
}

/**
 * @brief      Disables the specific PWM generator.
 * @details    This function disables the specified PWM generator, stopping it from generating PWM signals.
 * @param[in]  genNum - PWM generator number
 * @return     none  
 * @remarks  none
 */
inline static void PWM_GeneratorDisable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1CONbits.ON = 0U;
                break;
        case PWM_GENERATOR_2:
                PG2CONbits.ON = 0U;
                break;
        case PWM_GENERATOR_3:
                PG3CONbits.ON = 0U;
                break;
        case PWM_GENERATOR_4:
                PG4CONbits.ON = 0U;
                break;
        default:break;
    }     
}

/**
 * @brief      Sets the operating mode for a specific PWM generator.
 * @details    This function sets the operating mode of the PWM generator selected by the argument `genNum`.
 *             The operating mode determines the behavior of the PWM generator (e.g., edge-aligned or center-aligned).
 * @param[in]  genNum - PWM generator number
 * @param[in]  mode - PWM operating mode
 * @return     none  
 * @remarks  none
 */
inline static void PWM_ModeSet(PWM_GENERATOR genNum, PWM_MODES mode)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1CONbits.MODSEL = mode; 
                break;
        case PWM_GENERATOR_2:
                PG2CONbits.MODSEL = mode; 
                break;
        case PWM_GENERATOR_3:
                PG3CONbits.MODSEL = mode; 
                break;
        case PWM_GENERATOR_4:
                PG4CONbits.MODSEL = mode; 
                break;
        default:break;
    }     
}

/**
 * @brief      Configures the output mode of a specific PWM generator.
 * @details    This function allows setting the output mode of the PWM signal (e.g., Independent Edge, push-pull, etc.)
 *             for a specific generator. The output mode determines the type of PWM waveform generated.
 * @param[in]  genNum - PWM generator number
 * @param[in]  outputMode - PWM output mode
 * @return     none  
 * @remarks  none
 */
inline static void PWM_OutputModeSet(PWM_GENERATOR genNum, PWM_OUTPUT_MODES outputMode)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.PMOD = outputMode; 
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.PMOD = outputMode; 
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.PMOD = outputMode; 
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.PMOD = outputMode; 
                break;
        default:break;
    }     
}

/**
 * @brief    Enables all PWM generators.
 * @details  This function activates all PWM generators in the module, allowing them to start generating PWM signals.
 *           It is useful when enabling the entire module for the first time or after a reset.
 * @param    none
 * @return   none  
 * @remarks  none
 */
inline static void PWM_Enable(void)
{
    PG1CONbits.ON = 1U;
    PG2CONbits.ON = 1U;
    PG3CONbits.ON = 1U;
    PG4CONbits.ON = 1U;
}

/**
 * @brief    Disables all PWM generators.
 * @details  This function deactivates all PWM generators in the module, stopping them from generating PWM signals.
 *           The generators can be re-enabled individually or as a whole.
 * @param    none
 * @return   none  
 * @remarks  none
 */
inline static void PWM_Disable(void)
{
    PG1CONbits.ON = 0U;
    PG2CONbits.ON = 0U;
    PG3CONbits.ON = 0U;
    PG4CONbits.ON = 0U;
}

/**
 * @brief      Sets the period value for the Master Time Base generator.
 * @details    This function allows setting the period for the Master Time Base generator, which controls the overall 
 *             timing of the PWM module.
 * @param[in]  masterPeriod - Period value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks  none
 */
inline static void PWM_MasterPeriodSet(uint32_t masterPeriod)
{
    MPER = masterPeriod & 0x000FFFF0UL;
}

/**
 * @brief      Sets the PWM master duty cycle register.
 * @details    This function sets the duty cycle for the Master Time Base generator, which determines the width 
 *             of the PWM signal relative to the period.
 * @param[in]  masterDutyCycle - Master Duty Cycle value
 * @return     none
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_MasterDutyCycleSet(uint32_t masterDutyCycle)
{
    MDC = masterDutyCycle & 0x000FFFF0UL;
}

/**
 * @brief      Sets the phase value for the Master Time Base generator.
 * @details    This function sets the phase for the Master Time Base generator, allowing you to shift the timing 
 *             of the PWM signals generated by the module.
 * @param[in]  masterPhase - Phase value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_MasterPhaseSet(uint32_t masterPhase)
{
    MPHASE = masterPhase & 0x000FFFF0UL;
}

/**
 * @brief      Sets the period for a specific PWM generator's Time Base.
 * @details    This function sets the period for a specific PWM generator, which controls the timing of the PWM signal 
 *             generated by the generator.
 * @param[in]  genNum - PWM generator number
 * @param[in]  period - PWM generator period value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_PeriodSet(PWM_GENERATOR genNum,uint32_t period)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1PER = period & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_2:
                PG2PER = period & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_3:
                PG3PER = period & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_4:
                PG4PER = period & 0x000FFFF0UL;
                break;
        default:break;
    }     
}

/**
 * @brief      Sets the duty cycle for a specific PWM generator.
 * @details    This function sets the duty cycle for the selected PWM generator. The duty cycle controls the 
 *             width of the PWM pulse relative to the period.
 * @param[in]  genNum      - PWM generator number
 * @param[in]  dutyCycle   - PWM generator duty cycle
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks    none
 */
inline static void PWM_DutyCycleSet(PWM_GENERATOR genNum,uint32_t dutyCycle)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1DC = dutyCycle & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_2:
                PG2DC = dutyCycle & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_3:
                PG3DC = dutyCycle & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_4:
                PG4DC = dutyCycle & 0x000FFFF0UL;
                break;
        default:break;
    }     
}

/**
 * @brief      This inline function selects the PWM generator source for Phase.
 * @details    This function sets the source of the PWM phase for the specified generator. It allows you to configure
 *             the generator's phase either Master Phase or generator phase itself.
 * @param[in]  genNum - PWM generator number
 * @param[in]  source - PWM generator source select
 * @return     none  
 * @remarks    none
 */
inline static void PWM_PhaseSelect(PWM_GENERATOR genNum,PWM_SOURCE_SELECT source)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1CONbits.MPHSEL = source;
                break;
        case PWM_GENERATOR_2:
                PG2CONbits.MPHSEL = source;
                break;
        case PWM_GENERATOR_3:
                PG3CONbits.MPHSEL = source;
                break;
        case PWM_GENERATOR_4:
                PG4CONbits.MPHSEL = source;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function sets the phase value in count for the PWM generator specific Time Base.
 * @details    This function sets the phase for the specified PWM generator's time base, allowing you to adjust
 *             the signal's phase relative to other signals.
 * @param[in]  genNum - PWM generator number
 * @param[in]  phase - PWM generator phase value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_PhaseSet(PWM_GENERATOR genNum,uint32_t phase)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1PHASE = phase & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_2:
                PG2PHASE = phase & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_3:
                PG3PHASE = phase & 0x000FFFF0UL;
                break;
        case PWM_GENERATOR_4:
                PG4PHASE = phase & 0x000FFFF0UL;
                break;
        default:break;
    }     
}

/**
 * @brief      This inline function updates PWM override data bits with the requested value for a 
 *             specific PWM generator selected by the argument \ref PWM_GENERATOR.
 * @details    This function updates the override data of the PWM generator. Override data allows you to manipulate 
 *             the PWM signals for specific generator outputs.
 * @param[in]  genNum          -   PWM generator number
 * @param[in]  overrideData    -   Override data  
 * @return     none  
 * @remarks   none
 */
inline static void PWM_OverrideDataSet(PWM_GENERATOR genNum,uint16_t overrideData)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.OVRDAT = overrideData;
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRDAT = overrideData;
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRDAT = overrideData;
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRDAT = overrideData;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function updates PWM override high data bit with the requested value for a 
 *             specific PWM generator selected by the argument \ref PWM_GENERATOR.
 * @details    This function allows you to set the high override data bit for the specified PWM generator.
 * @param[in]  genNum              - PWM generator number
 * @param[in]  overrideDataHigh    - Override data  
 * @return     none  
 * @remarks   none
 */
inline static void PWM_OverrideDataHighSet(PWM_GENERATOR genNum,bool overrideDataHigh)
{
    switch(genNum) 
    { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.OVRDAT = (PG1IOCONbits.OVRDAT & 0x1) | ((uint8_t)overrideDataHigh << 0x1);
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRDAT = (PG2IOCONbits.OVRDAT & 0x1) | ((uint8_t)overrideDataHigh << 0x1);
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRDAT = (PG3IOCONbits.OVRDAT & 0x1) | ((uint8_t)overrideDataHigh << 0x1);
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRDAT = (PG4IOCONbits.OVRDAT & 0x1) | ((uint8_t)overrideDataHigh << 0x1);
                break;
        default:break;
    }
}

/**
 * @brief      This inline function updates PWM override low data bit with the requested value for a 
 *             specific PWM generator selected by the argument \ref PWM_GENERATOR.
 * @details    This function allows you to set the low override data bit for the specified PWM generator. 
 * @param[in]  genNum             - PWM generator number
 * @param[in]  overrideDataLow    - Override data  
 * @return     none  
 * @remarks    none
 */
inline static void PWM_OverrideDataLowSet(PWM_GENERATOR genNum,bool overrideDataLow)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.OVRDAT = (PG1IOCONbits.OVRDAT & 0x2) | overrideDataLow;
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRDAT = (PG2IOCONbits.OVRDAT & 0x2) | overrideDataLow;
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRDAT = (PG3IOCONbits.OVRDAT & 0x2) | overrideDataLow;
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRDAT = (PG4IOCONbits.OVRDAT & 0x2) | overrideDataLow;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function gets PWM override value for the PWM Generator selected by the 
 *             argument \ref PWM_GENERATOR.
 * @details    This function retrieves the override data for the specified PWM generator. The override data
 *             affects the behavior of the PWM signal, and this function allows you to read its current value.
 * @param[in]  genNum  -  PWM generator number
 * @return     Override data for the PWM Generator selected by the argument 
 *             PWM_GENERATOR.  
 * @remarks    none
 */
inline static uint16_t PWM_OverrideDataGet(PWM_GENERATOR genNum)
{
    uint16_t overrideData = 0x0U;
    switch(genNum) { 
        case PWM_GENERATOR_1:
                overrideData = PG1IOCONbits.OVRDAT;
                break;
        case PWM_GENERATOR_2:
                overrideData = PG2IOCONbits.OVRDAT;
                break;
        case PWM_GENERATOR_3:
                overrideData = PG3IOCONbits.OVRDAT;
                break;
        case PWM_GENERATOR_4:
                overrideData = PG4IOCONbits.OVRDAT;
                break;
        default:break;
    }
    return overrideData;
}

/**
 * @brief      This inline function enables PWM override on PWMH output for specific PWM generator selected 
 *             by the argument \ref PWM_GENERATOR.
 * @details    This function enables the override functionality on the PWMxH output for the specified PWM generator.
 * @param[in]  genNum - PWM generator number  
 * @return     none  
 * @remarks   none
 */
inline static void PWM_OverrideHighEnable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.OVRENH = 1U;
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRENH = 1U;
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRENH = 1U;
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRENH = 1U;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function enables PWM override on PWML output for specific PWM generator selected 
 *             by the argument \ref PWM_GENERATOR.
 * @details    This function enables the override functionality on the PWMxL output for the specified PWM generator.
 * @param[in]  genNum - PWM generator number
 * @return     none  
 * @remarks   none
 */
inline static void PWM_OverrideLowEnable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.OVRENL = 1U;
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRENL = 1U;
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRENL = 1U;
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRENL = 1U;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function disables PWM override on PWMH output for specific PWM generator selected 
 *             by the argument \ref PWM_GENERATOR.
 * @details    This function disables the override functionality on the PWMxH output for the specified PWM generator.
 * @param[in]  genNum - PWM generator number
 * @return     none  
 * @remarks   none
 */
inline static void PWM_OverrideHighDisable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.OVRENH = 0U;
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRENH = 0U;
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRENH = 0U;
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRENH = 0U;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function disables PWM override on PWML output for specific PWM generator selected 
 *             by the argument \ref PWM_GENERATOR.
 * @details    This function disables the override functionality on the PWMxL output for the specified PWM generator.
 * @param[in]  genNum - PWM generator number 
 * @return     none  
 * @remarks    none
 */
inline static void PWM_OverrideLowDisable(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1IOCONbits.OVRENL = 0U;
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRENL = 0U;
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRENL = 0U;
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRENL = 0U;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function updates PWM Deadtime low register with the requested value for a 
 *             specific PWM generator selected by the argument \ref PWM_GENERATOR.
 * @details    Deadtime insertion ensures that there is a gap between turning off one switch and turning on the other in 
 *             a complementary PWM generator configuration. This function sets the lower portion of the deadtime register.
 * @param[in]  genNum      - PWM generator number
 * @param[in]  deadtimeLow - Deadtime low value
 * @return     none  
 * @remarks   none
 */
inline static void PWM_DeadTimeLowSet(PWM_GENERATOR genNum,uint16_t deadtimeLow)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1DT = (PG1DT & 0xFFFF0000UL) | (deadtimeLow & (uint16_t)0x7FFF);
                break;
        case PWM_GENERATOR_2:
                PG2DT = (PG2DT & 0xFFFF0000UL) | (deadtimeLow & (uint16_t)0x7FFF);
                break;
        case PWM_GENERATOR_3:
                PG3DT = (PG3DT & 0xFFFF0000UL) | (deadtimeLow & (uint16_t)0x7FFF);
                break;
        case PWM_GENERATOR_4:
                PG4DT = (PG4DT & 0xFFFF0000UL) | (deadtimeLow & (uint16_t)0x7FFF);
                break;
        default:break;
    }
}

/**
 * @brief      This inline function updates PWM Deadtime high register with the requested value for a 
 *             specific PWM generator selected by the argument \ref PWM_GENERATOR.
 * @details    This function sets the upper portion of the deadtime register to control the delay between complementary
 *             switching signals for the specified PWM generator.
 * @param[in]  genNum          - PWM generator number
 * @param[in]  deadtimeHigh    - Deadtime high value
 * @return     none  
 * @remarks   none
 */
inline static void PWM_DeadTimeHighSet(PWM_GENERATOR genNum,uint16_t deadtimeHigh)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1DT = (((uint32_t)deadtimeHigh & (uint16_t)0x7FFF) << 16) | (uint16_t)PG1DT;
                break;
        case PWM_GENERATOR_2:
                PG2DT = (((uint32_t)deadtimeHigh & (uint16_t)0x7FFF) << 16) | (uint16_t)PG2DT;
                break;
        case PWM_GENERATOR_3:
                PG3DT = (((uint32_t)deadtimeHigh & (uint16_t)0x7FFF) << 16) | (uint16_t)PG3DT;
                break;
        case PWM_GENERATOR_4:
                PG4DT = (((uint32_t)deadtimeHigh & (uint16_t)0x7FFF) << 16) | (uint16_t)PG4DT;
                break;
        default:break;
    }
}

/**
 * @brief      This inline function updates PWM Deadtime low and high register with the requested value for a 
 *             specific PWM generator selected by the argument \ref PWM_GENERATOR.
 * @details    This function sets both the low and high portions of the deadtime register, which determines the 
 *             delay between complementary switching signals.
 * @param[in]  genNum          - PWM generator number
 * @param[in]  deadtimeHigh    - Deadtime value
 * @return     none  
 * @remarks   none
 */
inline static void PWM_DeadTimeSet(PWM_GENERATOR genNum,uint16_t deadtime)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1DT = (((uint32_t)deadtime & (uint16_t)0x7FFF) << 16) | ((uint32_t)deadtime & (uint16_t)0x7FFF);
                break;
        case PWM_GENERATOR_2:
                PG2DT = (((uint32_t)deadtime & (uint16_t)0x7FFF) << 16) | ((uint32_t)deadtime & (uint16_t)0x7FFF);
                break;
        case PWM_GENERATOR_3:
                PG3DT = (((uint32_t)deadtime & (uint16_t)0x7FFF) << 16) | ((uint32_t)deadtime & (uint16_t)0x7FFF);
                break;
        case PWM_GENERATOR_4:
                PG4DT = (((uint32_t)deadtime & (uint16_t)0x7FFF) << 16) | ((uint32_t)deadtime & (uint16_t)0x7FFF);
                break;
        default:break;
    }
}

/**
 * @brief      This inline function sets the PWM trigger compare value in count for the PWM Generator 
 *             selected by the argument \ref PWM_GENERATOR.
 * @details    This function sets the trigger compare value for the specified PWM generator, which determines the 
 *             timing for triggering other events or signals.
 * @param[in]  genNum          - PWM generator number
 * @param[in]  trigCompValue   - Trigger compare value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_TriggerCompareValueSet(PWM_GENERATOR genNum,uint32_t trigCompValue)
{
    switch(genNum) {
        case PWM_GENERATOR_1:
                PG1TRIGA = trigCompValue & 0x000FFFF0UL;  
                break;
        case PWM_GENERATOR_2:
                PG2TRIGA = trigCompValue & 0x000FFFF0UL;  
                break;
        case PWM_GENERATOR_3:
                PG3TRIGA = trigCompValue & 0x000FFFF0UL;  
                break;
        case PWM_GENERATOR_4:
                PG4TRIGA = trigCompValue & 0x000FFFF0UL;  
                break;
        default:break;
    }
}

/**
 * @brief      This inline function enables interrupt requests for the PWM Generator selected by the 
 *             argument \ref PWM_GENERATOR.   
 * @param[in]  genNum - PWM generator number
 * @param[in]  interrupt - PWM generator interrupt source
 * @return     none  
 */
inline static void PWM_GeneratorInterruptEnable(PWM_GENERATOR genNum, PWM_GENERATOR_INTERRUPT interrupt)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG1EVTbits.FLTIEN = 1U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG1EVTbits.CLIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG1EVTbits.FFIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG1EVTbits.SIEN = 1U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        case PWM_GENERATOR_2:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG2EVTbits.FLTIEN = 1U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG2EVTbits.CLIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG2EVTbits.FFIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG2EVTbits.SIEN = 1U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        case PWM_GENERATOR_3:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG3EVTbits.FLTIEN = 1U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG3EVTbits.CLIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG3EVTbits.FFIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG3EVTbits.SIEN = 1U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        case PWM_GENERATOR_4:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG4EVTbits.FLTIEN = 1U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG4EVTbits.CLIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG4EVTbits.FFIEN = 1U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG4EVTbits.SIEN = 1U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        default:break;
    }
}

/**
 * @brief      This inline function disables interrupt requests for the PWM Generator selected by the 
 *             argument \ref PWM_GENERATOR.
 * @param[in]  genNum 	 - PWM generator number
 * @param[in]  interrupt - PWM generator interrupt source
 * @return     none  
 */
inline static void PWM_GeneratorInterruptDisable(PWM_GENERATOR genNum, PWM_GENERATOR_INTERRUPT interrupt)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG1EVTbits.FLTIEN = 0U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG1EVTbits.CLIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG1EVTbits.FFIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG1EVTbits.SIEN = 0U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        case PWM_GENERATOR_2:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG2EVTbits.FLTIEN = 0U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG2EVTbits.CLIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG2EVTbits.FFIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG2EVTbits.SIEN = 0U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        case PWM_GENERATOR_3:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG3EVTbits.FLTIEN = 0U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG3EVTbits.CLIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG3EVTbits.FFIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG3EVTbits.SIEN = 0U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        case PWM_GENERATOR_4:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG4EVTbits.FLTIEN = 0U;
                                        break;       
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG4EVTbits.CLIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG4EVTbits.FFIEN = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG4EVTbits.SIEN = 0U;
                                        break;                                                        
                        default:break;  
                }
                break;   
        default:break;
    }
}

/**
 * @brief      This inline function clears the PWM interrupt status for the PWM Generator selected by the 
 *             argument \ref PWM_GENERATOR.   
 * @param[in]  genNum 	- PWM generator number
 * @param[in]  interrupt - PWM generator interrupt source
 * @return     none  
 */
inline static void PWM_GeneratorEventStatusClear(PWM_GENERATOR genNum, PWM_GENERATOR_INTERRUPT interrupt)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG1STATbits.FLTEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG1STATbits.CLEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG1STATbits.FFEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG1STATbits.SEVT = 0U;
                                        break;
                        default:break;  
                }              
                break; 
        case PWM_GENERATOR_2:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG2STATbits.FLTEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG2STATbits.CLEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG2STATbits.FFEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG2STATbits.SEVT = 0U;
                                        break;
                        default:break;  
                }              
                break; 
        case PWM_GENERATOR_3:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG3STATbits.FLTEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG3STATbits.CLEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG3STATbits.FFEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG3STATbits.SEVT = 0U;
                                        break;
                        default:break;  
                }              
                break; 
        case PWM_GENERATOR_4:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        PG4STATbits.FLTEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        PG4STATbits.CLEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        PG4STATbits.FFEVT = 0U;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        PG4STATbits.SEVT = 0U;
                                        break;
                        default:break;  
                }              
                break; 
        default:break;
    }
}

/**
 * @brief      This inline function gets the PWM interrupt status for the PWM Generator selected by the 
 *             argument \ref PWM_GENERATOR.   
 * @param[in]  genNum 	- PWM generator number
 * @param[in]  interrupt - PWM generator interrupt source
 * @return     true  - Interrupt is pending
 * @return     false - Interrupt is not pending
 */
inline static bool PWM_GeneratorEventStatusGet(PWM_GENERATOR genNum, PWM_GENERATOR_INTERRUPT interrupt)
{
    bool status = false;
    switch(genNum) { 
        case PWM_GENERATOR_1:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        status = PG1STATbits.FLTEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = PG1STATbits.CLEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = PG1STATbits.FFEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = PG1STATbits.SEVT;
                                        break;
                        default:break;  
                }              
                break; 
        case PWM_GENERATOR_2:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        status = PG2STATbits.FLTEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = PG2STATbits.CLEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = PG2STATbits.FFEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = PG2STATbits.SEVT;
                                        break;
                        default:break;  
                }              
                break; 
        case PWM_GENERATOR_3:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        status = PG3STATbits.FLTEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = PG3STATbits.CLEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = PG3STATbits.FFEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = PG3STATbits.SEVT;
                                        break;
                        default:break;  
                }              
                break; 
        case PWM_GENERATOR_4:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        status = PG4STATbits.FLTEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = PG4STATbits.CLEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = PG4STATbits.FFEVT;
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = PG4STATbits.SEVT;
                                        break;
                        default:break;  
                }              
                break; 
        default:break;
    }
    return status;
}

/**
 * @brief      This inline function requests to update the data registers for specific PWM generator 
 *             selected by the argument \ref PWM_GENERATOR.
 * @details    This function triggers a software update request for the specified PWM generator. 
 *             The update affects the data registers, and once requested, the update will 
 *             be processed by the system. The function does not return any status, but can 
 *             be used to initiate an update.
 * @param[in]  genNum - PWM generator number
 * @return     none  
 * @remarks    none
 */
inline static void PWM_SoftwareUpdateRequest(PWM_GENERATOR genNum)
{
    switch(genNum) { 
        case PWM_GENERATOR_1:
                PG1STATbits.UPDREQ = 1U;
                break;
        case PWM_GENERATOR_2:
                PG2STATbits.UPDREQ = 1U;
                break;
        case PWM_GENERATOR_3:
                PG3STATbits.UPDREQ = 1U;
                break;
        case PWM_GENERATOR_4:
                PG4STATbits.UPDREQ = 1U;
                break;
        default:break;
    }

}

/**
 * @brief      This inline function gets the status of the update request for specific PWM generator 
 *             selected by the argument \ref PWM_GENERATOR.
 * @details    This function checks whether a software update is currently pending for the 
 *             selected PWM generator. The function returns a boolean value indicating 
 *             whether the update has been requested and is awaiting processing.
 * @param[in]  genNum - PWM generator number
 * @return     true  - Software update is pending
 * @return     false - Software update is not pending 
 * @remarks   none
 */
inline static bool PWM_SoftwareUpdatePending(PWM_GENERATOR genNum)
{
    bool status = false;
    switch(genNum) { 
        case PWM_GENERATOR_1:
                status = PG1STATbits.UPDREQ;
                break;       
        case PWM_GENERATOR_2:
                status = PG2STATbits.UPDREQ;
                break;       
        case PWM_GENERATOR_3:
                status = PG3STATbits.UPDREQ;
                break;       
        case PWM_GENERATOR_4:
                status = PG4STATbits.UPDREQ;
                break;       
        default:break;
    }
    return status;
}

/**
 * @brief      This inline function sets the Trigger A compare value in count for a specific PWM generator 
 *             selected by the argument \ref PWM_GENERATOR.  
 * @details    This function sets the trigger A compare value for the specified PWM generator, which determines the 
 *             timing for triggering other events or signals.
 * @param[in]  genNum - PWM generator number
 * @param[in]  trigA  - Trigger A compare value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_TriggerACompareValueSet(PWM_GENERATOR genNum,uint32_t trigA)
{
    switch(genNum) {
        case PWM_GENERATOR_1:
                PG1TRIGA = trigA & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_2:
                PG2TRIGA = trigA & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_3:
                PG3TRIGA = trigA & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_4:
                PG4TRIGA = trigA & 0x800FFFF0UL;
                break;
        default:break;
    }
}


/**
 * @brief      This inline function sets the Trigger B compare value in count for a specific PWM generator 
 *             selected by the argument \ref PWM_GENERATOR.   
 * @details    This function sets the trigger B compare value for the specified PWM generator, which determines the 
 *             timing for triggering other events or signals.
 * @param[in]  genNum - PWM generator number
 * @param[in]  trigB  - Trigger B compare value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_TriggerBCompareValueSet(PWM_GENERATOR genNum,uint32_t trigB)
{
    switch(genNum) {
        case PWM_GENERATOR_1:
                PG1TRIGB = trigB & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_2:
                PG2TRIGB = trigB & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_3:
                PG3TRIGB = trigB & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_4:
                PG4TRIGB = trigB & 0x800FFFF0UL;
                break;
        default:break;
    }
}


/**
 * @brief      This inline function sets the Trigger C compare value in count for a specific PWM generator 
 *             selected by the argument \ref PWM_GENERATOR.
 * @details    This function sets the trigger C compare value for the specified PWM generator, which determines the 
 *             timing for triggering other events or signals.
 * @param[in]  genNum - PWM generator number
 * @param[in]  trigC  - Trigger C compare value in count
 * @return     none  
 * @Note       Refer datasheet for valid size of data bits
 * @remarks   none
 */
inline static void PWM_TriggerCCompareValueSet(PWM_GENERATOR genNum,uint32_t trigC)
{
    switch(genNum) {
        case PWM_GENERATOR_1:
                PG1TRIGC = trigC & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_2:
                PG2TRIGC = trigC & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_3:
                PG3TRIGC = trigC & 0x800FFFF0UL;
                break;
        case PWM_GENERATOR_4:
                PG4TRIGC = trigC & 0x800FFFF0UL;
                break;
        default:break;
    }
}


/**
 * @brief Registers a callback function for PWM EOC event.
 *
 * @details This function allows the application to register a callback function 
 * that the PLIB will call when a individual PWM generator EOC event has occured. 
 *
 * The function also allows the application to specify a context value 
 * (usually a pointer to a structure or data) that is passed into the callback 
 * function when it is executed. The registered callback will be invoked 
 * from the peripheral interrupt context. Therefore, the callback function should 
 * be lightweight and quick.
 *
 * The callback function must be registered before enabling the PWM Generator.
 *
 * @pre The `PWM_Initialize()` function must have been called. 
 * This function is only available if the library is configured for **interrupt mode**.
 *
 * @param[in] callback A pointer to a function with a calling signature defined 
 *                     by the `PWM_CALLBACK` data type. 
 *                     Setting this to `NULL` disables the callback feature.
 * @param[in] context  A value (usually a pointer) that is passed into the callback 
 *                     function when it is invoked.
 *
 * @return None.
 *
 * @remarks None.
 */
bool PWM_EOCEventCallbackRegister(
    PWM_GENERATOR genNum,
    const PWM_GENERATOR_EOC_EVENT_CALLBACK callback,
    uintptr_t context );

#endif //PWM_H

