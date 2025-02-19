/*******************************************************************************
  ptg PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_ptg.c
 
  Summary:
    ptg PLIB Source File
 
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
#include <stdlib.h>
#include <stdbool.h>
#include "device.h"
#include "plib_ptg.h"
#include "interrupts.h"

//Section: Data Type Definitions

volatile static PTG_EVENTS_CALLBACK_OBJECT trigger0Obj;
volatile static PTG_EVENTS_CALLBACK_OBJECT trigger1Obj;
volatile static PTG_EVENTS_CALLBACK_OBJECT trigger2Obj;
volatile static PTG_EVENTS_CALLBACK_OBJECT trigger3Obj;
volatile static PTG_EVENTS_CALLBACK_OBJECT triggerWdtObj;


void  PTG0_InterruptHandler(void);
void  PTG1_InterruptHandler(void);

//Section: Macro Definitions

#define PTGCON_PTGWDT_TIMEOUT_DISABLED      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(0) <<_PTGCON_PTGWDT_POSITION))) 
#define PTGCON_PTGWDT_TIMEOUT_8      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(1) <<_PTGCON_PTGWDT_POSITION))) 
#define PTGCON_PTGWDT_TIMEOUT_16      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(2) <<_PTGCON_PTGWDT_POSITION))) 
#define PTGCON_PTGWDT_TIMEOUT_32      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(3) <<_PTGCON_PTGWDT_POSITION))) 
#define PTGCON_PTGWDT_TIMEOUT_64      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(4) <<_PTGCON_PTGWDT_POSITION))) 
#define PTGCON_PTGWDT_TIMEOUT_128      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(5) <<_PTGCON_PTGWDT_POSITION))) 
#define PTGCON_PTGWDT_TIMEOUT_256      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(6) <<_PTGCON_PTGWDT_POSITION))) 
#define PTGCON_PTGWDT_TIMEOUT_512      ((uint32_t)(_PTGCON_PTGWDT_MASK & ((uint32_t)(7) <<_PTGCON_PTGWDT_POSITION))) 

#define PTGCON_PTGITM_MODE_0      ((uint32_t)(_PTGCON_PTGITM_MASK & ((uint32_t)(0) <<_PTGCON_PTGITM_POSITION))) 
#define PTGCON_PTGITM_MODE_1      ((uint32_t)(_PTGCON_PTGITM_MASK & ((uint32_t)(1) <<_PTGCON_PTGITM_POSITION))) 
#define PTGCON_PTGITM_MODE_2      ((uint32_t)(_PTGCON_PTGITM_MASK & ((uint32_t)(2) <<_PTGCON_PTGITM_POSITION))) 
#define PTGCON_PTGITM_MODE_3      ((uint32_t)(_PTGCON_PTGITM_MASK & ((uint32_t)(3) <<_PTGCON_PTGITM_POSITION))) 

#define PTGCON_PTGDIV_DIVIDE_BY_1    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(0) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_2    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(1) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_3    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(2) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_4    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(3) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_5    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(4) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_6    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(5) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_7    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(6) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_8    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(7) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_9    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(8) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_10    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(9) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_11    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(10) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_12    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(11) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_13    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(12) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_14    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(13) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_15    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(14) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_16    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(15) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_17    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(16) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_18    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(17) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_19    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(18) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_20    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(19) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_21    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(20) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_22    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(21) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_23    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(22) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_24    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(23) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_25    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(24) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_26    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(25) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_27    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(26) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_28    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(27) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_29    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(28) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_30    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(29) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_31    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(30) <<_PTGCON_PTGDIV_POSITION)))
#define PTGCON_PTGDIV_DIVIDE_BY_32    ((uint32_t)(_PTGCON_PTGDIV_MASK & ((uint32_t)(31) <<_PTGCON_PTGDIV_POSITION)))

// Section: PTG PLIB Routines
void PTG_Initialize (void)
{
	PTGCON = (PTGCON_PTGWDT_TIMEOUT_DISABLED
			|PTGCON_PTGITM_MODE_0
			|PTGCON_PTGDIV_DIVIDE_BY_1);
			
	PTGBTE   = 0x0UL;
	PTGHOLD  = 0x1UL;
	PTGT0LIM = 0xfa00UL;
	PTGT1LIM = 0x8UL;
	PTGSDLIM = 0x8UL;
    PTGC0LIM = 0x1UL;
	PTGC1LIM = 0x1UL;
	PTGADJ   = 0x1UL;
	PTGQPTR  = 0x0UL;
	

  /** 
   Step Commands 
  */

 PTG_STEP0 = PTGWLO | 0xfU; //Trigger Input from INT2 PPS
 PTG_STEP1 = PTGCTRL | 0x8U; //Wait for PTG Timer0 to match PTGT0LIM
 PTG_STEP2 = PTGIRQ | 0x0U; //Generate PTG Interrupt 0
 PTG_STEP3 = PTGTRIG | 0xcU; //Trigger for ADC Sample Trigger
 PTG_STEP4 = PTGIRQ | 0x1U; //Generate PTG Interrupt 1
 PTG_STEP5 = PTGCTRL | 0x0U; //NOP
 PTG_STEP6 = PTGCTRL | 0x0U; //NOP
 PTG_STEP7 = PTGCTRL | 0x0U; //NOP
 PTG_STEP8 = PTGCTRL | 0x0U; //NOP
 PTG_STEP9 = PTGCTRL | 0x0U; //NOP
 PTG_STEP10 = PTGCTRL | 0x0U; //NOP
 PTG_STEP11 = PTGCTRL | 0x0U; //NOP
 PTG_STEP12 = PTGCTRL | 0x0U; //NOP
 PTG_STEP13 = PTGCTRL | 0x0U; //NOP
 PTG_STEP14 = PTGCTRL | 0x0U; //NOP
 PTG_STEP15 = PTGCTRL | 0x0U; //NOP
 PTG_STEP16 = PTGCTRL | 0x0U; //NOP
 PTG_STEP17 = PTGCTRL | 0x0U; //NOP
 PTG_STEP18 = PTGCTRL | 0x0U; //NOP
 PTG_STEP19 = PTGCTRL | 0x0U; //NOP
 PTG_STEP20 = PTGCTRL | 0x0U; //NOP
 PTG_STEP21 = PTGCTRL | 0x0U; //NOP
 PTG_STEP22 = PTGCTRL | 0x0U; //NOP
 PTG_STEP23 = PTGCTRL | 0x0U; //NOP
 PTG_STEP24 = PTGCTRL | 0x0U; //NOP
 PTG_STEP25 = PTGCTRL | 0x0U; //NOP
 PTG_STEP26 = PTGCTRL | 0x0U; //NOP
 PTG_STEP27 = PTGCTRL | 0x0U; //NOP
 PTG_STEP28 = PTGCTRL | 0x0U; //NOP
 PTG_STEP29 = PTGCTRL | 0x0U; //NOP
 PTG_STEP30 = PTGCTRL | 0x0U; //NOP
 PTG_STEP31 = PTGCTRL | 0x0U; //NOP

	//Clear  interrupt flag  
    IFS4bits.PTG0IF = 0;
    //Enable  interrupt
    IEC4bits.PTG0IE = 1;
	//Clear  interrupt flag  
    IFS4bits.PTG1IF = 0;
    //Enable  interrupt
    IEC4bits.PTG1IE = 1;
	
}

