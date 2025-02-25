/*******************************************************************************
  Clock PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_clk.c
 
  Summary:
    Clock PLIB Source File
 
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

// Section: Includes
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "device.h"
#include "interrupts.h"
#include "plib_clk.h"
#include "plib_clk_common.h"

// Section: Macro Definitions

//CLOCK PLLxCON NOSC options
#define PLL1CON_NOSC_PGC          ((uint32_t)(_PLL1CON_NOSC_MASK & ((uint32_t)(0) << _PLL1CON_NOSC_POSITION))) 
#define PLL1CON_NOSC_FRC          ((uint32_t)(_PLL1CON_NOSC_MASK & ((uint32_t)(1) << _PLL1CON_NOSC_POSITION))) 
#define PLL1CON_NOSC_BFRC          ((uint32_t)(_PLL1CON_NOSC_MASK & ((uint32_t)(2) << _PLL1CON_NOSC_POSITION))) 
#define PLL1CON_NOSC_POSC          ((uint32_t)(_PLL1CON_NOSC_MASK & ((uint32_t)(3) << _PLL1CON_NOSC_POSITION))) 
#define PLL1CON_NOSC_REFI1          ((uint32_t)(_PLL1CON_NOSC_MASK & ((uint32_t)(9) << _PLL1CON_NOSC_POSITION))) 
#define PLL1CON_NOSC_REFI2          ((uint32_t)(_PLL1CON_NOSC_MASK & ((uint32_t)(10) << _PLL1CON_NOSC_POSITION))) 
#define PLL1CON_BOSC_PGC          ((uint32_t)(_PLL1CON_BOSC_MASK & ((uint32_t)(0) << _PLL1CON_BOSC_POSITION))) 
#define PLL1CON_BOSC_FRC          ((uint32_t)(_PLL1CON_BOSC_MASK & ((uint32_t)(1) << _PLL1CON_BOSC_POSITION))) 
#define PLL1CON_BOSC_BFRC          ((uint32_t)(_PLL1CON_BOSC_MASK & ((uint32_t)(2) << _PLL1CON_BOSC_POSITION))) 
#define PLL1CON_BOSC_POSC          ((uint32_t)(_PLL1CON_BOSC_MASK & ((uint32_t)(3) << _PLL1CON_BOSC_POSITION))) 
#define PLL1CON_BOSC_REFI1          ((uint32_t)(_PLL1CON_BOSC_MASK & ((uint32_t)(9) << _PLL1CON_BOSC_POSITION))) 
#define PLL1CON_BOSC_REFI2          ((uint32_t)(_PLL1CON_BOSC_MASK & ((uint32_t)(10) << _PLL1CON_BOSC_POSITION))) 

//CLOCK PLLxCON PLLPRE set
#define PLL1DIV_PLLPRE_SET(value)          ((uint32_t)(_PLL1DIV_PLLPRE_MASK & ((uint32_t)(value) << _PLL1DIV_PLLPRE_POSITION))) 
//CLOCK PLLxCON PLLFBDIV set
#define PLL1DIV_PLLFBDIV_SET(value)          ((uint32_t)(_PLL1DIV_PLLFBDIV_MASK & ((uint32_t)(value) << _PLL1DIV_PLLFBDIV_POSITION))) 
//CLOCK PLLxCON POSTDIV1 set
#define PLL1DIV_POSTDIV1_SET(value)          ((uint32_t)(_PLL1DIV_POSTDIV1_MASK & ((uint32_t)(value) << _PLL1DIV_POSTDIV1_POSITION))) 
//CLOCK PLLxCON POSTDIV2 set
#define PLL1DIV_POSTDIV2_SET(value)          ((uint32_t)(_PLL1DIV_POSTDIV2_MASK & ((uint32_t)(value) << _PLL1DIV_POSTDIV2_POSITION))) 

//CLOCK VCOxDIV INTDIV set
#define VCO1DIV_INTDIV_SET(value)          ((uint32_t)(_VCO1DIV_INTDIV_MASK & ((uint32_t)(value) << _VCO1DIV_INTDIV_POSITION))) 

//CLOCK CLKxCON NOSC options
#define CLK1CON_NOSC_PGC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(0) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_FRC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(1) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_BFRC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(2) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_POSC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(3) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_LPRC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(4) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(5) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(6) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(7) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(8) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_REFI1          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(9) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_REFI2          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(10) << _CLK1CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK1CON_BOSC_PGC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(0) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_FRC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(1) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_BFRC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(2) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_POSC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(3) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_LPRC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(4) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(5) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(6) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(7) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(8) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_REFI1          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(9) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_REFI2          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(10) << _CLK1CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK1DIV_INTDIV_SET(value)          ((uint32_t)(_CLK1DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK1DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK1DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK1DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK1DIV_FRACDIV_POSITION))) 

//CLOCK CLKxCON NOSC options
#define CLK2CON_NOSC_PGC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(0) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_FRC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(1) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_BFRC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(2) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_POSC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(3) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_LPRC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(4) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(5) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(6) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(7) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(8) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_REFI1          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(9) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_REFI2          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(10) << _CLK2CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK2CON_BOSC_PGC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(0) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_FRC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(1) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_BFRC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(2) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_POSC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(3) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_LPRC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(4) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(5) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(6) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(7) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(8) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_REFI1          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(9) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_REFI2          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(10) << _CLK2CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK2DIV_INTDIV_SET(value)          ((uint32_t)(_CLK2DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK2DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK2DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK2DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK2DIV_FRACDIV_POSITION))) 

//CLOCK CLKxCON NOSC options
#define CLK3CON_NOSC_PGC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(0) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_FRC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(1) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_BFRC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(2) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_POSC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(3) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_LPRC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(4) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(5) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(6) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(7) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(8) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_REFI1          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(9) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_REFI2          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(10) << _CLK3CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK3CON_BOSC_PGC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(0) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_FRC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(1) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_BFRC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(2) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_POSC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(3) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_LPRC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(4) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(5) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(6) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(7) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(8) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_REFI1          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(9) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_REFI2          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(10) << _CLK3CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK3DIV_INTDIV_SET(value)          ((uint32_t)(_CLK3DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK3DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK3DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK3DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK3DIV_FRACDIV_POSITION))) 

//CLOCK CLKxCON NOSC options
#define CLK12CON_NOSC_PGC          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(0) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_FRC          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(1) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_BFRC          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(2) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_POSC          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(3) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_LPRC          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(4) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(5) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(6) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(7) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(8) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_REFI1          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(9) << _CLK12CON_NOSC_POSITION))) 
#define CLK12CON_NOSC_REFI2          ((uint32_t)(_CLK12CON_NOSC_MASK & ((uint32_t)(10) << _CLK12CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK12CON_BOSC_PGC          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(0) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_FRC          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(1) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_BFRC          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(2) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_POSC          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(3) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_LPRC          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(4) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(5) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(6) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(7) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(8) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_REFI1          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(9) << _CLK12CON_BOSC_POSITION))) 
#define CLK12CON_BOSC_REFI2          ((uint32_t)(_CLK12CON_BOSC_MASK & ((uint32_t)(10) << _CLK12CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK12DIV_INTDIV_SET(value)          ((uint32_t)(_CLK12DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK12DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK12DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK12DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK12DIV_FRACDIV_POSITION))) 


#define PLL1FOUT_SOURCE         5U
#define PLL2VCODIV_SOURCE       8U 

// Section: Static Variables



void CLOCK_Initialize(void)
{
    /*  
        System Clock Source                             :  PLL1 FOUT
        System/Generator 1 frequency (Fosc)             :  200.0 MHz
        
        Clock Generator 2 frequency                     : 8.0 MHz
        Clock Generator 3 frequency                     : 8.0 MHz
        Clock Generator 12 frequency                     : 4.0 MHz
        
        PLL 1 frequency                                 : 200.0 MHz
        PLL 1 VCO Out frequency                         : 800.0 MHz

    */
    //Primary oscillator settings 
    OSCCFGbits.POSCMD = 3U;
    
    //If CLK GEN 1 (system clock) is using a PLL, switch to FRC to avoid risk of over-clocking the CPU while changing PLL settings
    uint32_t currentSysClock = CLK1CONbits.COSC;
    if((currentSysClock >= PLL1FOUT_SOURCE) && (currentSysClock <= PLL2VCODIV_SOURCE))
    {
        CLK1CONbits.NOSC = 1U; //FRC as source 
        CLK1CONbits.OSWEN = 1U;
        while(CLK1CONbits.OSWEN == 1U){};
    }
    
    //PLL 1 settings
    PLL1CON = (_PLL1CON_ON_MASK
                |PLL1CON_NOSC_FRC
                |PLL1CON_BOSC_BFRC);
    PLL1DIV = (PLL1DIV_PLLPRE_SET(1)
                 |PLL1DIV_PLLFBDIV_SET(200)
                 |PLL1DIV_POSTDIV1_SET(4)
                 |PLL1DIV_POSTDIV2_SET(2));
    //Enable PLL Input and Feedback Divider update
    PLL1CONbits.PLLSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR 
    while (PLL1CONbits.PLLSWEN == 1){};
