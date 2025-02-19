/*******************************************************************************
  INTC PLIB Header

  Company:
    Microchip Technology Inc.

  File Name:
    plib_intc.h

  Summary:
    INTC PLIB Header File

  Description:
    None

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef PLIB_INTC_H
#define PLIB_INTC_H

// Section: Included Files

#include "device.h"
#include <stddef.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END


// Section: Data Types

#define INT_SOURCE_NVMECC         5U
#define INT_SOURCE_NVM            6U
#define INT_SOURCE_NVMCRC         7U
#define INT_SOURCE_CLKF           9U
#define INT_SOURCE_CLKE           10U
#define INT_SOURCE_CLK1F          11U
#define INT_SOURCE_CLK1W          12U
#define INT_SOURCE_CLK1M          13U
#define INT_SOURCE_CLK1R          14U
#define INT_SOURCE_CLK2F          15U
#define INT_SOURCE_CLK2W          16U
#define INT_SOURCE_CLK2M          17U
#define INT_SOURCE_CLK2R          18U
#define INT_SOURCE_CLK3F          19U
#define INT_SOURCE_CLK3W          20U
#define INT_SOURCE_CLK3M          21U
#define INT_SOURCE_CLK3R          22U
#define INT_SOURCE_CLK4F          23U
#define INT_SOURCE_CLK4W          24U
#define INT_SOURCE_CLK4M          25U
#define INT_SOURCE_CLK4R          26U
#define INT_SOURCE_WDT            28U
#define INT_SOURCE_INT0           33U
#define INT_SOURCE_INT1           34U
#define INT_SOURCE_INT2           35U
#define INT_SOURCE_INT3           36U
#define INT_SOURCE_INT4           37U
#define INT_SOURCE_PEVTA          38U
#define INT_SOURCE_PEVTB          39U
#define INT_SOURCE_PEVTC          40U
#define INT_SOURCE_PEVTD          41U
#define INT_SOURCE_PEVTE          42U
#define INT_SOURCE_PEVTF          43U
#define INT_SOURCE_PWM1           44U
#define INT_SOURCE_PWM2           45U
#define INT_SOURCE_PWM3           46U
#define INT_SOURCE_PWM4           47U
#define INT_SOURCE_T1             48U
#define INT_SOURCE_CCT1           49U
#define INT_SOURCE_CCP1           50U
#define INT_SOURCE_CCT2           51U
#define INT_SOURCE_CCP2           52U
#define INT_SOURCE_CCT3           53U
#define INT_SOURCE_CCP3           54U
#define INT_SOURCE_CCT4           55U
#define INT_SOURCE_CCP4           56U
#define INT_SOURCE_SPI1RX         63U
#define INT_SOURCE_SPI1TX         64U
#define INT_SOURCE_SPI1E          65U
#define INT_SOURCE_SPI2RX         66U
#define INT_SOURCE_SPI2TX         67U
#define INT_SOURCE_SPI2E          68U
#define INT_SOURCE_SPI3RX         69U
#define INT_SOURCE_SPI3TX         70U
#define INT_SOURCE_SPI3E          71U
#define INT_SOURCE_DMA0           72U
#define INT_SOURCE_DMA1           73U
#define INT_SOURCE_DMA2           74U
#define INT_SOURCE_DMA3           75U
#define INT_SOURCE_CMP1           76U
#define INT_SOURCE_CMP2           77U
#define INT_SOURCE_CMP3           78U
#define INT_SOURCE_I2C1E          80U
#define INT_SOURCE_I2C1           81U
#define INT_SOURCE_I2C1RX         82U
#define INT_SOURCE_I2C1TX         83U
#define INT_SOURCE_I2C2E          84U
#define INT_SOURCE_I2C2           85U
#define INT_SOURCE_I2C2RX         86U
#define INT_SOURCE_I2C2TX         87U
#define INT_SOURCE_U1RX           89U
#define INT_SOURCE_U1TX           90U
#define INT_SOURCE_U1E            91U
#define INT_SOURCE_U2RX           93U
#define INT_SOURCE_U2TX           94U
#define INT_SOURCE_U2E            95U
#define INT_SOURCE_U3RX           97U
#define INT_SOURCE_U3TX           98U
#define INT_SOURCE_U3E            99U
#define INT_SOURCE_SENT1          105U
#define INT_SOURCE_SENT1E         106U
#define INT_SOURCE_SENT2          107U
#define INT_SOURCE_SENT2E         108U
#define INT_SOURCE_DMA4           109U
#define INT_SOURCE_DMA5           110U
#define INT_SOURCE_CNA            113U
#define INT_SOURCE_CNB            114U
#define INT_SOURCE_CNC            115U
#define INT_SOURCE_CND            116U
#define INT_SOURCE_QEI1           125U
#define INT_SOURCE_CRC            131U
#define INT_SOURCE_PTGSTEP        134U
#define INT_SOURCE_PTGWDT         135U
#define INT_SOURCE_PTG0           136U
#define INT_SOURCE_PTG1           137U
#define INT_SOURCE_PTG2           138U
#define INT_SOURCE_PTG3           139U
#define INT_SOURCE_AD1CH0         146U
#define INT_SOURCE_AD1CMP0        147U
#define INT_SOURCE_AD1CH1         148U
#define INT_SOURCE_AD1CMP1        149U
#define INT_SOURCE_AD1CH2         150U
#define INT_SOURCE_AD1CMP2        151U
#define INT_SOURCE_AD1CH3         152U
#define INT_SOURCE_AD1CMP3        153U
#define INT_SOURCE_AD1CH4         154U
#define INT_SOURCE_AD1CMP4        155U
#define INT_SOURCE_AD1CH5         156U
#define INT_SOURCE_AD1CMP5        157U
#define INT_SOURCE_AD1CH6         158U
#define INT_SOURCE_AD1CMP6        159U
#define INT_SOURCE_AD1CH7         160U
#define INT_SOURCE_AD1CMP7        161U
#define INT_SOURCE_AD1CH8         162U
#define INT_SOURCE_AD1CMP8        163U
#define INT_SOURCE_AD1CH9         164U
#define INT_SOURCE_AD1CMP9        165U
#define INT_SOURCE_AD1CH10        166U
#define INT_SOURCE_AD1CMP10       167U
#define INT_SOURCE_AD1CH11        168U
#define INT_SOURCE_AD1CMP11       169U
#define INT_SOURCE_AD1CH12        170U
#define INT_SOURCE_AD1CMP12       171U
#define INT_SOURCE_AD1CH13        172U
#define INT_SOURCE_AD1CMP13       173U
#define INT_SOURCE_AD1CH14        174U
#define INT_SOURCE_AD1CMP14       175U
#define INT_SOURCE_AD1CH15        176U
#define INT_SOURCE_AD1CMP15       177U
#define INT_SOURCE_AD1CH16        178U
#define INT_SOURCE_AD1CMP16       179U
#define INT_SOURCE_AD1CH17        180U
#define INT_SOURCE_AD1CMP17       181U
#define INT_SOURCE_AD1CH18        182U
#define INT_SOURCE_AD1CMP18       183U
#define INT_SOURCE_AD1CH19        184U
#define INT_SOURCE_AD1CMP19       185U
#define INT_SOURCE_AD2CH0         188U
#define INT_SOURCE_AD2CMP0        189U
#define INT_SOURCE_AD2CH1         190U
#define INT_SOURCE_AD2CMP1        191U
#define INT_SOURCE_AD2CH2         192U
#define INT_SOURCE_AD2CMP2        193U
#define INT_SOURCE_AD2CH3         194U
#define INT_SOURCE_AD2CMP3        195U
#define INT_SOURCE_AD2CH4         196U
#define INT_SOURCE_AD2CMP4        197U
#define INT_SOURCE_AD2CH5         198U
#define INT_SOURCE_AD2CMP5        199U
#define INT_SOURCE_AD2CH6         200U
#define INT_SOURCE_AD2CMP6        201U
#define INT_SOURCE_AD2CH7         202U
#define INT_SOURCE_AD2CMP7        203U
#define INT_SOURCE_AD2CH8         204U
#define INT_SOURCE_AD2CMP8        205U
#define INT_SOURCE_AD2CH9         206U
#define INT_SOURCE_AD2CMP9        207U
#define INT_SOURCE_AD2CH10        208U
#define INT_SOURCE_AD2CMP10       209U
#define INT_SOURCE_AD2CH11        210U
#define INT_SOURCE_AD2CMP11       211U
#define INT_SOURCE_AD2CH12        212U
#define INT_SOURCE_AD2CMP12       213U
#define INT_SOURCE_AD2CH13        214U
#define INT_SOURCE_AD2CMP13       215U
#define INT_SOURCE_AD2CH14        216U
#define INT_SOURCE_AD2CMP14       217U
#define INT_SOURCE_AD2CH15        218U
#define INT_SOURCE_AD2CMP15       219U
#define INT_SOURCE_AD2CH16        220U
#define INT_SOURCE_AD2CMP16       221U
#define INT_SOURCE_AD2CH17        222U
#define INT_SOURCE_AD2CMP17       223U
#define INT_SOURCE_AD2CH18        224U
#define INT_SOURCE_AD2CMP18       225U
#define INT_SOURCE_AD2CH19        226U
#define INT_SOURCE_AD2CMP19       227U
#define INT_SOURCE_CLC1P          232U
#define INT_SOURCE_CLC1N          233U
#define INT_SOURCE_CLC2P          234U
#define INT_SOURCE_CLC2N          235U
#define INT_SOURCE_CLC3P          236U
#define INT_SOURCE_CLC3N          237U
#define INT_SOURCE_CLC4P          238U
#define INT_SOURCE_CLC4N          239U

/**
 * @brief    Interrupt Source Type Definition
 *
 * Defines the type used to represent interrupt sources in the system. 
 * The value corresponds to various interrupt sources available in the device.
 */
