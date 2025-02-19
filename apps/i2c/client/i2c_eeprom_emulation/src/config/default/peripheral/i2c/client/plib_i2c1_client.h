/*******************************************************************************
  i2c1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_i2c1.h
 
  Summary:
    i2c1 PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    i2c1 peripheral.
 
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

#ifndef PLIB_I2C1_CLIENT_H
#define PLIB_I2C1_CLIENT_H

// Section: Included Files

#include "plib_i2c_client_common.h"

// /cond IGNORE_THIS
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// /endcond

// Section: Interface Routines

/** 
 * @brief           Initializes the instance of the I2C peripheral operating in I2C mode.
 * @description     This function initializes the given instance of the I2C peripheral as
 *                  configured by the user from the MHC.
 * @pre             None
 * @param           None
 * @returns         None
 * @remarks         Stops the I2C if it was already running and reinitializes it
 */
void I2C1_Initialize(void);

/** 
 * @brief           De-initializes the instance of the I2C peripheral
 * @description     This function resets the used registers to POR values
 *                  configured by the user from the MHC.
 * @pre             None
 * @param           None
 * @returns         None
 */
void I2C1_Deinitialize(void);

/**
 * @breif           Sets the pointer to the function (and it's context) to be called when the
 *                  given I2C's transfer events occur.
 *
 * @description     This function sets the pointer to a host function to be called "back" when
 *                  the given I2C's transfer events occur. It also passes a context value
 *                  (usually a pointer to a context structure) that is passed into the function
 *                  when it is called. The specified callback function will be called from the
 *                  peripheral interrupt context.
 *
 * @pre             I2C1_Initialize must have been called for the associated
 *                  I2C instance.
 * @param           callback -      A pointer to a function with a calling signature defined by
 *                                  the I2C_CALLBACK data type. Setting this to NULL
 *                                  disables the callback feature.
 *                  contextHandle - A value (usually a pointer) passed (unused) into the
 *                                  function identified by the callback parameter.
 * @return          None.
 * @remarks         None
*/
void I2C1_CallbackRegister(I2C_CLIENT_CALLBACK callback, uintptr_t contextHandle);

/**
 * @breif           Returns the Peripheral busy status.
 * 
 * @description     This function ture if the I2C I2C1I2C module is busy with a
 *                  transfer. The application can use the function to check if I2C
 *                  I2C1I2C module is busy before calling any of the data transfer
 *                  functions. The library does not allow a data transfer operation if another
 *                  transfer operation is already in progress.
 * @pre             I2C1_Initialize must have been called for the
 *                  associated I2C instance.
 * @param           None.
 * @return          true - Busy.
 *                  false - Not busy.
 * @remarks          None
*/
bool I2C1_IsBusy(void);

/**
 * @breif       This function returns the I2C data byte
 * @description This function returns read data byte post address match
 * @pre         I2C1_Initialize must have been called for the
 *              associated I2C instance.
 * @param       None.
 * @return      Returns one byte of data read from the receive register 
 * @remarks      None
*/
uint8_t I2C1_ReadByte(void);

/**
 * @breif       Writes byte data
 * @description This function writes one byte of data into I2C bus
 * @pre         I2C1_Initialize must have been called for the
 *              associated I2C instance.
 * @param       wrByte - byte data to be written
 * @return      None
 * @remarks      None
*/
void I2C1_WriteByte(uint8_t wrByte);

/**
 * @breif       Read/Write request status   
 * @description This function returns if the request direction is read or write
 * @pre         I2C1_Initialize must have been called for the
 *              associated I2C instance.
 * @param       wrByte - byte data to be written
 * @return      None
 * @remarks      None
*/
I2C_CLIENT_TRANSFER_DIR I2C1_TransferDirGet(void);

/**
 * @breif       Writes byte data
 * @description This function returns the ACK status of the last byte written to the I2C master
 * @pre         I2C1_Initialize must have been called for the
 *              associated I2C instance.
 * @param       I2C_CLIENT_ACK_STATUS_RECEIVED_ACK - I2C master acknowledged the last byte 
 *              I2C_CLIENT_ACK_STATUS_RECEIVED_NAK - I2C master sent NAK 
 * @return      None
 * @remarks      None
*/
I2C_CLIENT_ACK_STATUS I2C1_LastByteAckStatusGet(void);

/**
 * @breif       Returns the error occured during transfer.
 * @description This function returns the error during transfer.
 * @pre         I2C1_Initialize must have been called for the
 *              associated I2C instance.
 * @param       None.
 * @return      Returns an I2C_CLIENT_ERROR type of status identifying the error that has
 *              occurred.
 * @remarks      None
*/
I2C_CLIENT_ERROR I2C1_ErrorGet(void);


// /cond IGNORE_THIS
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// /endcond

#endif /* PLIB_I2C1_CLIENT_H */