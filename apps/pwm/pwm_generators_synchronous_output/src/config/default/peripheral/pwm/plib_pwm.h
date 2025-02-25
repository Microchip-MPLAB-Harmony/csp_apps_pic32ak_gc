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
 @brief    Defines the PWM generators that are selected from the MCC Harmony User 
           Interface for the PWM output controls.
 @details  These macro's represents the different PWM generators available for configuration 
           through the MCC Harmony User Interface. These values correspond to specific hardware PWM generators 
           that can be used for generating PWM outputs. The generated list in the help documentation may not represent 
           all the generators but is based on the user's configuration in the MCC Harmony tool.
           The user should configure the PWM generators according to their application requirements.
           The generated enum list may vary depending on the specific PWM Generators are enabled in User Interface.
*/
#define    PWM_GENERATOR_1      1U      /**< PWM generator 1*/
#define    PWM_GENERATOR_2      2U      /**< PWM generator 2*/
#define    PWM_GENERATOR_3      3U      /**< PWM generator 3*/
#define    PWM_GENERATOR_4      4U      /**< PWM generator 4*/

typedef uint32_t PWM_GENERATOR;

/**
 @brief    Defines the PWM generator interrupts that are available for the module to use.
 @details  Defines the PWM generator interrupts that are available for the module to use.
*/
#define    PWM_GENERATOR_INTERRUPT_FAULT           1U      /**< PWM Generator Fault Interrupt */ 
#define    PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT   2U      /**< PWM Generator Current Limit Interrupt */
#define    PWM_GENERATOR_INTERRUPT_FEED_FORWARD    3U      /**< PWM Generator Feed Forward Interrupt */
#define    PWM_GENERATOR_INTERRUPT_SYNC            4U      /**< PWM Generator Sync Interrupt */

typedef uint32_t PWM_GENERATOR_INTERRUPT;

/**
 @brief    Defines the PWM generator Trigger Compare registers 
           that are available for the module to use.
 @details  These macro's defines the registers used to trigger compare events for the PWM generator. 
           The trigger compare registers (A, B, C) are used to set the conditions for when the PWM generator should update or trigger an event.
           These registers are essential for controlling the timing and synchronization of the PWM signals and the system's response to those signals.
*/
#define    PWM_TRIGGER_COMPARE_A    1U         /**< PWM Trigger Compare A Register */
#define    PWM_TRIGGER_COMPARE_B    2U         /**< PWM Trigger Compare B Register */
#define    PWM_TRIGGER_COMPARE_C    3U         /**< PWM Trigger Compare C Register */

typedef uint32_t PWM_TRIGGER_COMPARE;

/**
 @brief    Defines the PWM generator operating modes that are available.
 @details  These macro's specifies various operating modes for the PWM generator. 
           These modes control how the PWM signal is generated, including independent edge modes, 
           center-aligned modes, and dual-edge modes. 
           Each mode provides different timing characteristics for the PWM signal, 
           which may be useful for applications requiring precise control of the signal's rise, fall, or center alignment.
           The modes available in this enum can be selected based on the specific requirements of the application.
*/
#define    PWM_MODE_INDEPENDENT_EDGE                          0x0U       /**< Independent Edge mode*/
#define    PWM_MODE_VARIABLE_PHASE                            0x1U       /**< Variable Phase mode*/
#define    PWM_MODE_INDEPENDENT_EDGE_DUAL_OUTPUT              0x2U       /**< Independent Edge, dual output mode*/
#define    PWM_MODE_CENTER_ALIGNED                            0x4U       /**< Center-Aligned mode*/
#define    PWM_MODE_DOUBLE_UPDATE_CENTER_ALIGNED              0x5U       /**< Double-Update Center-Aligned mode*/
#define    PWM_MODE_DUALEDGE_CTR_ALIGNED_ONE_UPDATE_CYCLE     0x6U       /**< Dual Edge Center-Aligned;one update/cycle mode*/
#define    PWM_MODE_DUALEDGE_CTR_ALIGNED_TWO_UPDATES_CYCLE    0x7U       /**< Dual Edge Center-Aligned;two updates/cycle mode*/

typedef uint32_t PWM_MODES;

/**
 @brief    Defines the PWM generator output modes that are available.
 @details  These macro's defines the different output configurations for the PWM generator. 
           The output modes control whether the PWM signals are generated in complementary, independent, or push-pull mode. 
           These output modes are essential for driving external devices or circuits, and the choice of output mode 
           depends on the hardware and the requirements of the system being controlled.
*/
#define    COMPLEMENTARY_OUTPUT_MODE   0x0U         /**< Complementary mode output mode*/
#define    INDEPENDENT_OUTPUT_MODE     0x1U         /**< Independent mode output mode*/
#define    PUSH_PULL_OUTPUT_MODE       0x2U         /**< Push-Pull mode output mode*/

typedef uint32_t PWM_OUTPUT_MODES;

/**    
 @brief    Defines the PWM generator Master or Independent source selection.
 @details  These macro's allows the user to select whether the PWM generator will operate in a master or independent source. 
*/
#define    PWM_SOURCE_SELECT_INDEPENDENT   0U         /**< PWM select Independent PWM as source */
#define    PWM_SOURCE_SELECT_MASTER        1U         /**< PWM select Master as source */

typedef uint32_t PWM_SOURCE_SELECT;

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
 * PWM_GEN_EOC_EVENT_CALLBACK callback = my_callback;
 * @endcode
 */
