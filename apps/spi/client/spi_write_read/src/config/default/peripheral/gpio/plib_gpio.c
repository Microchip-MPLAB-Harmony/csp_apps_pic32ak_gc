/*******************************************************************************
  GPIO PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_gpio.h
 
  Summary:
    GPIO PLIB Header File
 
  Description:
    This file has prototype of all the interfaces provided for particular
    gpio peripheral.
 
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

#include "plib_gpio.h"
#include "interrupts.h"
/**
* @brief   Offset value between registers LAT, TRIS, PORT 
*/
#define OFFSET_REG (uint32_t)0x14UL 

/**
* @brief  Offset value between two interrupt registers CNEN0, CNEN1, CNCON
*/
#define OFFSET_INT (uint32_t)0x24UL 
 
/**
* @brief   Macro to lock registers for PPS configuration.
*/
#define PINS_PPSLock()           (RPCONbits.IOLOCK = 1)

/**
* @brief   Macro to unlock registers for PPS configuration.
*/
#define PINS_PPSUnlock()         (RPCONbits.IOLOCK = 0)


/* Array to store callback objects of each configured interrupt */
volatile static GPIO_PIN_CALLBACK_OBJ portPinCbObj[1];

/* Array to store number of interrupts in each PORT Channel + previous interrupt count */
static uint8_t portNumCb[4 + 1] = { 0, 0, 1, 1, 1, };

void GPIO_Initialize ( void )
{
     /*PORTB Initialization */
    LATB = 0x0U; /* Initial Latch Value */
    TRISB = 0xfbfU; /* Direction Control */
    ANSELB= 0x33fU; /* Digital Mode Enable */
		
    /* Change Notice Enable */
    CNCONB = _CNCONB_ON_MASK;
    PORTB;
    IEC3bits.CNBIE = 1;
     /*PORTC Initialization */
    LATC = 0x0U; /* Initial Latch Value */
    TRISC = 0xff7U; /* Direction Control */

    /* Unlock system for PPS configuration */
    PINS_PPSUnlock();

 /* PPS Input Mapping */
    RPINR10bits.SCK1R = 33;
    RPINR10bits.SDI1R = 60;

 /* PPS Output Mapping */
    RPOR8bits.RP33R = 14;
    RPOR10bits.RP44R = 13;

    /* Lock back the system after PPS configuration */
    PINS_PPSLock();
        

    uint32_t cb_index;
    /* Initialize Interrupt Pin data structures */
    portPinCbObj[0].pin = GPIO_PIN_RB7;
    
    for(cb_index=0U; cb_index<1U; cb_index++)
    {
        portPinCbObj[cb_index].callback = NULL;
    }
}

// Section: GPIO APIs which operates on multiple pins of a port

uint32_t  GPIO_PortRead(GPIO_PORT port)
{
    return (*(volatile uint32_t *)((uint32_t)&PORTA + (port * OFFSET_REG)));
}

void  GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value)
{
    *(volatile uint32_t *)((uint32_t)&LATA + (port * OFFSET_REG)) = (*(volatile uint32_t *)((uint32_t)&LATA + (port * OFFSET_REG)) & (~mask)) | (mask & value);
}

uint32_t  GPIO_PortLatchRead(GPIO_PORT port)
{
    return (*(volatile uint32_t *)((uint32_t)&LATA + (port * OFFSET_REG)));
}

void  GPIO_PortSet(GPIO_PORT port, uint32_t mask)
{
	*(volatile uint32_t *)((uint32_t)&LATA +(port * OFFSET_REG)) |= mask;
}

void  GPIO_PortClear(GPIO_PORT port, uint32_t mask)
{
	*(volatile uint32_t *)((uint32_t)&LATA + (port * OFFSET_REG)) &= ~mask;
}

void  GPIO_PortToggle(GPIO_PORT port, uint32_t mask)
{
	*(volatile uint32_t *)((uint32_t)&LATA +(port * OFFSET_REG)) ^= mask;
}