typedef uint32_t INT_SOURCE;


// Section: Interface Routines

/**
 * @brief   Configures and initializes the interrupt subsystem.
 * @details This function configures and initializes the interrupt subsystem 
 *          appropriately for the current system design.
 * @pre     None.
 * @param   None.
 * @return  None.
 * 
 * @b Example
 * @code
 * INTC_Initialize();
 * @endcode
 * 
 * @remarks None.
 */
void INTC_Initialize ( void );

/**
 * @brief   Enables the interrupt source.
 * @details This function enables the interrupt source. The interrupt flag is set 
 *          when the interrupt request is sampled. The pending interrupt request will not 
 *          cause further processing if the interrupt is not enabled using this function 
 *          or if interrupts are not enabled.
 * @pre     The INTC_Initialize() function must have been called first.
 * @param   source One of the possible values from INT_SOURCE.
 * @return  None.
 * @note    This API performs a Read-Modify-Write (RMW) operation on the IECx register. 
 *          To prevent race conditions, please disable global interrupts using 
 *          `INTC_Disable` API before calling this function.
 * 
 * @b Example
 * @code
 * bool prevStatus = INTC_Disable();
 * INTC_SourceEnable(INT_SOURCE_INT0);
 * INTC_Restore(prevStatus);
 * @endcode
 * 
 * @remarks This function implements an operation of the SourceControl feature. 
 *          This feature may not be available on all devices. Please refer to the specific 
 *          device data sheet to determine availability.
 */
