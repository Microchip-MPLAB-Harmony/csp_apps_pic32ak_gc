/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "definitions.h"
#include "device.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************

// Configuration Bit Settings

// FCP
#pragma config FCP_CP = OFF            // Memory protection is disabled
#pragma config FCP_CRC = OFF            // CRC is disabled
#pragma config FCP_WPUCA = OFF            // Protection is disabled

// FICD
#pragma config FICD_JTAGEN = ON            // JTAG is enabled

// FDEVOPT
#pragma config FDEVOPT_ALTI2C1 = OFF            // Primary I2C1 pins are used
#pragma config FDEVOPT_ALTI2C2 = OFF            // Primary I2C2 pins are used
#pragma config FDEVOPT_BISTDIS = OFF            // RAM test after reset is enabled
#pragma config FDEVOPT_SPI2PIN = OFF            // SPI2 pins are selected by peripheral pin selection feature

// FWDT
#pragma config FWDT_WINDIS = ON            // Watchdog Timer operates in Non-Window mode
#pragma config FWDT_SWDTMPS = PS2147483648            // Sleep Mode Watchdog Timer Post Scaler select bits
#pragma config FWDT_RCLKSEL = BFRC256            // WDT Run Mode uses BFRC:256
#pragma config FWDT_RWDTPS = PS2147483648            // Run Mode Watchdog Timer Post Scaler select bits
#pragma config FWDT_WDTWIN = WIN25            // WDT Window is 25% of WDT period
#pragma config FWDT_WDTEN = SW            // WDT is controlled by software, use WDTCON.ON bit
#pragma config FWDT_WDTRSTEN = ON            // WDT event generates a reset

// FPR0CTRL
#pragma config FPR0CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR0CTRL_EX = ON            // Code execution is enabled
#pragma config FPR0CTRL_RD = ON            // Read operation is enabled
#pragma config FPR0CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR0CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR0CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR0ST
#pragma config FPR0ST_START = 0x0UL            // Region start address bits

// FPR0END
#pragma config FPR0END_END = 0x0UL            // Region end address bits

// FPR1CTRL
#pragma config FPR1CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR1CTRL_EX = ON            // Code execution is enabled
#pragma config FPR1CTRL_RD = ON            // Read operation is enabled
#pragma config FPR1CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR1CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR1CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR1ST
#pragma config FPR1ST_START = 0x0UL            // Region start address bits

// FPR1END
#pragma config FPR1END_END = 0x0UL            // Region end address bits

// FPR2CTRL
#pragma config FPR2CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR2CTRL_EX = ON            // Code execution is enabled
#pragma config FPR2CTRL_RD = ON            // Read operation is enabled
#pragma config FPR2CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR2CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR2CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR2ST
#pragma config FPR2ST_START = 0x0UL            // Region start address bits

// FPR2END
#pragma config FPR2END_END = 0x0UL            // Region end address bits

// FPR3CTRL
#pragma config FPR3CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR3CTRL_EX = ON            // Code execution is enabled
#pragma config FPR3CTRL_RD = ON            // Read operation is enabled
#pragma config FPR3CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR3CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR3CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR3ST
#pragma config FPR3ST_START = 0x0UL            // Region start address bits

// FPR3END
#pragma config FPR3END_END = 0x0UL            // Region end address bits

// FPR4CTRL
#pragma config FPR4CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR4CTRL_EX = ON            // Code execution is enabled
#pragma config FPR4CTRL_RD = ON            // Read operation is enabled
#pragma config FPR4CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR4CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR4CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR4ST
#pragma config FPR4ST_START = 0x0UL            // Region start address bits

// FPR4END
#pragma config FPR4END_END = 0x0UL            // Region end address bits

// FPR5CTRL
#pragma config FPR5CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR5CTRL_EX = ON            // Code execution is enabled
#pragma config FPR5CTRL_RD = ON            // Read operation is enabled
#pragma config FPR5CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR5CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR5CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR5ST
#pragma config FPR5ST_START = 0x0UL            // Region start address bits

// FPR5END
#pragma config FPR5END_END = 0x0UL            // Region end address bits

// FPR6CTRL
#pragma config FPR6CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR6CTRL_EX = ON            // Code execution is enabled
#pragma config FPR6CTRL_RD = ON            // Read operation is enabled
#pragma config FPR6CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR6CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR6CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR6ST
#pragma config FPR6ST_START = 0x0UL            // Region start address bits

// FPR6END
#pragma config FPR6END_END = 0x0UL            // Region end address bits

// FPR7CTRL
#pragma config FPR7CTRL_RDIS = ON            // Protection is disabled
#pragma config FPR7CTRL_EX = ON            // Code execution is enabled
#pragma config FPR7CTRL_RD = ON            // Read operation is enabled
#pragma config FPR7CTRL_WR = ON            // Write and erase operation is enabled
#pragma config FPR7CTRL_CRC = ON            // NVM controller CRC calculation is enabled
#pragma config FPR7CTRL_RTYPE = FIRMWARE            // Firmware configurable region

// FPR7ST
#pragma config FPR7ST_START = 0x0UL            // Region start address bits

// FPR7END
#pragma config FPR7END_END = 0x0UL            // Region end address bits

// FIRT
#pragma config FIRT_IRT = OFF            // Immutable root of trust (IRT) regions are disabled

// FSECDBG
#pragma config FSECDBG_SECDBG = OFF            // Secure debug is disabled

// FPED
#pragma config FPED_ICSPPED = OFF            // ICSP can program and erase

// FEPUCB
#pragma config FEPUCB_EPUCB = 0xffffffffUL            // UCB erase protection bits

// FWPUCB
#pragma config FWPUCB_WPUCB = 0xffffffffUL            // UCB write protection bits









// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
/* Following MISRA-C rules are deviated in the below code block */
/* MISRA C-2012 Rule 7.2 - Deviation record ID - H3_MISRAC_2012_R_7_2_DR_1 */
/* MISRA C-2012 Rule 11.1 - Deviation record ID - H3_MISRAC_2012_R_11_1_DR_1 */
/* MISRA C-2012 Rule 11.3 - Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
/* MISRA C-2012 Rule 11.8 - Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */



// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************

/* MISRAC 2012 deviation block end */

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{

    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */

    /* Start out with interrupts disabled before configuring any modules */
    (void)__builtin_disable_interrupts();

  
	CLOCK_Initialize();



	GPIO_Initialize();

	PWM_Initialize();


    INTC_Initialize();

	/* Enable global interrupts */
    (void)__builtin_enable_interrupts();



    /* MISRAC 2012 deviation block end */
}

/*******************************************************************************
 End of File
*/