void  GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask)
{
	*(volatile uint32_t *)((uint32_t)&TRISA +(port * OFFSET_REG)) |= mask;
}

void  GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask)
{
	*(volatile uint32_t *)((uint32_t)&TRISA + (port * OFFSET_REG)) &= ~mask;
}


void  GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask)
{
	*(volatile uint32_t *)((uint32_t)&CNEN0A +(port * OFFSET_INT)) |= mask;
}

void  GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask)
{
	*(volatile uint32_t *)((uint32_t)&CNEN0A + (port * OFFSET_INT)) &= ~mask;
}

// Section: GPIO APIs which operates on one pin at a time

void  GPIO_PinIntEnable(GPIO_PIN pin, GPIO_INTERRUPT_STYLE style)
{
    GPIO_PORT port;
    uint32_t mask;
    port = (GPIO_PORT)(pin>>4);
    mask =  (uint32_t)0x1U << (pin & 0xFU);

    if (style == GPIO_INTERRUPT_ON_MISMATCH){
        *(volatile uint32_t *)((uint32_t)&CNEN0A + (port * OFFSET_INT)) |= mask;
    }
        
    else if (style == GPIO_INTERRUPT_ON_POSITIVE_EDGE)
    {
        *(volatile uint32_t *)((uint32_t)&CNEN0A +(port * OFFSET_INT)) |= mask;
		*(volatile uint32_t *)((uint32_t)&CNEN1A +(port * OFFSET_INT)) &= ~mask;
    }
	else if (style == GPIO_INTERRUPT_ON_NEGATIVE_EDGE)
    {
        *(volatile uint32_t *)((uint32_t)&CNEN1A +(port * OFFSET_INT)) |=  mask;
        *(volatile uint32_t *)((uint32_t)&CNEN0A +(port * OFFSET_INT)) &= ~mask;
    }
	else if (style == GPIO_INTERRUPT_ON_ANY_EDGES)
	{
		*(volatile uint32_t *)((uint32_t)&CNEN1A +(port * OFFSET_INT)) |= mask;
		*(volatile uint32_t *)((uint32_t)&CNEN0A +(port * OFFSET_INT)) |= mask;
	}
    else
    {
        //Nothing to process
    }
}

void  GPIO_PinIntDisable(GPIO_PIN pin)
{
    GPIO_PORT port;
    uint32_t mask;

    port = (GPIO_PORT)(pin>>4);
    mask =  (uint32_t)0x1U << (pin & 0xFU);

    *(volatile uint32_t *)((uint32_t)&CNEN0A + (port * OFFSET_INT)) &= ~mask;
    *(volatile uint32_t *)((uint32_t)&CNEN1A + (port * OFFSET_INT)) &= ~mask;
}

bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const GPIO_PIN_CALLBACK callback,
    uintptr_t context
)
{
    uint8_t cb_index;
    uint8_t portIndex;

    portIndex = (uint8_t)(pin >> 4);

    for(cb_index = portNumCb[portIndex]; cb_index < portNumCb[portIndex +1U]; cb_index++)
    {
        if (portPinCbObj[cb_index].pin == pin)
        {
            portPinCbObj[cb_index].callback = callback;
            portPinCbObj[cb_index].context  = context;
            return true;
        }
    }
    return false;
}

// Section: Local Function Implementation

    
void __attribute__((used)) CNB_InterruptHandler(void)
{
    uint8_t cb_index;
    uint32_t status;
    GPIO_PIN pin;
    uintptr_t context;

    status  = CNSTATB;
    status &= CNEN0B;

    PORTB;
    IFS3bits.CNBIF = 0;

    /* Check pending events and call callback if registered */
    for(cb_index = 0U; cb_index< 1U; cb_index++)
    {
        pin = portPinCbObj[cb_index].pin;

        if((portPinCbObj[cb_index].callback != NULL) && ((status & (1UL << (pin & 0xFU))) != 0U))
        {
            context = portPinCbObj[cb_index].context;
            portPinCbObj[cb_index].callback (pin, context);
        }
    }
}


/*******************************************************************************
 End of File
*/
