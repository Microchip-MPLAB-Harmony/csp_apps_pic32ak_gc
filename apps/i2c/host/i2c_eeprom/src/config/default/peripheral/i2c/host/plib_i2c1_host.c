/*******************************************************************************
  i2c1 PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_i2c1.c
 
  Summary:
    i2c1 PLIB Source File
 
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

#include "device.h"
#include "plib_i2c1_host.h"
#include "interrupts.h"

// Section: Macro Definitions

//I2CxCON SMBEN options
#define I2C1CON1_SMBEN_DISABLED          ((uint32_t)(_I2C1CON1_SMBEN_MASK & ((uint32_t)(0) << _I2C1CON1_SMBEN_POSITION))) 
#define I2C1CON1_SMBEN_SMBUS_2_0          ((uint32_t)(_I2C1CON1_SMBEN_MASK & ((uint32_t)(1) << _I2C1CON1_SMBEN_POSITION))) 
#define I2C1CON1_SMBEN_SMBUS_3_0          ((uint32_t)(_I2C1CON1_SMBEN_MASK & ((uint32_t)(2) << _I2C1CON1_SMBEN_POSITION))) 

//I2CxCON SDHAT options
#define I2C1CON1_SDAHT_100NS          ((uint32_t)(_I2C1CON1_SDAHT_MASK & ((uint32_t)(0) << _I2C1CON1_SDAHT_POSITION))) 
#define I2C1CON1_SDAHT_300NS          ((uint32_t)(_I2C1CON1_SDAHT_MASK & ((uint32_t)(1) << _I2C1CON1_SDAHT_POSITION))) 

// Section: Global Data

#define NOP asm(" NOP")
volatile static I2C_HOST_OBJ i2c1Obj;
void I2C1E_InterruptHandler(void);
void I2C1_InterruptHandler(void);

void I2C1_Initialize(void)
{

    _I2C1IE = 0U;
    _I2C1EIE = 0U;

    I2C1HBRG = 0x1ddUL; 
    I2C1LBRG = 0x1ddUL;

    I2C1CON1 = (I2C1CON1_SMBEN_DISABLED
                        |I2C1CON1_SDAHT_100NS
                        |_I2C1CON1_SCLREL_MASK
                        |_I2C1CON1_DISSLW_MASK);
    I2C1INTC = (_I2C1INTC_BCLIE_MASK
                        |_I2C1INTC_HPCIE_MASK
                        |_I2C1INTC_HSCIE_MASK
                        |_I2C1INTC_HACKSIE_MASK
                        |_I2C1INTC_HDTXIE_MASK
                        |_I2C1INTC_HDRXIE_MASK
                        |_I2C1INTC_HSTIE_MASK);   

    /* Clear host interrupt flag */
    _I2C1IF = 0U;

    /* Clear fault interrupt flag */
    _I2C1EIF = 0U;

    /* Turn on the I2C module */
    I2C1CON1bits.ON = 1U;
    
    i2c1Obj.callback = NULL;  

    /* Set the initial state of the I2C state machine */
    i2c1Obj.state = I2C_STATE_IDLE;
}

void I2C1_Deinitialize(void)
{
    /* Turn off the I2C module */
    I2C1CON1bits.ON = 0U;
    
    /* Clear host interrupt flag */
    _I2C1IF = 0U;
    /* Clear fault interrupt flag */
    _I2C1EIF = 0U;
    
    I2C1CON1 = (_I2C1CON1_SCLREL_MASK);
    I2C1INTC = 0x0UL;  
    
    I2C1HBRG = 0x0UL; 
    I2C1LBRG = 0x0UL;
    
    i2c1Obj.callback = NULL;  

}

