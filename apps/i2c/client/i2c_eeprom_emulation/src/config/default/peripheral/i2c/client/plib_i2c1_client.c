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

// Section: Included Files

#include "device.h"
#include "plib_i2c1_client.h"
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

volatile static I2C_CLIENT_OBJ i2c1Obj;

void I2C1_Initialize(void)
{
    _I2C1IE = 0U;
    _I2C1EIE = 0U;

    I2C1CON1 = (_I2C1CON1_PCIE_MASK
                |_I2C1CON1_SCIE_MASK
                |I2C1CON1_SMBEN_DISABLED
                |I2C1CON1_SDAHT_100NS);
                
    I2C1INTC = (_I2C1INTC_BCLIE_MASK
                |_I2C1INTC_CADDRIE_MASK
                |_I2C1INTC_CDTXIE_MASK
                |_I2C1INTC_CDRXIE_MASK
                |_I2C1INTC_CLTIE_MASK);

    I2C1ADD = 0x54UL;
    I2C1MSK = 0x0UL;

    /* Clear client interrupt flag */
    _I2C1IF = 0U;

    /* Clear fault interrupt flag */
    _I2C1EIF = 0U;

    /* Enable the I2C Client interrupt */
    _I2C1IE = 1U;

    /* Enable the I2C Bus collision interrupt */
    _I2C1EIE = 1U;

    i2c1Obj.callback = NULL;

    /* Turn on the I2C module */
    I2C1CON1bits.ON = 1U;
}

void I2C1_Deinitialize(void)
{
    /* Turn off the I2C module */
    I2C1CON1bits.ON = 0U;
    
    /* Disable the I2C Bus collision interrupt */
    _I2C1EIE = 0U;
    /* Disable the I2C Client interrupt */
    _I2C1IE = 0U;
    
    I2C1CON1 = (_I2C1CON1_SCLREL_MASK);
    I2C1INTC = 0x0UL;  

    I2C1ADD = 0x0UL;
    I2C1MSK = 0x0UL;

    i2c1Obj.callback = NULL;  
}

/* I2C client state machine */
static void I2C1_TransferSM(void)
{
    uint32_t i2c_addr;
    uintptr_t context = i2c1Obj.context;

    /* ACK the client interrupt */
    IFS2bits.I2C1IF = 0U;

    if (I2C1STAT1bits.I2COV != 0U)
    {
        if (i2c1Obj.callback != NULL)
        {
            (void)i2c1Obj.callback(I2C_CLIENT_TRANSFER_EVENT_STOP_BIT_RECEIVED, context);
        }
    }
    else if (I2C1STAT1bits.D_A == 0U)
    {
        if (I2C1STAT1bits.RBF != 0U)
        {
            /* Received I2C address must be read out */
            i2c_addr = I2C1RCV;
            (void)i2c_addr;
            /* Notify that a address match event has occurred */
            if (i2c1Obj.callback != NULL)
            {
                if (i2c1Obj.callback(I2C_CLIENT_TRANSFER_EVENT_ADDR_MATCH, context) == true)
                {
                    if (I2C1STAT1bits.R_W != 0U)
                    {
                        /* I2C master wants to read */
                        if (I2C1STAT1bits.TBF == 0U)
                        {
                            /* In the callback, client must write to transmit register by calling I2Cx_WriteByte() */
                            (void)i2c1Obj.callback(I2C_CLIENT_TRANSFER_EVENT_TX_READY, context);
                        }
                    }
                    /* Send ACK */
                    I2C1CON1bits.ACKDT = 0U;
                }
                else
                {
                    /* Send NAK */
                    I2C1CON1bits.ACKDT = 1U;
                }
            }
        }
    }
    else
    {
        /* Master reads from client, client transmits */
        if (I2C1STAT1bits.R_W != 0U)
        {
            if (((I2C1STAT1 & (_I2C1STAT1_TBF_MASK | _I2C1STAT1_ACKSTAT_MASK))  == 0U))
            {
                if (i2c1Obj.callback != NULL)
                {
                    /* I2C master wants to read. In the callback, client must write to transmit register */
                    (void)i2c1Obj.callback(I2C_CLIENT_TRANSFER_EVENT_TX_READY, context);

                }
            }
        }
        /* Master writes to client, client receives */
        else
        {
            if ((I2C1STAT1 & _I2C1STAT1_RBF_MASK) != 0U)
            {
                if (i2c1Obj.callback != NULL)
                {
                    /* I2C master wants to write. In the callback, client must read data by calling I2Cx_ReadByte()  */
                    if (i2c1Obj.callback(I2C_CLIENT_TRANSFER_EVENT_RX_READY, context) == true)
                    {
                        /* Send ACK */
                        I2C1CON1bits.ACKDT = 0U;
                    }
                    else
                    {
                        /* Send NAK */
                        I2C1CON1bits.ACKDT = 1U;
                    }

                }
            }
        }
    }
    // Release clock stretch on 9th bit
    I2C1CON1bits.SCLREL = 1U;

}

void I2C1_CallbackRegister(I2C_CLIENT_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback != NULL)
    {
        i2c1Obj.callback = callback;
        i2c1Obj.context = contextHandle;
    }
}

bool I2C1_IsBusy(void)
{
    return ((I2C1STAT1 & _I2C1STAT1_S_MASK) != 0U);
}

uint8_t I2C1_ReadByte(void)
{
    return (uint8_t)I2C1RCV;
}

void I2C1_WriteByte(uint8_t wrByte)
{
    if ((I2C1STAT1 & _I2C1STAT1_TBF_MASK)  == 0U)
    {
        I2C1TRN = wrByte;
        i2c1Obj.lastByteWritten = wrByte;
    }
}

I2C_CLIENT_TRANSFER_DIR I2C1_TransferDirGet(void)
{
    return ((I2C1STAT1 & _I2C1STAT1_R_W_MASK) != 0U) ? I2C_CLIENT_TRANSFER_DIR_READ : I2C_CLIENT_TRANSFER_DIR_WRITE;
}

I2C_CLIENT_ACK_STATUS I2C1_LastByteAckStatusGet(void)
{
    return ((I2C1STAT1 & _I2C1STAT1_ACKSTAT_MASK) != 0U) ? I2C_CLIENT_ACK_STAT_RECEIVED_NAK : I2C_CLIENT_ACK_STAT_RECEIVED_ACK;
}

I2C_CLIENT_ERROR I2C1_ErrorGet(void)
{
    I2C_CLIENT_ERROR error;

    error = i2c1Obj.error;
    i2c1Obj.error = I2C_CLIENT_ERROR_NONE;

    return error;
}

void __attribute__((used)) I2C1E_InterruptHandler(void)
{
    /* Clear the bus collision error status bit */
    I2C1STAT1bits.BCL = 0U;

    /* ACK the bus interrupt */
    _I2C1EIF = 0U;

    i2c1Obj.error = I2C_CLIENT_ERROR_BUS_COLLISION;

    if (i2c1Obj.callback != NULL)
    {
        uintptr_t context = i2c1Obj.context;

        (void) i2c1Obj.callback(I2C_CLIENT_TRANSFER_EVENT_ERROR, context);
    }
}

void __attribute__((used)) I2C1_InterruptHandler(void)
{
    I2C1_TransferSM();
}
