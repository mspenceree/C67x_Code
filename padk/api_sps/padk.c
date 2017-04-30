/***************************************************************************
 *
 *
 *         **   **    **  ******  ********  ********  ********  **    **    
 *        **    **   **  **   ** ********  ********  ********  **    **
 *       **     *****   **   **    **     **        **        **    **
 *      **       **    ******     **     ****      **        ********
 *     **       **    **  **     **     **        **        **    **
 *    *******  **    **   **    **     ********  ********  **    **
 *   *******  **    **    **   **     ********  ********  **    **  
 *
 *            L Y R T E C H   S I G N A L   P R O C E S S I N G              
 *
 ***************************************************************************
 *                                                                          
 *  Project     : PADK
 *  File        : PADK.c
 *  Description	: Evaluation Board Driver
 *
 *						Copyright (c) Lyrtech inc. 2005						
 *
 ***************************************************************************
 *                                      													
 * "$Revision: 1.1 $"
 * "$Name:  $"
 * "$Date: 2006/02/22 19:32:16 $"
 * "$Author: louischarles.dumais $"
 *
 ***************************************************************************/

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "PADK.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**************************************************************************** 
 *
 * NAME  
 *      PADK_Init
 *
 * PURPOSE:
 *		Reset all external peripherals.
 *
 * USAGE
 *      This routine is C-callable and can be called as:
 *
 *		int PADK_Init() 
 *
 * RETURN VALUE
 *		Error code
 *
 * REFERENCE
 *
 * NOTE     
 * 		
 ****************************************************************************/ 
far int PADK_Init()
{
	int res = 0;
	volatile int cnt;
	
	// hardware reset of the different parts of the board
	// Uart, Midi, ADCs, DIR, DAC, PLL
	
	// assert reset
	FPGA_MISC_CTRL = (   (FPGA_MISC_CTRL & 
						~( NOT_PER_RESET_BIT | NOT_ADC_RESET_BIT ) )
						| UART_RESET_BIT
					  );

	cnt = 0;
	while( cnt++< 1000000 );
	
	// deassert reset
	FPGA_MISC_CTRL = (   (FPGA_MISC_CTRL & ~UART_RESET_BIT) |
						( NOT_PER_RESET_BIT | NOT_ADC_RESET_BIT ) 
					  );
					  
	// configure devices with default values
	res |= UART_reset();
/*	res |= MIDI_reset();	
	res |= CLKGEN_reset();
	res |= ADC_reset();
	res |= DAC_reset();
	res |= DIR_reset();
	res |= GPIO_reset();	   fixthis */
	USB_DISABLE_ALL();
	
	return res;
}
