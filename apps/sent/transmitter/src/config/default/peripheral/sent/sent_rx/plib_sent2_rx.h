/*******************************************************************************
  sent2 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_sent2.h
 
  Summary:
    sent2 PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    sent2 peripheral.
 
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

#ifndef SENT2_H
#define SENT2_H

// /cond IGNORE_THIS   ----> DOM will ignore these comments
/* Provide C++ Compatibility */
#ifdef __cplusplus
 
    extern "C" {
 
#endif
// /endcond

// Section: Included Files

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "plib_sent_rx_common.h"

// Section: SENT2 PLIB Routines
 
/**
 * @brief Initializes the SENT2 peripheral of the device.
 *
 * @details This function initializes the SENT2 Peripheral Library (PLIB) of the device 
 * with the values configured in the MCC GUI. Once the peripheral is initialized, 
 * APIs can be used to transfer the data and receive the data.
 *
 * @pre MCC GUI should be configured with the correct values.
 *
 * @param None
 *
 * @return None
 *
 * @note This function must be called only once and before any other SENT2 receive/transfer function is called.
 *
 * @b Example:
 * @code
 *    struct SENT_DATA_TRANSMIT data;
 *    data.data1 = 0x01;
 *    data.data2 = 0x02;
 *    data.data3 = 0x03;
 *    data.status = 0x00;
 *    SENT2_Initialize();
 *    SENT2_Transmit(data);
 * @endcode
 * 
 * remarks None
 */
void SENT2_Initialize(void);

/**
 * @brief Deinitializes the SENT2 peripheral of the device.
 *
 * @details This function deinitializes the SENT2 Peripheral Library (PLIB) of the device, 
 * returning the SENT2 peripheral to its default reset state. After calling this function, 
 * the SENT2 peripheral will no longer be operational, and any subsequent SENT2 operations 
 * will fail until re-initialization using the SENT2_Initialize API.
 *
 * @pre The SENT2 peripheral must have been initialized using the SENT2_Initialize API.
 *
 * @param None
 *
 * @return None
 *
 * @note This function should be called when the SENT2 peripheral is no longer required 
 * to release resources and ensure proper reset behavior.
 *
 * @b Example
 * @code
 * SENT2_Initialize();
 * SENT2_Enable();
 * SENT2_Deinitialize();
 * @endcode
 *
 * @remarks None
 */
void SENT2_Deinitialize(void);


/**
 * @brief Enables the SENT2 peripheral of the device.
 *
 * @details This function enables the SENT2 Peripheral Library (PLIB) of the device, 
 * for its desired use.
 *
 * @pre The SENT2 peripheral must have been initialized using the SENT2_Initialize API.
 *
 * @param None
 *
 * @return None
 *
 * @note This function should be called when the SENT2 peripheral is required 
 * to start operation
 *
 * @b Example
 * @code
 * SENT2_Initialize();
 * SENT2_Enable();
 * @endcode
 *
 * @remarks None
 */
void SENT2_Enable(void);

/**
 * @brief Disables the SENT2 peripheral of the device.
 *
 * @details This function disables the SENT2 Peripheral Library (PLIB) of the device, 
 * for its desired use
 *
 * @pre The SENT2 peripheral must have been initialized using the SENT2_Initialize API.
 * The peripheral should be enabled using the SENT2_Enable() API.
 *
 * @param None
 *
 * @return None
 *
 * @note This function should be called when the SENT2 peripheral is required 
 * to disable
 * 
 * @b Example:
 * @code
 *        SENT2_Disable();
 * @endcode
 * @remarks None
 */
void SENT2_Disable(void);

/**
 *
 * @brief    Reads the received data from transmitter
 *
 * @details  This function is responsible for receiving data transmitted for the
 * specified SENT instance. The function will handle the reception process and return the
 * received data.
 *
 * @pre  The SENT2 peripheral must have been initialized using the SENT2_Initialize API.
 * The peripheral should be enabled using the SENT2_Enable() API.
 * @param    none
 *
 * @return   \ref SENT_DATA_RECEIVE - Received data structure
 * 
 * @b Example:
 * @code
 *    SENT2_Initialize();
 *    while(SENT2_IsDataReceived());
 *    struct SENT_DATA_RECEIVE sentData = SENT2_Receive(void);
 * @endcode
 * @remarks None
    
 */
SENT_DATA_RECEIVE SENT2_Receive(void);

/**
 * @brief     Checks if data has been received for SENT Peripheral.
 *
 * @details   The function returns a boolean value indicating whether new data has been successfully
 * received and is available for processing. 
 *
 * @pre   The SENT2 peripheral must have been initialized using the SENT2_Initialize API.
 * The peripheral should be enabled using the SENT2_Enable() API.
 *
 * @param    none
 * @return   true   - SENT2 receive completed
 * @return   false  - SENT2 receive not completed
   
 * 
 * @b Example:
 * @code
 *    struct SENT_DATA_RECEIVE data;
 *    SENT2_Initialize();
 *    data = SENT2_Receive();
 *    while(!SENT2_IsDataReceived());
 *    {
 *    }
 * @endcode
 * @remarks None 
 */
 bool SENT2_IsDataReceived(void);

/**
 * @brief    Retrieves the current receive status SENT peripheral.
 *
 * @details  The function indicates the current status of the data reception process.
 * This status can provide information about whether data has been successfully received,
 * if there are any errors, or if the reception is still in progress.
 * @param    none
 * @return   Returns the SENT2 module reception status \ref SENT2_RECEIVE_STATUS
 *  
 *@b Example:
 * @code
 *    struct SENT_DATA_RECEIVE data;
 *    enum SENT_RECEIVE_STATUS status;
 *    SENT2_Initialize();
 *    data = SENT2_Receive(data);
 *    while(!SENT2_IsDataReceived());
 *    enum SENT2_RECEIVE_STATUS status = SENT2_ReceiveStatusGet();
 * @endcode
 * @remarks None   
 */
SENT_RECEIVE_STATUS SENT2_ReceiveStatusGet(void);

/** 
 * @brief    Returns the type of reception error
 * @details  This function returns the most recent error code that was recorded during reception.
 *
 * @param    none
 * @return   Returns the SENT2 module reception error \ref SENT_ERROR_CODE
 *
 * @note 	 Ensure that the SENT instance is properly initialized and configured before calling this function.
 *
 * @b Example:
 * @code
 *	SENT_ERROR_CODE errorCode = ReceiveErrorGet();
 * 	if (errorCode != NO_ERROR) {
 *     // Handle the error accordingly
 * }
 * @endcode
 *
 * @remarks None
 */
 SENT_ERROR_CODE ReceiveErrorGet(void);

/**
 * @brief      Registers a callback function to be called upon the completion of data reception for the SENT2 Peripheral
 * @details    This function registers a callback upon completion of data reception. The callback function allows the user to define
 * custom actions to be taken when data reception is complete, such as processing the received data or signaling other parts of the application.
 *
 * @param[in] callback_fn The user-defined callback function to be registered. This function should match the
 *                        signature defined by `SENT_RECEIVE_COMPLETE_CALLBACK`.
 * @param[in] context     A user-defined context value that will be passed to the callback function when it is invoked.
 *                        This can be used to provide additional information or state to the callback function.
 *
 * @return     none 
 *
 * @note Ensure that the SENT instance is properly initialized and configured before calling this function.
 * The function does not perform any error checking related to the initialization state of the SENT instance.
 *
 * @b Example: 
 * @code
 * void MyReceiveCompleteCallback(uintptr_t context) {
 *     
 *     
 * }
 *
 *		SENT2_Initialize();
 *      SENT2_ReceiveCompleteCallbackRegister(MyReceiveCompleteCallback, (uintptr_t)myContext);
 * }
 * @endcode
 * @remarks     None  
 */
void SENT2_ReceiveCompleteCallbackRegister(SENT_RECEIVE_COMPLETE_CALLBACK callback_fn, uintptr_t context);

/**
 * @brief      Registers a callback function to be called upon encountering an error during data reception for the SENT2 Peripheral
 * @details    This function registers a callback upon encountering an error during data reception. The callback function allows the user to define
 * custom actions to be taken when error occurs, such as processing the received data or signaling other parts of the application.
 *
 * @param[in] callback_fn The user-defined callback function to be registered. This function should match the
 *                        signature defined by `SENT_ERROR_CALLBACK`.
 * @param[in] context     A user-defined context value that will be passed to the callback function when it is invoked.
 *                        This can be used to provide additional information or state to the callback function.
 *
 * @return     none 
 *
 * @note Ensure that the SENT instance is properly initialized and configured before calling this function.
 * The function does not perform any error checking related to the initialization state of the SENT instance.
 *
 * @b Example:
 * @code
 * void MyErrorReceiveCompleteCallback(uintptr_t context) {
 *     
 *     
 * }
 *
 * 		SENT2_Initialize();
 *      SENT2_ReceiveCompleteCallbackRegister(MyReceiveCompleteCallback, (uintptr_t)myContext);
 * 
 * @endcode
 * @remarks     None  
 */
void SENT2_ErrorCallbackRegister(SENT_ERROR_CALLBACK callback_fn, uintptr_t context);

// /cond IGNORE_THIS
/* Provide C++ Compatibility */
#ifdef __cplusplus
 
    }
 
#endif
// /endcond

#endif  // SENT2_H
/**
 End of File
*/
