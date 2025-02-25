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

#ifndef PLIB_I2C1_HOST_H
#define PLIB_I2C1_HOST_H

// Section: Included Files

#include "plib_i2c_host_common.h"

// /cond IGNORE_THIS
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {


#endif
// /endcond

// Section: Interface Routines

/** 
 * @brief           Initializes the instance of the I2C peripheral operating in I2C mode.
 * @details         This function initializes the given instance of the I2C peripheral as
 *                  configured by the user from the MHC.
 * @pre             None
 * @param           None
 * @returns         None
 * @remarks         Stops the I2C if it was already running and reinitializes it
 */
void I2C1_Initialize(void);

/** 
 * @brief           De-initializes the instance of the I2C peripheral
 * @details         This function resets the used registers to POR values
 *                  configured by the user from the MHC.
 * @pre             None
 * @param           None
 * @returns         None
 */
void I2C1_Deinitialize(void);

/**
 * @breif           Reads data from the client.
 * @details         This function reads the data from a client on the bus. The function will
 *                  attempt to read length number of bytes into pdata buffer from a client whose
 *                  address is specified as address. The I2C Host generate a Start condition,
 *                  read the data and then generate a Stop Condition.
 *                  If the client NAKs the request or a bus error is encountered on the bus, the
 *                  transfer is terminated. The application can call I2C1_ErrorGet()
 *                  function to know that cause of the error.
 *
 *                  The function is non-blocking. It initiates bus activity and returns
 *                  immediately. The transfer is completed in the peripheral interrupt. A
 *                  transfer request cannot be placed when another transfer is in progress.
 *                  Calling the read function when another function is already in progress will
 *                  cause the function to return false.
 *
 *                  The library will call the registered callback function when the transfer has
 *                  terminated if callback is registered.
 * @pre             I2C1_Initialize must have been called for the associated I2C instance.
 * @param           address -   7-bit / 10-bit client address.
 *                  data    -   pointer to destination data buffer where the received data should 
 *                              be stored.
 *                  length  -   length of data buffer in number of bytes. Also the number of bytes
 *                              to be read.
 * @return          true  -     The request was placed successfully and the bus activity was
                                initiated.
                    false -     The request fails,if there was already a transfer in progress when this
                                function was called.
  @remarks          None.
*/
bool I2C1_Read(uint16_t address, uint8_t* rdata, size_t rlength);

/**
 * @breif           Writes data to the client.
 * @details         This function writes data to a client on the bus. The function will attempt
 *                  to write length number of bytes from pdata buffer to a client whose address
 *                  is specified by address. The I2C Host will generate a Start condition,
 *                  write the data and then generate a Stop Condition. If the client NAKs the request
 *                  or a bus error was encountered on the bus, the transfer is terminated. The
 *                  application can call the I2C1_ErrorGet() function to know that
 *                  cause of the error.
 * 
 *                  The function is non-blocking. It initiates bus activity and returns
 *                  immediately. The transfer is then completed in the peripheral interrupt. A
 *                  transfer request cannot be placed when another transfer is in progress.
 *                  Calling the write function when another function is already in progress will
 *                  cause the function to return false.
 *
 *                  The library will call the registered callback function when the transfer has
 *                  terminated.
 * @pre             I2C1_Initialize must have been called for the associated I2C instance.
 * @param           address -   7-bit / 10-bit client address.
 *                  pdata   -   pointer to source data buffer that contains the data to be
 *                              transmitted.
 *                  length  -   length of data buffer in number of bytes. Also the number of bytes
 *                              to be written.
 * @return          true  - The request was placed successfully and the bus activity was initiated.
 *                  false - The request fails,if there was already a transfer in progress when this function was called
 *
  @remarks          None
*/
bool I2C1_Write(uint16_t address, uint8_t* wdata, size_t wlength);

/**
 * @breif           Write and Read data from Slave.
 * @details         This function writes data from the wdata to the bus and then reads data from
 *                  the client and stores the received in the rdata. The function generates a
 *                  Start condition on the bus and will then send wlength number of bytes
 *                  contained in wdata. The function will then insert a Repeated start condition
 *                  and proceeed to read rlength number of bytes from the client. The received
 *                  bytes are stored in rdata buffer. A Stop condition is generated after the
 *                  last byte has been received.
 *
 *                  If the client NAKs the request or a bus error was encountered on the bus,
 *                  the transfer is terminated. The application can call I2C1_ErrorGet()
 *                  function to know that cause of the error.
 *
 *                  The function is non-blocking. It initiates bus activity and returns
 *                  immediately. The transfer is then completed in the peripheral interrupt. A
 *                  transfer request cannot be placed when another transfer is in progress.
 *                  Calling this function when another function is already in progress will
 *                  cause the function to return false.
 *
 *                  The library will call the registered callback function when the transfer has
 *                  terminated.
 *
 * @pre             I2C1_Initialize must have been called for the associated I2C instance.
 * @param           address -   7-bit / 10-bit client address.
 *                  wdata   -   pointer to write data buffer
 *                  wlength -   write data length in bytes.
 *                  rdata   -   pointer to read data buffer.
 *                  rlength -   read data length in bytes.
 *
 *  @return         true  -     The request was placed successfully and the bus activity was
 *                              initiated.
 *                  false -     The request fails, if there was already a transfer in progress when this
 *                              function was called.
 * @remarks         Calling this function is not the same as calling the I2C1_Write()
                    function and then calling the I2C1_Read() function.
                    The I2C1_WriteRead function will insert a Repeated Start
                    condition between the Write and the Read stages. The I2C1_Write()
                    and the I2C1_Read() function insert a stop condtion after
                    the write and the read has completed.
*/
bool I2C1_WriteRead(uint16_t address, uint8_t* wdata, size_t wlength, uint8_t* rdata, size_t rlength);

/**
 * @breif           Returns the Peripheral busy status.
 * 
 * @details         This function ture if the I2C I2C1I2C module is busy with a
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
 * @breif       Returns the error occured during transfer.
 * @details     This function returns the error during transfer.
 * @pre         I2C1_Initialize must have been called for the
 *              associated I2C instance.
 * @param       None.
 * @return      Returns an I2C_ERROR type of status identifying the error that has
 *              occurred.
 * @remarks      None
*/
I2C_ERROR I2C1_ErrorGet(void);

/**
 * @breif           Sets the pointer to the function (and it's context) to be called when the
 *                  given I2C's transfer events occur.
 *
 * @details         This function sets the pointer to a host function to be called "back" when
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
void I2C1_CallbackRegister(I2C_CALLBACK callback, uintptr_t contextHandle);

/**
 * @breif           Dynamic setup of I2C Peripheral.
 * @pre             I2C1_Initialize must have been called for the associated I2C instance.
 *                  The transfer status should not be busy.
 * @param           setup -     Pointer to the structure containing the transfer setup.
 *                  srcClkFreq - I2C Peripheral Clock Source Frequency.
 * @return          true - Transfer setup was updated Successfully.
 *                  false - Failure while updating transfer setup.
 * @remarks     srcClkFreq overrides any change in the peripheral clock frequency.
 *              If configured to zero PLib takes the peripheral clock frequency from MHC.
*/
bool I2C1_TransferSetup(I2C_TRANSFER_SETUP* setup, uint32_t srcClkFreq );

/**
 * @breif           Force stops the I2C transfer
 * @pre             None
 * @param           None
 * @return          None
 **/ 
void I2C1_TransferAbort( void );

// /cond IGNORE_THIS
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// /endcond

#endif /* PLIB_I2C1_HOST_H */