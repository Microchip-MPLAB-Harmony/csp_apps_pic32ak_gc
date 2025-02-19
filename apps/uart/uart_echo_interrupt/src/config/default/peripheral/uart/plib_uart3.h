/*******************************************************************************
  uart3 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_uart3.h
 
  Summary:
    uart3 PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    uart3 peripheral.
 
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

#ifndef PLIB_UART3_H
#define PLIB_UART3_H

// /cond IGNORE_THIS
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// /endcond

// Section: Included Files

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "plib_uart_common.h"

// Section: Macro Definitions

#define UART3_FrequencyGet()    (uint32_t)(100000000UL)

// Section: UART3 PLIB Routines

/**
 * @brief    Initializes given instance of the UART peripheral
 * @pre      None
 * @param    None
 * @return   None
 */
void UART3_Initialize( void );

/**
 * @brief       Writes data to the given UART peripheral instance
 * @pre         None
 * @param[in]   serialSetup - Pointer to serial configuration data structure
 * @param[in]   clkFrequency - Input clock frequency to the Baud Rate Generator. 
 *              If configured to zero, the PLIB takes the UARTx clock frequency \ref UART3_FrequencyGet
 * @return      true  - Serial configuration was successful
 * @return      false - The specified serial configuration could not be supported. 
 *              This, for example, could happen if the requested baud is not supported.
 */
bool UART3_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq );

/**
 * @brief       Submits a write buffer to the given UART peripheral to transfer
 * @pre         None
 * @param       buffer - Pointer to the user buffer. This contains the data to be transferred.
 * @param       size - Number of bytes to be transferred. 
 * @return[Blocking mode] true - Specified number of bytes were transferred successfully or if 
 *                        the size requested is 0
 * @return[Blocking mode] false - Arguments are not valid
 * @return[Non-blocking mode] true - Request was placed successfully (in interrupt mode) or if 
 *                        the specified number of bytes were transferred or if the size requested is 0
 * @return[Non-blocking mode] false - Arguments are not valid or if the peripheral is busy with 
 *                        another request.
 */
bool UART3_Write( void *buffer, const size_t size );

/**
 * @brief       Submits a read buffer to the given UART peripheral to process
 * @pre         None
 * @param       buffer -	Pointer to the user buffer where received data will be placed.
 * @param       size -	Number of bytes to be received. 
 * @return      true - if the Read transaction is successful or if the requested size is 0.
 * @return      false - if the arguments are not valid or if the device is busy or if an error occurred while receiving data
 */
bool UART3_Read( void *buffer, const size_t size );

/**
 * @brief    Gets the error of the given UART peripheral instance.
 * @pre      None
 * @param    None
 * @return   Errors occurred as listed by @enum UART_ERROR.
 */
UART_ERROR UART3_ErrorGet( void );

/**
 * @brief       Auto-baud once enabled, this API will return the status
 * @pre         \ref UART3_AutoBaudSet has to be called before calling this function
 * @param       None
 * @return      true - if auto-baud is in-progress
 * @return      false - if auto-baud is not in-progress or complete
 */
bool UART3_AutoBaudQuery( void );

/**
 * @brief       Sets the auto-baud detect mode. The auto-baud mode stays enabled until reception 
 *              of sync field (0x55)
 * @pre         None
 * @param       enable - Pass true to enable the auto-baud detect mode
 * @return      None
 */
void UART3_AutoBaudSet( bool enable );

/**
 * @brief       Checks if any current read request in progress
 * @pre         None
 * @param       None
 * @return      true - Read request is in progress. This happens when UARTx_Read is called previously.
 * @return      false - No read request is in progress.
 */
bool UART3_ReadIsBusy( void );

/**
 * @brief       Returns number of data bytes loaded to data buffer from the read register
 * @pre         None
 * @param       None
 * @return      Number of data bytes available in the software buffer to read
 */
size_t UART3_ReadCountGet( void );

/**
 * @brief       Disables the read operation and its interrupt if enabled
 * @pre         None
 * @param       None
 * @return      true - disabled the read operation
 */
bool UART3_ReadAbort(void);

/**
 * @brief       Checks if any current write request in progress
 * @pre         None
 * @param       None
 * @return      true - Write request is in progress. This happens when UARTx_Write is called previously.
 * @return      false - No write request is in progress.
 */
bool UART3_WriteIsBusy( void );

/**
 * @brief       Returns number of data bytes written from the transmit buffer to the transmit register
 * @pre         None
 * @param       None
 * @return      Number of data bytes written from the software buffer to transmit register
 */
size_t UART3_WriteCountGet( void );

/**
 * @brief       Sets the pointer to the function and it's context to be called when the write events occur.
 * @pre         None
 * @param       callback	Pointer to the function to be called when the write transfer has completed. Setting this to NULL will disable the callback feature.
 * @param       context	    A value (usually a pointer) passed (unused) into the function identified by the callback parameter.
 * @return      None
 */
void UART3_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context );

/**
 * @brief       Sets the pointer to the function and it's context to be called when the read events occur.
 * @pre         None
 * @param       callback	Pointer to the function that will be called when a read request has completed. Setting this to NULL will disable the callback feature.
 * @param       context	    A value (usually a pointer) passed (unused) into the function identified by the callback parameter.
 * @return      None
 */
void UART3_ReadCallbackRegister( UART_CALLBACK callback, uintptr_t context );
/**
 * @brief       Checks if no current transmission is in progress
 * @pre         None
 * @param       None
 * @return      true - Transmit is not in progress or complete
 * @return      false - Transmit is in progress
 */
bool UART3_TransmitComplete( void );

// /cond IGNORE_THIS
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// /endcond

#endif // PLIB_UART3_H