void INTC_SourceEnable( INT_SOURCE source );

/**
 * @brief   Disables the interrupt source.
 * @details This function disables the given interrupt source.
 * @pre     The INTC_Initialize() function must have been called first.
 * @param   source One of the possible values from INT_SOURCE.
 * @return  None.
 * @note    This API performs a Read-Modify-Write (RMW) operation on the IECx register. 
 *          To prevent race conditions, please disable global interrupts using 
 *          `INTC_Disable` API before calling this function.
 *
 * @b Example
 * @code
 * bool prevStatus = INTC_Disable();
 * INTC_SourceDisable(INT_SOURCE_INT0);
 * INTC_Restore(prevStatus);
 * @endcode
 * 
 * @remarks This function implements an operation of the SourceControl feature. 
 *          This feature may not be available on all devices. Please refer to the specific 
 *          device data sheet to determine availability.
 */
void INTC_SourceDisable( INT_SOURCE source );

/**
 * @brief   Gets the enable state of the interrupt source.
 * @details This function gets the enable state of the interrupt source.
 * @pre     The INTC_Initialize() function must have been called first.
 * @param   source One of the possible values from INT_SOURCE.
 * 
 * @return
 * - true: If the interrupt source is enabled
 * - false: If the interrupt source is disabled
 * 
 * @b Example
 * @code
 * if(INTC_SourceIsEnabled(INT_SOURCE_INT0) != true)
 * {
 *     INTC_SourceEnable(INT_SOURCE_INT0);
 * }
 * @endcode
 * 
 * @remarks This function implements an operation of the SourceControl feature. 
 *          This feature may not be available on all devices. Please refer to the specific 
 *          device data sheet to determine availability.
 */
bool INTC_SourceIsEnabled( INT_SOURCE source );

