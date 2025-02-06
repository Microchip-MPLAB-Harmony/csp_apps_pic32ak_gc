/*******************************************************************************
  MPLAB Harmony TRAPS Source File

  File Name:
    traps.c

  Summary:
    This is the generated source file for TRAPS  
 
  Description:
    None

 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
#include <xc.h>
#include "traps.h"

void _BusErrorTrap(void);
void _AddressErrorTrap(void);
void _GeneralTrap(void);
void _MathErrorTrap(void);
void _StackErrorTrap(void);
void _IllegalInstructionTrap(void);

#define ERROR_HANDLER __attribute__((weak,interrupt,no_auto_psv))
#define FAILSAFE_STACK_GUARDSIZE 8
#define FAILSAFE_STACK_SIZE 32

/* Address of instruction that caused the exception. */
static uint32_t  exception_address;
 
/* Code identifying the cause of the exception. */
static uint32_t  exception_code;

// Section: Driver Interface Function Definitions

//@brief Halts
void __attribute__((weak)) TRAPS_halt_on_error(uint16_t code)  // 
{
    exception_code  = code;
    
    exception_address = PCTRAP;
    
    while(1)
    {
        #ifdef __DEBUG
        /* If we are in debug mode, cause a software breakpoint in the debugger */
        __builtin_software_breakpoint();
        #endif
    }
}

inline static void use_failsafe_stack(void)
{
    static uint8_t failsafe_stack[FAILSAFE_STACK_SIZE];
    asm volatile (
        "   mov    %[pstack], W15\n"
        :
        : [pstack]"r"(failsafe_stack)
    );

    /* Controls where the stack pointer limit is, relative to the end of the
     * failsafe stack
     */
    SPLIM = (uint32_t)(((uint8_t *)failsafe_stack) + sizeof(failsafe_stack) - (uint32_t) FAILSAFE_STACK_GUARDSIZE);
}

/** Bus error.**/
void ERROR_HANDLER _BusErrorTrap(void)
{
    if(INTCON3bits.DMABET == 1)
    {
      INTCON3bits.DMABET = 0;  //Clear the trap flag
      TRAPS_halt_on_error(TRAPS_BUS_DMA_ERR);
    }

    if(INTCON3bits.YRAMBET == 1)
    {
      INTCON3bits.YRAMBET = 0;  //Clear the trap flag
      TRAPS_halt_on_error(TRAPS_BUS_CPU_Y_ERR);
    }

    if(INTCON3bits.XRAMBET == 1)
    {
      INTCON3bits.XRAMBET = 0;  //Clear the trap flag
      TRAPS_halt_on_error(TRAPS_BUS_CPU_X_ERR);
    }

    if(INTCON3bits.CPUBET == 1)
    {
      INTCON3bits.CPUBET = 0;  //Clear the trap flag
      TRAPS_halt_on_error(TRAPS_BUS_CPU_INSTR_ERR);
    }

    while(1)
    {
    }
}

/** Address error.**/
void ERROR_HANDLER _AddressErrorTrap(void)
{
    INTCON1bits.ADDRERR = 0;  //Clear the trap flag
    TRAPS_halt_on_error(TRAPS_ADDRESS_ERR);
}

/** General error.**/
void ERROR_HANDLER _GeneralTrap(void)
{
    if(INTCON5bits.DMTE == 1)
    {
      INTCON5bits.DMTE = 0;  //Clear the trap flag 
      TRAPS_halt_on_error(TRAPS_DMT_ERR);
    }

    if(INTCON5bits.SOFT == 1)
    {
      INTCON5bits.SOFT = 0;  //Clear the trap flag 
      TRAPS_halt_on_error(TRAPS_SOFT_ERR);
    }

    if(INTCON5bits.WDTE == 1)
    {
      INTCON5bits.WDTE = 0;  //Clear the trap flag
      TRAPS_halt_on_error(TRAPS_WDT_ERR); 
    }
    while(1)
    {
    }
}

/** Math error.**/
void ERROR_HANDLER _MathErrorTrap(void)
{
    INTCON4bits.DIV0ERR = 0;  //Clear the trap flag        
    TRAPS_halt_on_error(TRAPS_DIV0_ERR);
}

/** Stack error.**/
void ERROR_HANDLER _StackErrorTrap(void)
{
    /* We use a failsafe stack: the presence of a stack-pointer error
     * means that we cannot trust the stack to operate correctly unless
     * we set the stack pointer to a safe place.
     */
    use_failsafe_stack(); 
    
    INTCON1bits.STKERR = 0;  //Clear the trap flag         
    TRAPS_halt_on_error(TRAPS_STACK_ERR);
}

/** Illegal instruction.**/
void ERROR_HANDLER _IllegalInstructionTrap(void)
{
    INTCON1bits.BADOPERR = 0;  //Clear the trap flag   
    TRAPS_halt_on_error(TRAPS_ILLEGAL_INSTRUCTION); 
}