#endif
    PLL1CONbits.FOUTSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR 
    while (PLL1CONbits.FOUTSWEN == 1U){};
#endif
    //Enable clock switching
    PLL1CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for switching
    while(PLL1CONbits.OSWEN == 1U){}; 
    //Wait for clock to be ready
    while(OSCCTRLbits.PLL1RDY == 0U){}; 
#endif  
    //Configure VCO Divider 
    VCO1DIV = VCO1DIV_INTDIV_SET(1);
    //Enable PLL VCO divider
    PLL1CONbits.DIVSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for setup complete
    while(PLL1CONbits.DIVSWEN == 1U){}; 
#endif
    
    //Clearing ON shuts down oscillator when no downstream clkgen or peripheral is requesting the clock
    PLL1CONbits.ON = 0U;
    //Clock Generator 1 settings
    CLK1CON = (_CLK1CON_ON_MASK
                |CLK1CON_NOSC_PLL1_FOUT
                |CLK1CON_BOSC_BFRC
                |_CLK1CON_FSCMEN_MASK);
    //Enable clock switching
    CLK1CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK1CONbits.OSWEN == 1U){};
#endif

    //Clock Generator 2 settings
    CLK2CON = (_CLK2CON_ON_MASK
                |CLK2CON_NOSC_FRC
                |CLK2CON_BOSC_BFRC);
    //Enable clock switching
    CLK2CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK2CONbits.OSWEN == 1U){};
#endif

    //Clock Generator 3 settings
    CLK3CON = (_CLK3CON_ON_MASK
                |CLK3CON_NOSC_BFRC
                |CLK3CON_BOSC_FRC);
    //Enable clock switching
    CLK3CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK3CONbits.OSWEN == 1U){};
#endif

    //Clock Generator 12 settings
    CLK12CON = (_CLK12CON_ON_MASK
                |CLK12CON_NOSC_PLL1_FOUT
                |CLK12CON_BOSC_BFRC);
    CLK12DIV = (CLK12DIV_INTDIV_SET(25)
                 |CLK12DIV_FRACDIV_SET(0));
    //Enable divide factors
    CLK12CONbits.DIVSWEN = 1U; 
#ifndef __MPLAB_DEBUGGER_SIMULATOR
    //Wait for divide factors to get updated
    while(CLK12CONbits.DIVSWEN == 1U){};
#endif
    //Enable clock switching
    CLK12CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK12CONbits.OSWEN == 1U){};
#endif

}

 