/**
 * @brief   Returns the status of the interrupt flag for the selected source.
 * @details This function returns the status of the interrupt flag for the selected 
 *          source. The flag is set when the interrupt request is recognized. The pending 
 *          interrupt request will not cause further processing if the interrupt is not 
 *          enabled using the function INTC_SourceEnable or if interrupts are not enabled.
 * @pre     The INTC_Initialize() function must have been called first.
 * @param   source One of the possible values from INT_SOURCE.
 * 
 * @return
 * - true: If the interrupt request is recognized for the source
 * - false: If the interrupt request is not recognized for the source
 * 
 * @b Example
 * @code
 * if(INTC_SourceStatusGet(INT_SOURCE_INT0) != true)
 * {
 *     INTC_SourceStatusClear(INT_SOURCE_INT0);
 * }
 * @endcode
 * 
 * @remarks This function implements an operation of the SourceFlag feature. 
 *          This feature may not be available on all devices. Please refer to the specific 
 *          device data sheet to determine availability.
 */
bool INTC_SourceStatusGet( INT_SOURCE source );

/**
 * @brief   Sets the status of the interrupt flag for the selected source.
 * @details This function sets the status of the interrupt flag for the selected 
 *          source. 
 * @note    This function will not be used during normal operation of the system. 
 *          It is used to generate test interrupts for debug and testing purposes.
 * @pre     The INTC_Initialize() function must have been called first.
 * @param   source One of the possible values from INT_SOURCE.
 * @return  None.
 * 
 * @b Example
 * @code
 * INTC_SourceStatusSet(INT_SOURCE_CORE_TIMER);
 * @endcode
 * 
 * @remarks This function implements an operation of the SourceFlag feature. 
 *          This feature may not be available on all devices. Please refer to the specific 
 *          device data sheet to determine availability.
 */
void INTC_SourceStatusSet( INT_SOURCE source );

/**
 * @brief   Clears the status of the interrupt flag for the selected source.
 * @details This function clears the status of the interrupt flag for the selected 
 *          source. The flag is set when the interrupt request is identified. The pending 
 *          interrupt request will not cause further processing if the interrupt is not 
 *          enabled using the function INTC_SourceEnable or if interrupts are not enabled.
 * @pre     The INTC_Initialize() function must have been called first.
 * @param   source One of the possible values from INT_SOURCE.
 * @return  None.
 * 
 * @b Example
 * @code
 * if(INTC_SourceStatusGet(INT_SOURCE_CORE_TIMER) != true)
 * {
 *     INTC_SourceStatusClear(INT_SOURCE_CORE_TIMER);
 * }
 * @endcode
 * 
 * @remarks This function implements an operation of the SourceFlag feature. 
 *          This feature may not be available on all devices. Please refer to the specific 
 *          device data sheet to determine availability.
 */
void INTC_SourceStatusClear( INT_SOURCE source );

/**
 * @brief   Enables all global interrupts.
 * @details This function enables all global interrupts, allowing the system 
 *          to respond to interrupt requests.
 * @pre     None.
 * @param   None.
 * @return  None.
 * 
 * @b Example
 * @code
 * INTC_Enable();
 * @endcode
 * 
 * @remarks None.
 */
void INTC_Enable( void );

/**
 * @brief   Saves the current state of global interrupts and then disables all global interrupts.
 * @details This function saves the current state of global interrupts and then 
 *          disables all global interrupts, preventing further interrupts until they are 
 *          explicitly re-enabled. The interrupt status is returned to allow restoring 
 *          the previous interrupt state.
 * @pre     None.
 * @param   None.
 * @return  The interrupt status, which indicates the previous state of the global interrupts.
 * 
 * @b Example
 * @code
 * bool prevStatus = INTC_Disable();
 * @endcode
 * 
 * @remarks None.
 */
bool INTC_Disable( void );

/**
 * @brief   Restores the state of global interrupts before the disable occurred.
 * @details This function restores the state of global interrupts to the specified 
 *          state that was saved prior to the disable operation. This is typically used 
 *          to re-enable interrupts after they were temporarily disabled.
 * @pre     The state must be previously saved using the INTC_Disable() function.
 * @param   state The interrupt status to restore, typically obtained from the INTC_Disable() function.
 * @return  None.
 * 
 * @b Example
 * @code
 * bool prevStatus = INTC_Disable();
 * INTC_SourceEnable(INT_SOURCE_INT0);
 * INTC_Restore(prevStatus);
 * @endcode
 * 
 * @remarks None.
 */
void INTC_Restore( bool state );


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif // PLIB_INTC_H
