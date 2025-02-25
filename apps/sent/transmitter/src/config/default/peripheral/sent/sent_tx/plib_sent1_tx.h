/*******************************************************************************
  sent1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_sent1.h
 
  Summary:
    sent1 PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    sent1 peripheral.
 
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

#ifndef PLIB_SENT1_H
#define PLIB_SENT1_H

// /cond IGNORE_THIS   ----> DOM will ignore these comments
/* Provide C++ Compatibility */
#ifdef __cplusplus
 
    extern "C" {
 
#endif
// /endcond

// Section: Included Files

#include "device.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "plib_sent_tx_common.h"

// Section: SENT1 PLIB Routines
 
/**
 * @brief Initializes the SENT1 peripheral of the device.
 *
 * @details This function initializes the SENT1 Peripheral Library (PLIB) of the device 
 * with the values configured in the MCC GUI. Once the peripheral is initialized, 
 * APIs can be used to transfer the data and receive the data.
 *
 * @pre MCC GUI should be configured with the correct values.
 *
 * @param None
 *
 * @return None
 *
 * @note This function must be called only once and before any other SENT1 receive/transfer function is called.
 *
 * @b Example:
 * @code
 *    struct SENT_DATA_TRANSMIT data;
 *    data.data1 = 0x01;
 *    data.data2 = 0x02;
 *    data.data3 = 0x03;
 *    data.status = 0x00;
 *    SENT1_Initialize();
 *    SENT1_Transmit(data);
 * @endcode
 *
 * @remarks None
 */
void SENT1_Initialize(void);

/**
 * @brief Deinitializes the SENT1 peripheral of the device.
 *
 * @details This function deinitializes the SENT1 Peripheral Library (PLIB) of the device, 
 * returning the SENT1 peripheral to its default reset state. After calling this function, 
 * the SENT1 peripheral will no longer be operational, and any subsequent SENT1 operations 
 * will fail until re-initialization using the SENT1_Initialize API.
 *
 * @pre The SENT1 peripheral must have been initialized using the SENT1_Initialize API.
 *
 * @param None
 *
 * @return None
 *
 * @note This function should be called when the SENT1 peripheral is no longer required 
 * to release resources and ensure proper reset behavior.
 *
 * @b Example
 * @code
 * SENT1_Initialize();
 * SENT1_Enable();
 * SENT1_Deinitialize();
 * @endcode
 *
 * @remarks None
 */
void SENT1_Deinitialize(void);

/**
 * @brief Enables the SENT1 peripheral of the device.
 *
 * @details This function enables the SENT1 Peripheral Library (PLIB) of the device, 
 * for its desired use.
 *
 * @pre The SENT1 peripheral must have been initialized using the SENT1_Initialize API.
 *
 * @param None
 *
 * @return None
 *
 * @note This function should be called when the SENT1 peripheral is required 
 * to start operation
 *
 * @b Example
 * @code
 * SENT1_Initialize();
 * SENT1_Enable();
 * @endcode
 *
 * @remarks None
 */
void SENT1_Enable(void);

/**
 * @brief Disables the SENT1 peripheral of the device.
 *
 * @details This function disables the SENT1 Peripheral Library (PLIB) of the device, 
 * for its desired use
 *
 * @pre The SENT1 peripheral must have been initialized using the SENT1_Initialize API.
 * The peripheral should be enabled using the SENT1_Enable() API.
 *
 * @param None
 *
 * @return None
 *
 * @note This function should be called when the SENT1 peripheral is required 
 * to stop functioning
 * 
 * @b Example:
 * @code
 *        SENT1_Disable();
 * @endcode
 * @remarks None
 */
void SENT1_Disable(void);

/**
 * @brief      Sets the SENT1 transmit mode
 *
 * @details    This function determines how data is sent over the SENT interface. Asynchronous mode means that data frames are
 * continuously sent. Synchronous mode means that data frames are sent only when SYNCTXEN bit is set. 
 *
 * @param[in]  mode - SENT1 SYNCHRONOUS or ASYNCHRONOUS  
 *
 * @return     None  
 *  
 * @note   Ensure that the SENT1 Peripheral is properly initialized and configured before calling this function.
 *
 * @b Example:
 * @code
 *    struct SENT_DATA_TRANSMIT data;
 *    data.data1 = 0x01;
 *    data.data2 = 0x02;
 *    data.data3 = 0x03;
 *    data.status = 0x00;
 *    SENT1_Initialize();
 *    SENT1_Transmit(data);
 *    SENT1_TransmitModeSet(SENT1_TRANSMIT_SYNCHRONOUS);
 *    while(!SENT1_IsTransmissionComplete());
 *    SENT1_SyncTransmit(data);
 * @endcode
 * 
 * @remarks None 
 */
void SENT1_TransmitModeSet(SENT_TRANSMIT_MODE mode);

/**
 * @brief    Transmits the data to the receiver
 *
 * @details  This function is responsible for transmitting data for the
 * SENT peripheral. The function will handle the reception process and return the
 * received data.
 *
 * @pre  The SENT1 peripheral must have been initialized using the SENT1_Initialize API.
 * The peripheral should be enabled using the SENT1_Enable() API.
 *
 * @param[in] sentData  a structure data type \ref SENT_DATA_TRANSMIT
 *
 * @return None
 * 
 * @note   Ensure that the SENT1 Peripheral is properly initialized and configured before calling this function.
 *
 * @b Example:
 * @code
 *    struct SENT_DATA_TRANSMIT data;
 *    data.data1 = 0x01;
 *    data.data2 = 0x02;
 *    data.data3 = 0x03;
 *    data.status = 0x00;
 *    SENT1_Initialize();
 *    while(!SENT1_IsTransmissionComplete());
 *    SENT1_Transmit(data);
 * @endcode
 *
 * @remarks None
 */
void SENT1_Transmit(const SENT_DATA_TRANSMIT *sentData);

/**
 * @brief    Returns the transmit complete status
 *
 * @details  This function checks the status of the SENT transmission to determine if the 
 * current transmission has been completed. 
 *
 * @param    none
 *
 * @return   true   - SENT1 transmit completed
 * @return   false  - SENT1 transmit not completed 
 *
 * @note	Ensure that the SENT1 Peripheral is properly initialized, enabled and configured before calling this function.
 * 
 * @b Example:
 * @code
 *    struct SENT_DATA_TRANSMIT data;
 *    data.data1 = 0x01;
 *    data.data2 = 0x02;
 *    data.data3 = 0x03;
 *    data.status = 0x00;
 *    SENT1_Initialize();
 *    SENT1_Transmit(data);
 *    while(!SENT1_IsTransmissionComplete());
 *    {
 *    }
 * @endcode
 * 
 * @remarks None
 */
bool SENT1_IsTransmissionComplete(void);

/**
 * @brief    Gets the transmission status of the SENT module
 *
 * @details  This function returns the current status of the transmission process. It can be used to check
 * whether the transmission is ongoing, completed, or if there was an error during transmission.
 *
 * @param    None
 *
 * @return   Returns the SENT1 module transmission status \ref SENT_TRANSMIT_STATUS
 *  
 * @note   Ensure that the SENT1 Peripheral is properly initialized and configured before calling this function.
 *
 * @b Example:
 * @code
 *    SENT_DATA_TRANSMIT data;
 *    SENT_TRANSMIT_STATUS status;
 *    data.data1 = 0x01;
 *    data.data2 = 0x02;
 *    data.data3 = 0x03;
 *    data.status = 0x00;
 *    SENT1_Initialize();
 *    SENT1_Transmit(data);
 *    while(!SENT1_IsTransmissionComplete());
 *    SENT1_TRANSMIT_STATUS status = SENT1_TransmitStatusGet();
 * @endcode
 *
 * @remarks None  
 */
SENT_TRANSMIT_STATUS SENT1_TransmitStatusGet(void);

/**
 * @brief      Registers a callback function to be called upon completing transmission for the SENT1 Peripheral
 *
 * @details    This function registers a callback upon completing transmission. The callback function allows the user to define
 * custom actions to be taken when error occurs, such as processing the received data or signaling other parts of the application.
 *
 * @param[in] callback_fn The user-defined callback function to be registered. This function should match the
 *                        signature defined by `SENT_ERROR_CALLBACK`.
 * @param[in] context     A user-defined context value that will be passed to the callback function when it is invoked.
 *                        This can be used to provide additional information or state to the callback function.
 *
 * @return     none 
 *
 * @note Ensure that the SENT1 is properly initialized and configured before calling this function.
 * The function does not perform any error checking related to the initialization state of the SENT instance.
 *
 * @b Example:
 * @code
 * void MyTransmitCompleteCallback(uintptr_t context) {
 *     
 *     
 * }
 *
 * 		SENT1_Initialize();
 *      SENT1_TransmitCompleteCallbackRegister(MyReceiveCompleteCallback, (uintptr_t)myContext);
 * 
 * @endcode
 *
 * @remarks     None  
 */
void SENT1_TransmitCompleteCallbackRegister(SENT_TRANSMIT_COMPLETE_CALLBACK callback_fn, uintptr_t context);
#endif  // SENT1_H
/**
 End of File
*/