/* I2C state machine */
static void I2C1_TransferSM(void)
{
    uint8_t tempVar = 0;
    _I2C1IF = 0;
    switch (i2c1Obj.state)
    {
        case I2C_STATE_START_CONDITION:
            /* Generate Start Condition */
            _I2C1IE = 1U;
            _I2C1EIE= 1U;
            I2C1CON1bits.SEN = 1;
            i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND;
            break;

        case I2C_STATE_ADDR_BYTE_1_SEND:
            /* Is transmit buffer full? */
            if (I2C1STAT1bits.TBF == 0U)
            {
                if (i2c1Obj.address > 0x007FU)
                {
                    tempVar = (((volatile uint8_t*)&i2c1Obj.address)[1] << 1);
                    /* Transmit the MSB 2 bits of the 10-bit slave address, with R/W = 0 */
                    I2C1TRN = (uint32_t)( 0xF0U | (uint32_t)tempVar);
                    i2c1Obj.state = I2C_STATE_ADDR_BYTE_2_SEND;
                }
                else
                {
                    /* 8-bit addressing mode */
                    I2C_TRANSFER_TYPE transferType = i2c1Obj.transferType;
                    I2C1TRN = (((uint32_t)i2c1Obj.address << 1U) | (uint32_t)transferType);
                    if (i2c1Obj.transferType == I2C_TRANSFER_TYPE_WRITE)
                    {
                        i2c1Obj.state = I2C_STATE_WRITE;
                    }
                    else
                    {
                        i2c1Obj.state = I2C_STATE_READ;
                    }
                }
            }
            break;

        case I2C_STATE_ADDR_BYTE_2_SEND:
            /* Transmit the 2nd byte of the 10-bit slave address */
            if (I2C1STAT1bits.ACKSTAT == 0U)
            {
                if (I2C1STAT1bits.TBF == 0U)
                {
                    /* Transmit the remaining 8-bits of the 10-bit address */
                    I2C1TRN = i2c1Obj.address;

                    if (i2c1Obj.transferType == I2C_TRANSFER_TYPE_WRITE)
                    {
                        i2c1Obj.state = I2C_STATE_WRITE;
                    }
                    else
                    {
                        i2c1Obj.state = I2C_STATE_READ_10BIT_MODE;
                    }
                }
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CON1bits.PEN = 1U;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_READ_10BIT_MODE:
            if (I2C1STAT1bits.ACKSTAT == 0U)
            {
                /* Generate repeated start condition */
                I2C1CON1bits.RSEN = 1U;
                i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY;
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CON1bits.PEN = 1U;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY:
            /* Is transmit buffer full? */
            if (I2C1STAT1bits.TBF == 0U)
            {
                tempVar = (((volatile uint8_t*)&i2c1Obj.address)[1] << 1);
                /* Transmit the first byte of the 10-bit slave address, with R/W = 1 */
                I2C1TRN = (uint32_t)( 0xF1U | (uint32_t)tempVar);
                i2c1Obj.state = I2C_STATE_READ;
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CON1bits.PEN = 1U;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_WRITE:
            if (I2C1STAT1bits.ACKSTAT == 0U)
            {
                size_t writeCount = i2c1Obj.writeCount;

                /* ACK received */
                if (writeCount < i2c1Obj.writeSize)
                {
                    if (I2C1STAT1bits.TBF == 0U)
                    {
                        /* Transmit the data from writeBuffer[] */
                        I2C1TRN = i2c1Obj.writeBuffer[writeCount];
                        i2c1Obj.writeCount++;
                    }
                }
                else
                {
                    size_t readSize = i2c1Obj.readSize;

                    if (i2c1Obj.readCount < readSize)
                    {
                        /* Generate repeated start condition */
                        I2C1CON1bits.RSEN = 1U;

                        i2c1Obj.transferType = I2C_TRANSFER_TYPE_READ;

                        if (i2c1Obj.address > 0x007FU)
                        {
                            /* Send the I2C slave address with R/W = 1 */
                            i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY;
                        }
                        else
                        {
                            /* Send the I2C slave address with R/W = 1 */
                            i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND;
                        }

                    }
                    else
                    {
                        /* Transfer Complete. Generate Stop Condition */
                        I2C1CON1bits.PEN = 1U;
                        i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
                    }
                }
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CON1bits.PEN = 1U;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_READ:
            if (I2C1STAT1bits.ACKSTAT == 0U)
            {
                /* Slave ACK'd the device address. Enable receiver. */
                I2C1CON1bits.RCEN = 1U;
                i2c1Obj.state = I2C_STATE_READ_BYTE;
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CON1bits.PEN = 1U;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_READ_BYTE:
            /* Data received from the slave */
            if (I2C1STAT1bits.RBF != 0U)
            {
                size_t readCount = i2c1Obj.readCount;

                i2c1Obj.readBuffer[readCount] = (uint8_t)I2C1RCV;
                readCount++;
                if (readCount == i2c1Obj.readSize)
                {
                    /* Send NAK */
                    I2C1CON1bits.ACKDT = 1U;
                    I2C1CON1bits.ACKEN = 1U;
                }
                else
                {
                    /* Send ACK */
                    I2C1CON1bits.ACKDT = 0U;
                    I2C1CON1bits.ACKEN = 1U;
                }
                i2c1Obj.readCount = readCount;
                i2c1Obj.state = I2C_STATE_WAIT_ACK_COMPLETE;
            }
            break;

        case I2C_STATE_WAIT_ACK_COMPLETE:
            {
                size_t readSize = i2c1Obj.readSize;
                /* ACK or NAK sent to the I2C slave */
                if (i2c1Obj.readCount < readSize)
                {
                    /* Enable receiver */
                    I2C1CON1bits.RCEN = 1U;
                    i2c1Obj.state = I2C_STATE_READ_BYTE;
                }
                else
                {
                    /* Generate Stop Condition */
                    I2C1CON1bits.PEN = 1U;
                    i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
                }
            }
            break;

        case I2C_STATE_WAIT_STOP_CONDITION_COMPLETE:
            i2c1Obj.state = I2C_STATE_IDLE;
            _I2C1IE = 0;
            _I2C1EIE = 0;
            if (i2c1Obj.callback != NULL)
            {
                uintptr_t context = i2c1Obj.context;

                i2c1Obj.callback(context);
            }
            break;

        default:
                   /*Do Nothing*/
            break;
    }
}


void I2C1_CallbackRegister(I2C_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback != NULL)
    {
       i2c1Obj.callback = callback;
       i2c1Obj.context = contextHandle;
    }
    return;
}

bool I2C1_IsBusy(void)
{
    bool busycheck = false;
    uint32_t tempVar = I2C1CON1;
    uint32_t tempVar1 = I2C1STAT1;
    if( (i2c1Obj.state != I2C_STATE_IDLE ) || ((tempVar & 0x0000001FU) != 0U) ||
        ((tempVar1 & _I2C1STAT1_TRSTAT_MASK) != 0U) || ((tempVar1 & _I2C1STAT1_S_MASK) != 0U) )
    {
        busycheck = true;
    }
    return busycheck;
}

bool I2C1_Read(uint16_t address, uint8_t* rdata, size_t rlength)
{
    bool statusRead = false;
    uint32_t tempVar = I2C1STAT1;
    /* State machine must be idle and I2C module should not have detected a start bit on the bus */
    if((i2c1Obj.state == I2C_STATE_IDLE) && (( tempVar & _I2C1STAT1_S_MASK) == 0U))
    {
        i2c1Obj.address             = address;
        i2c1Obj.readBuffer          = rdata;
        i2c1Obj.readSize            = rlength;
        i2c1Obj.writeBuffer         = NULL;
        i2c1Obj.writeSize           = 0;
        i2c1Obj.writeCount          = 0;
        i2c1Obj.readCount           = 0;
        i2c1Obj.transferType        = I2C_TRANSFER_TYPE_READ;
        i2c1Obj.error               = I2C_ERROR_NONE;
        i2c1Obj.state               = I2C_STATE_ADDR_BYTE_1_SEND;
        I2C1CON1bits.SEN = 1U;
        _I2C1IE = 1U;
        _I2C1EIE = 1U;
        statusRead = true;
    }
    return statusRead;
}


bool I2C1_Write(uint16_t address, uint8_t* wdata, size_t wlength)
{
    bool statusWrite = false;
    uint32_t tempVar = I2C1STAT1;
    /* State machine must be idle and I2C module should not have detected a start bit on the bus */
    if((i2c1Obj.state == I2C_STATE_IDLE) && (( tempVar & _I2C1STAT1_S_MASK) == 0U))
    {
        i2c1Obj.address             = address;
        i2c1Obj.readBuffer          = NULL;
        i2c1Obj.readSize            = 0;
        i2c1Obj.writeBuffer         = wdata;
        i2c1Obj.writeSize           = wlength;
        i2c1Obj.writeCount          = 0;
        i2c1Obj.readCount           = 0;
        i2c1Obj.transferType        = I2C_TRANSFER_TYPE_WRITE;
        i2c1Obj.error               = I2C_ERROR_NONE;
        i2c1Obj.state               = I2C_STATE_ADDR_BYTE_1_SEND;
        
        I2C1CON1bits.SEN = 1U;
        _I2C1IE = 1U;
        _I2C1EIE= 1U;
        statusWrite = true;
    }
    return statusWrite;
}


bool I2C1_WriteRead(uint16_t address, uint8_t* wdata, size_t wlength, uint8_t* rdata, size_t rlength)
{
    bool statusWriteread = false;
    uint32_t tempVar = I2C1STAT1;
    /* State machine must be idle and I2C module should not have detected a start bit on the bus */
    if((i2c1Obj.state == I2C_STATE_IDLE) &&
       ((tempVar & _I2C1STAT1_S_MASK) == 0U))
    {
        i2c1Obj.address             = address;
        i2c1Obj.readBuffer          = rdata;
        i2c1Obj.readSize            = rlength;
        i2c1Obj.writeBuffer         = wdata;
        i2c1Obj.writeSize           = wlength;
        i2c1Obj.writeCount          = 0;
        i2c1Obj.readCount           = 0;
        i2c1Obj.transferType        = I2C_TRANSFER_TYPE_WRITE;
        i2c1Obj.error               = I2C_ERROR_NONE;
        i2c1Obj.state               = I2C_STATE_ADDR_BYTE_1_SEND;

        I2C1CON1bits.SEN = 1U;
        _I2C1IE = 1U;
        _I2C1EIE = 1U;
        statusWriteread = true;
    }
    return statusWriteread;
}

I2C_ERROR I2C1_ErrorGet(void)
{
    I2C_ERROR error;

    error = i2c1Obj.error;
    i2c1Obj.error = I2C_ERROR_NONE;

    return error;
}

bool I2C1_TransferSetup(I2C_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
{
    uint32_t baudValue;
    uint32_t i2cClkSpeed;
    float fBaudValue;

    if (setup == NULL)
    {
        return false;
    }

    i2cClkSpeed = setup->clkSpeed;

    /* Maximum I2C clock speed cannot be greater than 1 MHz */
    if (i2cClkSpeed > 1000000UL)
    {
        return false;
    }

    if( srcClkFreq == 0U)
    {
        srcClkFreq = 100000000UL;
    }
    
    fBaudValue = (float)((((1.0f / (2.0f * (float)i2cClkSpeed)) - 0.0000002f) * (float)srcClkFreq) - 3.0f);
    baudValue = (uint32_t)fBaudValue;

    /* I2CxBRG value cannot be from 0 to 3 or more than the size of the baud rate register */
    if ((baudValue < 0x4UL) || (baudValue > 0xFFFFFFUL))
    {
        return false;
    }

    I2C1HBRG = baudValue;
    I2C1LBRG = baudValue;

    /* Enable slew rate for 400 kHz clock speed; disable for all other speeds */

    if (i2cClkSpeed == 400000U)
    {
        I2C1CON1bits.DISSLW = 0U;
    }
    else
    {
        I2C1CON1bits.DISSLW = 1U;
    }

    return true;
}

void I2C1_TransferAbort( void )
{
    i2c1Obj.error = I2C_ERROR_NONE;

    // Reset the PLib objects and Interrupts
    i2c1Obj.state = I2C_STATE_IDLE;
    _I2C1IE = 0U;
    _I2C1EIE = 0U;

    // Disable and Enable I2C Host
    I2C1CON1bits.ON = 0U;
    NOP;NOP;
    I2C1CON1bits.ON = 1U;
}

void __attribute__((used)) I2C1E_InterruptHandler(void)
{
    /* ACK the bus interrupt */
    _I2C1EIF = 0;
    
    if(I2C1STAT1bits.BCL == 1U)
    {
        /* Clear the bus collision error status bit */
        I2C1STAT1bits.BCL = 0U;

        i2c1Obj.state = I2C_STATE_IDLE;

        i2c1Obj.error = I2C_ERROR_BUS_COLLISION;
    }

    if (i2c1Obj.callback != NULL)
    {
        uintptr_t context = i2c1Obj.context;

        i2c1Obj.callback(context);
    }
}

void __attribute__((used)) I2C1_InterruptHandler(void)
{
    I2C1_TransferSM();
}