typedef  void (*PWM_GEN_EOC_EVENT_CALLBACK) (PWM_GENERATOR genNum, uintptr_t context);

/**
 @struct   EXT_INT_PIN_CALLBACK_OBJ
 @brief    This structure holds the callback and context information for handling
           events on a specified PWM Generator.
*/
typedef struct
{
    PWM_GEN_EOC_EVENT_CALLBACK            callback;
    
    uintptr_t                                   context;
    
}PWM_GEN_EOC_EVENT_CALLBACK_OBJ;

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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                PG1CONbits.MODSEL = (uint8_t)mode; 
                break;
        case PWM_GENERATOR_2:
                PG2CONbits.MODSEL = (uint8_t)mode; 
                break;
        case PWM_GENERATOR_3:
                PG3CONbits.MODSEL = (uint8_t)mode; 
                break;
        case PWM_GENERATOR_4:
                PG4CONbits.MODSEL = (uint8_t)mode; 
                break;
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                PG1IOCONbits.PMOD = (uint8_t)outputMode; 
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.PMOD = (uint8_t)outputMode; 
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.PMOD = (uint8_t)outputMode; 
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.PMOD = (uint8_t)outputMode; 
                break;
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                PG1CONbits.MPHSEL = (uint8_t)source;
                break;
        case PWM_GENERATOR_2:
                PG2CONbits.MPHSEL = (uint8_t)source;
                break;
        case PWM_GENERATOR_3:
                PG3CONbits.MPHSEL = (uint8_t)source;
                break;
        case PWM_GENERATOR_4:
                PG4CONbits.MPHSEL = (uint8_t)source;
                break;
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                PG1IOCONbits.OVRDAT = (uint8_t)overrideData;
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRDAT = (uint8_t)overrideData;
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRDAT = (uint8_t)overrideData;
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRDAT = (uint8_t)overrideData;
                break;
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                PG1IOCONbits.OVRDAT = (uint8_t)((PG1IOCONbits.OVRDAT & 0x1U) | ((uint8_t)overrideDataHigh << 0x1U));
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRDAT = (uint8_t)((PG2IOCONbits.OVRDAT & 0x1U) | ((uint8_t)overrideDataHigh << 0x1U));
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRDAT = (uint8_t)((PG3IOCONbits.OVRDAT & 0x1U) | ((uint8_t)overrideDataHigh << 0x1U));
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRDAT = (uint8_t)((PG4IOCONbits.OVRDAT & 0x1U) | ((uint8_t)overrideDataHigh << 0x1U));
                break;
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                PG1IOCONbits.OVRDAT = (uint8_t)((PG1IOCONbits.OVRDAT & 0x2U) | overrideDataLow);
                break;
        case PWM_GENERATOR_2:
                PG2IOCONbits.OVRDAT = (uint8_t)((PG2IOCONbits.OVRDAT & 0x2U) | overrideDataLow);
                break;
        case PWM_GENERATOR_3:
                PG3IOCONbits.OVRDAT = (uint8_t)((PG3IOCONbits.OVRDAT & 0x2U) | overrideDataLow);
                break;
        case PWM_GENERATOR_4:
                PG4IOCONbits.OVRDAT = (uint8_t)((PG4IOCONbits.OVRDAT & 0x2U) | overrideDataLow);
                break;
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
                }
                break;   
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
                }
                break;   
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
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
                        default:
                            /* Invalid Interrupt type, do nothing */  
                            break;  
                }              
                break; 
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                                        status = (PG1STATbits.FLTEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = (PG1STATbits.CLEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = (PG1STATbits.FFEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = (PG1STATbits.SEVT != 0U);
                                        break;
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
                }              
                break; 
        case PWM_GENERATOR_2:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        status = (PG2STATbits.FLTEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = (PG2STATbits.CLEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = (PG2STATbits.FFEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = (PG2STATbits.SEVT != 0U);
                                        break;
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
                }              
                break; 
        case PWM_GENERATOR_3:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        status = (PG3STATbits.FLTEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = (PG3STATbits.CLEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = (PG3STATbits.FFEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = (PG3STATbits.SEVT != 0U);
                                        break;
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
                }              
                break; 
        case PWM_GENERATOR_4:
                switch(interrupt) { 
                        case PWM_GENERATOR_INTERRUPT_FAULT:
                                        status = (PG4STATbits.FLTEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_CURRENT_LIMIT:
                                        status = (PG4STATbits.CLEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_FEED_FORWARD:
                                        status = (PG4STATbits.FFEVT != 0U);
                                        break;
                        case PWM_GENERATOR_INTERRUPT_SYNC:
                                        status = (PG4STATbits.SEVT != 0U);
                                        break;
                        default:
                            /* Invalid Interrupt type, do nothing */ 
                            break;  
                }              
                break; 
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
                status = (PG1STATbits.UPDREQ != 0U);
                break;       
        case PWM_GENERATOR_2:
                status = (PG2STATbits.UPDREQ != 0U);
                break;       
        case PWM_GENERATOR_3:
                status = (PG3STATbits.UPDREQ != 0U);
                break;       
        case PWM_GENERATOR_4:
                status = (PG4STATbits.UPDREQ != 0U);
                break;       
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
        default:
            /* Invalid PWM Generator, do nothing */ 
            break;
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
    const PWM_GEN_EOC_EVENT_CALLBACK callback,
    uintptr_t context );

#endif //PWM_H