void PTG_Deinitialize (void)
{
    PTG_Disable();
	IFS4bits.PTG0IF = 0;
	IEC4bits.PTG0IE = 0;
	IFS4bits.PTG1IF = 0;
	IEC4bits.PTG1IE = 0;
	
    PTGCON = 0x0UL;
    PTGBTE = 0x0UL;
    PTGHOLD = 0x0UL;
    PTGT0LIM = 0x0UL;
    PTGT1LIM = 0x0UL;
    PTGSDLIM = 0x0UL;
    PTGC0LIM = 0x0UL;
    PTGC1LIM = 0x0UL;
    PTGADJ = 0x0UL;
    PTGL0 = 0x0UL;
    PTGQPTR = 0x0UL;

}

void PTG_Enable (void)
{
    PTGCONbits.ON = 1;
}

void PTG_StepSequenceStart (void)
{
    PTGCONbits.PTGSTRT = 1; 
}

void PTG_SoftwareTriggerSet (void)
{
    PTGCONbits.PTGSWT = 1;    
}

void PTG_SoftwareTriggerClear (void)
{
    PTGCONbits.PTGSWT = 0;    
}

bool PTG_WatchdogTimeoutStatusGet (void)
{
    return( PTGCONbits.PTGWDTO);
}

void PTG_StepSequenceStop (void)
{
    PTGCONbits.PTGSTRT = 0;
}   

void PTG_Disable (void)
{
    PTGCONbits.ON = 0;
}

void PTG_EventCallbackRegister(PTG_EVENTS event, PTG_EVENTS_CALLBACK callback_fn, uintptr_t context)
{
    switch(event)
   {
	case TRIGGER0:
		trigger0Obj.callback_fn = callback_fn;
		trigger0Obj.context = context;
		break;
	case TRIGGER1:
		trigger1Obj.callback_fn = callback_fn;
		trigger1Obj.context = context;
		break;
	case TRIGGER2:
		trigger2Obj.callback_fn = callback_fn;
		trigger2Obj.context = context;
		break;
	case TRIGGER3:
		trigger3Obj.callback_fn = callback_fn;
		trigger3Obj.context = context;
		break;
	
	case WATCHDOGTIMER:
		triggerWdtObj.callback_fn = callback_fn;
		triggerWdtObj.context = context;
		break;
	default:
		break;
	}
}
void __attribute__ ( ( used ) ) PTG0_InterruptHandler( void )

{
    if(trigger0Obj.callback_fn != NULL)
    {
        uintptr_t context = trigger0Obj.context;
		trigger0Obj.callback_fn(context);
    }
	IFS4bits.PTG0IF = 0;
}
void __attribute__ ( ( used ) ) PTG1_InterruptHandler( void )

{
    if(trigger1Obj.callback_fn != NULL)
    {
        uintptr_t context = trigger1Obj.context;
		trigger1Obj.callback_fn(context);
    }
	IFS4bits.PTG1IF = 0;
}

