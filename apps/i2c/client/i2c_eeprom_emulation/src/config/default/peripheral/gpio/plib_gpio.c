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
 



void GPIO_Initialize ( void )
{





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



/*******************************************************************************
 End of File
*/
